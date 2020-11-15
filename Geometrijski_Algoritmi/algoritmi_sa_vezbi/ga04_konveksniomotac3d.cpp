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

}

bool KonveksniOmotac3D::Tetraedar()
{
    return true;
}

void KonveksniOmotac3D::DodajTeme(Teme* t)
{

}

void KonveksniOmotac3D::ObrisiVisak()
{

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
