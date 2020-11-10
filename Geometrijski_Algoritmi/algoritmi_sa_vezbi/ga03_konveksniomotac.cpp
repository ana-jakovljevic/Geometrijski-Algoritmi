#include "ga03_konveksniomotac.h"

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
}
void konveksniomotac::crtajAlgoritam(QPainter *painter) const {

}

void konveksniomotac::pokreniNaivniAlgoritam() {

}
