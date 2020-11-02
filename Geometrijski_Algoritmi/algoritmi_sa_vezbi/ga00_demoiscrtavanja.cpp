#include "ga00_demoiscrtavanja.h"

DemoIscrtavanja::DemoIscrtavanja(QWidget* pCrtanje, QOpenGLWidget* pCrtanjeGL,
                                 int pauzaKoraka,
                                 std::string imeDatoteke,
                                 int broj_tacaka)
    :AlgoritamBaza(pCrtanje, pCrtanjeGL, pauzaKoraka), _n(0)
{
    if (imeDatoteke == "")
        _tacke = generisiNasumicneTacke(broj_tacaka);
    else
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
}

void DemoIscrtavanja::pokreniAlgoritam()
{
    while (_n != _tacke.size())
    {
        _n++;
        AlgoritamBaza_updateCanvasAndBlock();
    }

    emit animacijaZavrsila();
}

void DemoIscrtavanja::crtajAlgoritam(QPainter &painter) const
{
    QPen p = painter.pen();
    p.setColor(Qt::red);
    p.setWidth(2);

    painter.setPen(p);

    painter.drawPolygon(_tacke.data(), _n);
}

void DemoIscrtavanja::crtajAlgoritam3D() const
{

}

void DemoIscrtavanja::pokreniNaivniAlgoritam()
{

}

bool DemoIscrtavanja::is_3D() const
{
    return false;
}
