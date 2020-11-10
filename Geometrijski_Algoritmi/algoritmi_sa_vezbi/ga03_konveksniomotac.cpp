#include "ga03_konveksniomotac.h"
#include "pomocnefunkcije.h"

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
        return  (P < 0) ||  (std::abs(P) < 0.000001 && pomocneFunkcije::distanceKvadrat(maks_tacka, lhs) < pomocneFunkcije::distanceKvadrat(maks_tacka, rhs));
    });

    konveksni_omotac.push_back(maks_tacka);
    konveksni_omotac.push_back(_tacke[1]);
    int pom = 2;

    for(unsigned j = 2; j < _tacke.size(); ++j) {
        if(pomocneFunkcije::povrsinaTrougla(konveksni_omotac[pom-2],
                           konveksni_omotac[pom-1],
                           _tacke[j]) < 0)
        {
            konveksni_omotac.push_back(_tacke[j]);
            ++pom;
        }
        else {
            konveksni_omotac.pop_back();
            --pom;
            --j;    //Ne smemo da povecamo j u ovom slucaju, jer nismo zavrsili sa ovom tackom
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

}
