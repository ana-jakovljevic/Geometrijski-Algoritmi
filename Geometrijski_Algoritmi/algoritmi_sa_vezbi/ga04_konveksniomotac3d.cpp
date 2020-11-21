#include "ga04_konveksniomotac3d.h"
#include "pomocnefunkcije.h"
#include <fstream>

KonveksniOmotac3D::KonveksniOmotac3D(QWidget *pCrtanje,
                                     int pauzaKoraka,
                                     std::string imeDatoteke,
                                     int broj_tacaka)
        :AlgoritamBaza(pCrtanje, pauzaKoraka)
{
    if (imeDatoteke != "")
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
    else
        _tacke = generisiNasumicneTacke(broj_tacaka);
}

/*--------------------------------------------------------------------------------------------------*/
/*---------------------------------Ucitavanje podataka----------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
std::vector<Teme *> KonveksniOmotac3D::generisiNasumicneTacke(int broj_tacaka) const
{
    srand(static_cast<unsigned>(time(0)));

    std::vector<Teme*> randomPoints;

    for(int i=0; i < broj_tacaka; i++)
        randomPoints.emplace_back(new Teme(
           1.*rand()/RAND_MAX,
           1.*rand()/RAND_MAX,
           1.*rand()/RAND_MAX));

    return randomPoints;
}

std::vector<Teme *> KonveksniOmotac3D::ucitajPodatkeIzDatoteke(std::string imeDatoteke) const
{
    std::ifstream inputFile(imeDatoteke);
    std::vector<Teme*> points;
    float x, y, z;
    while(inputFile >> x >> y >> z)
    {
        points.emplace_back(new Teme(x, y, z));
    }
    return points;
}


/*--------------------------------------------------------------------------------------------------*/
/*---------------------------------Implementacija---------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

void KonveksniOmotac3D::pokreniAlgoritam()
{
    if(!Tetraedar()){
        emit animacijaZavrsila();
        return;
    }

    for(auto i=0ul; i<_tacke.size() ;i++)
    {
        if(!_tacke[i]->getObradjeno())
        {
            DodajTeme(_tacke[i]);
            ObrisiVisak();
            _tacke[i]->setObradjeno(true);
            AlgoritamBaza_updateCanvasAndBlock();
        }
    }

    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

bool KonveksniOmotac3D::Tetraedar()
{
    auto it = _tacke.begin();
    Teme *t1 = *it;
    t1->setObradjeno(true);
    it++;
    Teme *t2 = *it;
    t2->setObradjeno(true);
    it++;
    for(;it != _tacke.end(); it++)
        if(!kolinearne(t1, t2, *it))
            break;
    if(it == _tacke.end())
        return false;

    Teme *t3 = *it;
    t3->setObradjeno(true);

    for(;it != _tacke.end(); it++)
        if(pomocneFunkcije::zapremina(t1->koordinate(), t2->koordinate(), t3->koordinate(), (*it)->koordinate()) != 0)
            break;
    if(it == _tacke.end())
        return false;

    Teme *t4 = *it;
    t4->setObradjeno(true);

    Ivica *i1 = new Ivica(t1, t2);
    Ivica *i2 = new Ivica(t2, t3);
    Ivica *i3 = new Ivica(t3, t1);
    Stranica *s1 = new Stranica(t1, t2, t3);
    _ivice.insert(i1);
    _ivice.insert(i2);
    _ivice.insert(i3);

    i1->postavi_stranicu(s1);
    i2->postavi_stranicu(s1);
    i3->postavi_stranicu(s1);

    if(zapremina6(s1, t4) < 0){
        s1->izmeniRedosledTemena();
        i1->izmeniRedosledTemena();
        i2->izmeniRedosledTemena();
        i3->izmeniRedosledTemena();
    }

    i1->postavi_stranicu(napraviDruguStranicu(i1, t4));
    i2->postavi_stranicu(napraviDruguStranicu(i2, t4));
    i3->postavi_stranicu(napraviDruguStranicu(i3, t4));

    return true;
}

void KonveksniOmotac3D::DodajTeme(Teme* t)
{
    bool vidljiva = false;

    /* Kroz stranice iteriramo tako sto zapravo
     * uzimamo dve po dve prolaskom kroz ivice */
    for(auto ivica : _ivice){
        Stranica *s1 = ivica->s1();
        double zapremina = zapremina6(s1, t);

        if(zapremina < 0){
            vidljiva = true;
            s1->setVidljiva(true);
        }

        Stranica *s2 = ivica->s2();
        zapremina = zapremina6(s2, t);

        if(zapremina < 0){
            vidljiva = true;
            s2->setVidljiva(true);
        }
    }

    if(!vidljiva){
        return;
    }

    /* Skup ivica nije konstantan, pa je ovde neophodno kopirati ih u vektor, kako
     * bi se proslo samo kroz stare ivice, ne i one koje su novododate u prolazu.
     * Ovo mozda zvuci skupo, ali je zapravo vrlo jeftino i ne utice na slozenost. */
    for(auto ivica: std::vector<Ivica*>(_ivice.begin(), _ivice.end())){
        const auto s1Losa = ivica->s1()->getVidljiva();
        const auto s2Losa = ivica->s2()->getVidljiva();
        if(s1Losa && s2Losa)
            ivica->setObrisati(true);
        else if(s1Losa)
            ivica->zameniVidljivuStranicu(napraviPrvuStranicu(ivica, t), 0);
        else if(s2Losa)
            ivica->zameniVidljivuStranicu(napraviDruguStranicu(ivica, t), 1);
    }

    /* Dakle, u okviru pravljenja stranica, cesto postoji potreba za pravljenjem novih ivica,
     * pri cemu im se dodeljuje samo jedna stranica, dok je druga jos nepoznata. Pri prolasku kroz
     * novododate ivice, neka moze doci na red za obradu i pre nego sto se otkrije druga stranica,
     * pa _noveIvice[i]->s2() lako moze biti nullptr. Stoga je treba napraviti pre nastavka petlje.
     * U suprotnom se dobija SIGSEGV (segmentation fault) za sve iole vece ulaze (npr. 100). */
    for (auto i = 0ul; i < _noveIvice.size(); i++) {
        const auto s1Losa = _noveIvice[i]->s1()->getVidljiva();
        const auto s2Losa = !_noveIvice[i]->s2() || _noveIvice[i]->s2()->getVidljiva();
        if(s1Losa && s2Losa)
            _noveIvice[i]->setObrisati(true);
        else if(s1Losa)
            _noveIvice[i]->zameniVidljivuStranicu(napraviPrvuStranicu(_noveIvice[i], t), 0);
        else if(s2Losa)
            _noveIvice[i]->zameniVidljivuStranicu(napraviDruguStranicu(_noveIvice[i], t), 1);
    }
    _noveIvice.clear();
}

