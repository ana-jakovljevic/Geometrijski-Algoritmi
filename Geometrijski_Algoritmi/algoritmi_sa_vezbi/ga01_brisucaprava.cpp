#include "ga01_brisucaprava.h"

BrisucaPrava::BrisucaPrava(QWidget* pCrtanje, QOpenGLWidget* pCrtanjeGL,
                           int pauzaKoraka,
                           std::string imeDatoteke,
                           int broj_tacaka)
    :AlgoritamBaza(pCrtanje, pCrtanjeGL, pauzaKoraka), _yPoz(0)
{
    if (imeDatoteke == "")
      _tacke = generisiNasumicneTacke(broj_tacaka);
    else
      _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
}

void BrisucaPrava::pokreniAlgoritam()
{
    while (_yPoz < _pCrtanje->height())
    {
        _yPoz += 5;
        AlgoritamBaza_updateCanvasAndBlock();
    }

    emit animacijaZavrsila();
}

void BrisucaPrava::crtajAlgoritam(QPainter &painter) const
{
    QPen p = painter.pen();
    p.setColor(Qt::magenta);
    p.setWidth(2);
    p.setCapStyle(Qt::RoundCap);

    painter.setPen(p);
    painter.drawLine(0, _yPoz, _pCrtanje->width(), _yPoz);

    p.setWidth(5);
    for(const QPoint pt : _tacke)
    {
        if (pt.y() < _yPoz)
            p.setColor(Qt::red);
        else
            p.setColor(Qt::black);

        painter.setPen(p);
        painter.drawPoint(pt);
    }

}

void BrisucaPrava::crtajAlgoritam3D() const
{

}

void BrisucaPrava::pokreniNaivniAlgoritam()
{

}

bool BrisucaPrava::is_3D() const
{
    return false;
}
