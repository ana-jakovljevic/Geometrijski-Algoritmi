#include "ga04_konveksniomotac3d.h"
#include "pomocnefunkcije.h"
#include <fstream>

KonveksniOmotac3D::KonveksniOmotac3D(QWidget *pCrtanje,
                                     int pauzaKoraka,
                                     std::string imeDatoteke,
                                     int broj_tacaka)
        :AlgoritamBaza(pCrtanje, pauzaKoraka)
{
    _znakZapremine = 1;

    if (imeDatoteke != "")
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
    else
        _tacke = generisiNasumicneTacke(broj_tacaka);
}

/*--------------------------------------------------------------------------------------------------*/
/*---------------------------------Ucitavanje podataka----------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
std::vector<Teme *> KonveksniOmotac3D::generisiNasumicneTacke(int broj_tacaka)
{
    srand(static_cast<unsigned>(time(0)));

    std::vector<Teme*> randomPoints;

    for(int i=0; i < broj_tacaka; i++)
        randomPoints.push_back(new Teme(QVector3D(
           1.*rand()/RAND_MAX,
           1.*rand()/RAND_MAX,
           1.*rand()/RAND_MAX)));

    return randomPoints;
}

std::vector<Teme *> KonveksniOmotac3D::ucitajPodatkeIzDatoteke(std::string imeDatoteke)
{
    std::ifstream inputFile(imeDatoteke);
    std::vector<Teme*> points;
    float x, y, z;
    while(inputFile >> x >> y >> z)
    {
        points.push_back(new Teme(QVector3D(x, y, z)));
    }
    return points;
}


/*--------------------------------------------------------------------------------------------------*/
/*---------------------------------Implementacija---------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

void KonveksniOmotac3D::pokreniAlgoritam()
{
    if(Tetraedar() == false){
        return;
    }

    for(auto i=0ul; i<_tacke.size() ;i++){
        if(!_tacke[i]->getObradjeno()){
            DodajTeme(_tacke[i]);
            ObrisiVisak();
            _tacke[i]->setObradjeno(true);
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
    _ivice.push_back(i1);
    _ivice.push_back(i2);
    _ivice.push_back(i3);
    _stranice.push_back(s1);

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
    int vidljiva = 0;
    for(int i=0; i<_stranice.size(); i++){

        double zapremina = zapremina6(_stranice[i], t);

        if(zapremina<0){
            vidljiva=1;
            _stranice[i]->setVidljiva(true);
        }
    }

    if(vidljiva==0){
        return;
    }

    for(int i=0; i<_ivice.size(); i++){
        if(_ivice[i]->s1()->getVidljiva() && _ivice[i]->s2()->getVidljiva())
            _ivice[i]->setObrisati(true);
        else if(_ivice[i]->s1()->getVidljiva())
            _ivice[i]->zameniVidljivuStranicu(napraviPrvuStranicu(_ivice[i], t), 0);
        else if(_ivice[i]->s2()->getVidljiva())
            _ivice[i]->zameniVidljivuStranicu(napraviDruguStranicu(_ivice[i], t), 1);

    }


}

void KonveksniOmotac3D::ObrisiVisak()
{
    int i = 0;
    while(i < _stranice.size()){
        if(_stranice[i]->getVidljiva()){
            _stranice.removeAt(i);
        } else {
            i++;
        }
    }

    i = 0;
    while(i < _ivice.size()){
        if(_ivice[i]->obrisati()){
            _ivice.removeAt(i);
        } else {
            i++;
        }
    }

}

/*--------------------------------------------------------------------------------------------------*/
/*---------------------------------Pomocni metodi---------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
double KonveksniOmotac3D::zapremina6(Stranica *s, Teme *t)
{
    return pomocneFunkcije::zapremina(s->t1()->koordinate(),
                s->t2()->koordinate(), s->t3()->koordinate(),
                t->koordinate());
}

bool KonveksniOmotac3D::kolinearne(Teme *a, Teme *b, Teme *c)
{
    return pomocneFunkcije::kolinearne3D(a->koordinate(), b->koordinate(), c->koordinate());
}

Stranica* KonveksniOmotac3D::napraviPrvuStranicu(Ivica *iv, Teme *t){
    Ivica* i1=nullptr;
    Ivica* i2=nullptr;
    bool iv1=false, iv2 =false;
    for(auto i=0; i<_ivice.size(); i++){
        if((_ivice[i]->t1() == iv->t1() && _ivice[i]->t2() == t)
                || (_ivice[i]->t1() == t && _ivice[i]->t2() == iv->t1())){
            iv1 = true;
            i1  = _ivice[i];
        }
        if((_ivice[i]->t1() == iv->t2() && _ivice[i]->t2() == t)
                || (_ivice[i]->t1() == t && _ivice[i]->t2() == iv->t2())){
            iv2 = true;
            i2  = _ivice[i];
        }

    }
    if(!iv1){
        i1 = new Ivica(t, iv->t1());
        _ivice.push_back(i1);
    }
    if(!iv2){
        i2 = new Ivica(iv->t2(), t);
        _ivice.push_back(i2);
    }

    Stranica *s = new Stranica(iv->t1(),iv->t2(), t);
    _stranice.push_back(s);

    i1->postavi_stranicu(s);
    i2->postavi_stranicu(s);

    return s;
}

Stranica* KonveksniOmotac3D::napraviDruguStranicu(Ivica *iv, Teme *t){
    Ivica* i1=nullptr;
    Ivica* i2=nullptr;
    bool iv1=false, iv2 =false;
    for(auto i=0; i<_ivice.size(); i++){
        if((_ivice[i]->t1() == iv->t1() && _ivice[i]->t2() == t)
                || (_ivice[i]->t1() == t && _ivice[i]->t2() == iv->t1())){
            iv1 = true;
            i1  = _ivice[i];
        }
        if((_ivice[i]->t1() == iv->t2() && _ivice[i]->t2() == t)
                || (_ivice[i]->t1() == t && _ivice[i]->t2() == iv->t2())){
            iv2 = true;
            i2  = _ivice[i];
        }

    }
    if(!iv1){
        i1 = new Ivica(iv->t1(),t);
        _ivice.push_back(i1);
    }
    if(!iv2){
        i2 = new Ivica(t,iv->t2());
        _ivice.push_back(i2);
    }

    Stranica *s = new Stranica(iv->t2(),iv->t1(), t);
    _stranice.push_back(s);

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
