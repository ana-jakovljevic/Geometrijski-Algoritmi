#include "ga02_3discrtavanje.h"

#include <QVector3D>

Discrtavanje::Discrtavanje(QWidget *pCrtanje,
                           int pauzaKoraka,
                           std::string,
                           int broj_tacaka)
    :AlgoritamBaza(pCrtanje, pauzaKoraka), _n(0)
{
    _tacke = generisiNasumicneTacke(broj_tacaka);
}

void Discrtavanje::pokreniAlgoritam()
{
    while (_n != _tacke.size())
    {
        _n++;
        AlgoritamBaza_updateCanvasAndBlock()
    }

    emit animacijaZavrsila();
}

void Discrtavanje::crtajAlgoritam(QPainter *) const
{
    glBegin(GL_TRIANGLE_STRIP);
        for(auto i = 0u; i < _n; i++)
        {
            double drand = 3.*rand()/RAND_MAX;
            if (drand < 1)
                glColor3d(1, 0, 0);
            else if (drand < 2)
                glColor3d(0, 1, 0);
            else
                glColor3d(0, 0, 1);
            glVertex3f(_tacke[i].x(), _tacke[i].y(), _tacke[i].z());
        }
    glEnd();
}

void Discrtavanje::pokreniNaivniAlgoritam()
{

}

std::vector<QVector3D> Discrtavanje::generisiNasumicneTacke(int broj_tacaka) const
{
    std::vector<QVector3D> tacke;
    srand(static_cast<unsigned>(time(nullptr)));

    for(int i=0; i < broj_tacaka; i++)
        tacke.emplace_back(-100.*rand()/RAND_MAX,
                           -100.*rand()/RAND_MAX,
                           -120.*rand()/RAND_MAX);

    return tacke;
}
