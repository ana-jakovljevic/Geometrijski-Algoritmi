#include "ga03_konveksniomotac.h"
//#include "pomocnefunkcije.h"
//using namespace pomocneFunkcije;

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

    auto povrsinaTrougla = [](const QPoint& A, const QPoint& B, const QPoint& C)
    {
        // returns P/2
        return (B.x() - A.x())*(C.y() - A.y()) - (C.x() - A.x())*(B.y() - A.y());
    };

    auto distanceKvadrat = [](const QPoint& A, const QPoint& B)
    {
        return (A.x() - B.x())*(A.x() - B.x()) + (A.y() - B.y())*(A.y() - B.y());
    };

    std::sort(_tacke.begin(), _tacke.end(), [&](const auto& lhs, const auto& rhs) {
        double P = povrsinaTrougla(maks_tacka, lhs, rhs);
        return  (P < 0) ||  (std::abs(P) < 0.000001 && distanceKvadrat(maks_tacka, lhs) < distanceKvadrat(maks_tacka, rhs));
    });
}

void konveksniomotac::crtajAlgoritam(QPainter *painter) const {

}

void konveksniomotac::pokreniNaivniAlgoritam() {

}
