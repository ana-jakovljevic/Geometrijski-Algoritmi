#include "ga11_kernel.h"


Kernel::Kernel(QWidget *pCrtanje, int pauzaKoraka, const bool &naivni, const std::string &imeDatoteke, unsigned brojNasumicnihTacaka)
    :AlgoritamBaza(pCrtanje, pauzaKoraka, naivni),
      mCvorovi(imeDatoteke.empty() ?
                   generisiNasumicneTacke(brojNasumicnihTacaka) :
                   ucitajPodatkeIzDatoteke(imeDatoteke)) {
        pomocneFunkcije::sortirajTackeZaProstPoligon(mCvorovi);

    }

void Kernel::pokreniAlgoritam()
{
    while(!mCvorovi.empty()) {

        AlgoritamBaza_updateCanvasAndBlock()
        mCvorovi.pop_back();
    }
    emit animacijaZavrsila();
}

void Kernel::crtajAlgoritam(QPainter *painter) const
{
    painter->drawPolygon(mCvorovi.data(), mCvorovi.size()-2);
}

void Kernel::pokreniNaivniAlgoritam()
{

}

void Kernel::crtajNaivniAlgoritam(QPainter *painter) const
{

}
