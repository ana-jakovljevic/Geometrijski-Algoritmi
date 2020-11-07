#include "ga06_presekPravougaonika.h"
#include <fstream>

PresekPravougaonika::PresekPravougaonika(QWidget *pCrtanje,
                                         int pauzaKoraka,
                                         std::string imeDatoteke,
                                         int brojPravougaonika)
    :AlgoritamBaza(pCrtanje, pauzaKoraka)
{
    if (imeDatoteke == "")
      generisiNasumicnePravougaonike(brojPravougaonika);
    else
      ucitajPodatkeIzDatoteke(imeDatoteke);
}

void PresekPravougaonika::pokreniAlgoritam()
{
    emit animacijaZavrsila();
}

void PresekPravougaonika::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    for (auto i = 0ul; i < _n; i++) {
        painter->drawRect(*_pravougaonici[i]);
    }
}

void PresekPravougaonika::pokreniNaivniAlgoritam()
{
    struct timespec ts = { 2, 0 };
    nanosleep(&ts, nullptr);
}

void PresekPravougaonika::generisiNasumicnePravougaonike(int brojPravougaonika)
{
    _n = brojPravougaonika;
    _pravougaonici = new QRect*[_n];

    const auto tacke = generisiNasumicneTacke(2*brojPravougaonika);
    for (auto i = 0ul; i < tacke.size(); i+=2) {
        auto tacka1 = tacke[i];
        auto tacka2 = tacke[i+1];

        if (tacka1.x() > tacka2.x()) {
            std::swap(tacka1.rx(), tacka2.rx());
        }

        if (tacka1.y() < tacka2.y()) {
            std::swap(tacka1.ry(), tacka2.ry());
        }

        _pravougaonici[i/2] = new QRect(tacka1, tacka2);
    }
}

void PresekPravougaonika::ucitajPodatkeIzDatoteke(std::string imeDatoteke)
{
    std::ifstream datoteka(imeDatoteke);
    datoteka >> _n;

    _pravougaonici = new QRect*[_n];
    for (auto i = 0ul; i < _n; i++) {
        int x, y, w, h;
        datoteka >> x >> y >> w >> h;
        _pravougaonici[i] = new QRect(x, y, w, h);
    }
}
