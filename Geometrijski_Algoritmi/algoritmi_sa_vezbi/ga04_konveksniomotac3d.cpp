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

    for(int i=0u;i<_tacke.size();i++){
        if(!_tacke[i]->getObradjeno()){
            DodajTeme(_tacke[i]);
            ObrisiVisak();
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
