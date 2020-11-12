#include "ga03_konveksniomotac.h"
#include "pomocnefunkcije.h"
#include <algorithm>

konveksniomotac::konveksniomotac(QWidget *pCrtanje,
                                 int pauzaKoraka,
                                 std::string imeDatoteke,
                                 int broj_tacaka)
    :AlgoritamBaza(pCrtanje, pauzaKoraka)
{
    if (imeDatoteke == "")
        _tacke = generisiNasumicneTacke(broj_tacaka);
    else
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
}

void konveksniomotac::pokreniAlgoritam() {
    maks_tacka = _tacke[0];

    for (auto i = 1ul; i < _tacke.size(); i++) {
        if (_tacke[i].x() > maks_tacka.x() || (_tacke[i].x() == maks_tacka.x() && _tacke[i].y() < maks_tacka.y()))
            maks_tacka = _tacke[i];
    }
    AlgoritamBaza_updateCanvasAndBlock();
    std::sort(_tacke.begin(), _tacke.end(), [&](const auto& lhs, const auto& rhs) {
        double P = pomocneFunkcije::povrsinaTrougla(maks_tacka, lhs, rhs);
        return  (P < 0) ||  (std::abs(P) == 0 && pomocneFunkcije::distanceKvadrat(maks_tacka, lhs) < pomocneFunkcije::distanceKvadrat(maks_tacka, rhs));
    });

    konveksni_omotac.push_back(maks_tacka);
    konveksni_omotac.push_back(_tacke[1]);
    int pom = 2;
    unsigned j = 2;

    while(j < _tacke.size()) {
        if(pomocneFunkcije::povrsinaTrougla(konveksni_omotac[pom-2],
                           konveksni_omotac[pom-1],
                           _tacke[j]) < 0)
        {
            konveksni_omotac.push_back(_tacke[j]);
            ++pom;
            ++j;
        }
        else {
            konveksni_omotac.pop_back();
            --pom;
                       //Ne smemo da povecamo j u ovom slucaju, jer nismo zavrsili sa ovom tackom
        }
        AlgoritamBaza_updateCanvasAndBlock();
    }

    konveksni_omotac.push_back(maks_tacka);
    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

void konveksniomotac::crtajAlgoritam(QPainter *painter) const {

    if (!painter) return;

    QPen pen = painter->pen();
    pen.setColor(Qt::red);
    painter->setPen(pen);
    for(auto tacka: _tacke) {
        painter->drawPoint(tacka);
    }

    pen.setColor(Qt::blue);
    painter->setPen(pen);
    for(auto i = 1ul; i < konveksni_omotac.size(); i++) {
       painter->drawLine(konveksni_omotac.at(i-1), konveksni_omotac.at(i));
    }


}

void konveksniomotac::pokreniNaivniAlgoritam() {
    for (auto i = 0ul;i < _tacke.size(); i++) {
        for (auto j = 0ul; j < _tacke.size(); j++) {
            if (i == j){
                continue;
            }
            bool a = true;
            for (auto k = 0ul; k < _tacke.size(); k++){
                if (k == i || k == j){
                    continue;
                }
                if (pomocneFunkcije::povrsinaTrougla(_tacke.at(i), _tacke.at(j), _tacke.at(k)) > 0){
                    a = false;
                    break;
                }
            }
            if (a){
                if(std::find(naivni_konveksni_omotac.begin(), naivni_konveksni_omotac.end(), _tacke[i]) == naivni_konveksni_omotac.end()){
                   naivni_konveksni_omotac.push_back(_tacke[i]);
                }
                if(std::find(naivni_konveksni_omotac.begin(), naivni_konveksni_omotac.end(), _tacke[j]) == naivni_konveksni_omotac.end()){
                   naivni_konveksni_omotac.push_back(_tacke[j]);
                }
            }
        }

    }

    std::sort(naivni_konveksni_omotac.begin(), naivni_konveksni_omotac.end(), [&](const auto& lhs, const auto& rhs) {
        double P = pomocneFunkcije::povrsinaTrougla(maks_tacka, lhs, rhs);
        return  (P < 0) ||  (std::abs(P) == 0 && pomocneFunkcije::distanceKvadrat(maks_tacka, lhs) < pomocneFunkcije::distanceKvadrat(maks_tacka, rhs));
    });
}