void KonveksniOmotac3D::ObrisiVisak()
{
    std::experimental::erase_if(_ivice, [](Ivica* ivica){ return ivica->obrisati(); });
}

/*--------------------------------------------------------------------------------------------------*/
/*---------------------------------Pomocni metodi---------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
double KonveksniOmotac3D::zapremina6(Stranica *s, Teme *t) const
{
    return pomocneFunkcije::zapremina(s->t1()->koordinate(),
                s->t2()->koordinate(), s->t3()->koordinate(),
                t->koordinate());
}

bool KonveksniOmotac3D::kolinearne(Teme *a, Teme *b, Teme *c) const
{
    return pomocneFunkcije::kolinearne3D(a->koordinate(), b->koordinate(), c->koordinate());
}

Stranica* KonveksniOmotac3D::napraviPrvuStranicu(Ivica *iv, Teme *t){
    /* Pretraga u neuredjenom skupu je u konstantnom vremenu. Pravi
     * se zeljena ivica i zatim pronalazi fjom find. Ako je uspesno
     * nadjena, uzima se taj pokazivac. Inace se prosiruje skup. */
    Ivica* i1 = new Ivica(t, iv->t1());
    auto iv1 = _ivice.find(i1);
    if(iv1 != _ivice.end()){
        i1 = *iv1;
    } else {
        _ivice.insert(i1);
        _noveIvice.push_back(i1);
    }

    Ivica* i2 = new Ivica(iv->t2(), t);
    auto iv2 = _ivice.find(i2);
    if(iv2 != _ivice.end()){
        i2 = *iv2;
    } else {
        _ivice.insert(i2);
        _noveIvice.push_back(i2);
    }

    Stranica *s = new Stranica(iv->t1(),iv->t2(), t);
    i1->postavi_stranicu(s);
    i2->postavi_stranicu(s);

    return s;
}

Stranica* KonveksniOmotac3D::napraviDruguStranicu(Ivica *iv, Teme *t){
    Ivica* i1 = new Ivica(iv->t1(), t);
    auto iv1 = _ivice.find(i1);
    if(iv1 != _ivice.end()){
        i1 = *iv1;
    } else {
        _ivice.insert(i1);
        _noveIvice.push_back(i1);
    }

    Ivica* i2 = new Ivica(t, iv->t2());
    auto iv2 = _ivice.find(i2);
    if(iv2 != _ivice.end()){
        i2 = *iv2;
    } else {
        _ivice.insert(i2);
        _noveIvice.push_back(i2);
    }

    Stranica *s = new Stranica(iv->t2(),iv->t1(), t);
    i1->postavi_stranicu(s);
    i2->postavi_stranicu(s);

    return s;
}

/*--------------------------------------------------------------------------------------------------*/
/*---------------------------------Crtanje i Naivni algoritam---------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

void KonveksniOmotac3D::crtajAlgoritam(QPainter*) const
{

}

void KonveksniOmotac3D::pokreniNaivniAlgoritam()
{
    /* Kao naivni moze da se uzmu svake 3 tacke i da se gleda da li su sve ostale
     * tacke sa iste strane stranice koja je kreirana od te 3 izabrane tacke.
     * Ako jesu sve sa iste strane onda izabrane 3 tacke ulaze u konveksni omotac.
     * Inace ne ulaze.
     * U rezultatu pamtimo odabrane stranice.
     */
}
