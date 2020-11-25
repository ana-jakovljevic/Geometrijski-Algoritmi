#include "ga05_preseciduzi.h"

#include <fstream>
#include <math.h>

PreseciDuzi::PreseciDuzi(QWidget *pCrtanje,
                          int pauzaKoraka,
                          std::string imeDatoteke,
                          int broj_tacaka)
   :AlgoritamBaza(pCrtanje, pauzaKoraka)
{
    if (imeDatoteke != "")
        _duzi = ucitajPodatkeIzDatoteke(imeDatoteke);
    else
        _duzi = generisiNasumicneDuzi(broj_tacaka);
}

void PreseciDuzi::pokreniAlgoritam() {}

void PreseciDuzi::crtajAlgoritam(QPainter *painter) const {
    if (!painter)
        return;
}

void PreseciDuzi::pokreniNaivniAlgoritam() {}

std::vector<QLineF> PreseciDuzi::generisiNasumicneDuzi(int brojDuzi) const {

    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<QLineF> randomDuzi;

    std::vector<QPoint> tacke_za_duzi = generisiNasumicneTacke(2*brojDuzi);

    for(auto i=0ul; i < tacke_za_duzi.size()-1; i+=2){
        auto x1 = tacke_za_duzi[i].x();
        auto x2 = tacke_za_duzi[i+1].x();
        auto y1 = tacke_za_duzi[i].y();
        auto y2 = tacke_za_duzi[i+1].y();

        if (y1 < y2 || (fabs(y1-y2) < 0.000001 && x2 < x1)) {
            auto tmp_y = y1;
            auto tmp_x = x1;
            y1 = y2;
            x1 = x2;
            y2 = tmp_y;
            x2 = tmp_x;
        }
        randomDuzi.emplace_back(x1, y1, x2, y2);
    }

    return randomDuzi;
}

std::vector<QLineF> PreseciDuzi::ucitajPodatkeIzDatoteke(std::string imeDatoteke) const {

    std::ifstream inputFile(imeDatoteke);
    std::vector<QLineF> duzi;

    double x1, y1, x2, y2;

    while(inputFile >> x1 >> y1 >> x2 >> y2)
    {
        if (y1 < y2 || (fabs(y1-y2) < 0.000001 && x2 < x1)) {
            auto tmp_y = y1;
            auto tmp_x = x1;
            y1 = y2;
            x1 = x2;
            y2 = tmp_y;
            x2 = tmp_x;
        }

        duzi.emplace_back(x1, y1, x2, y2);
    }

    return duzi;
}
