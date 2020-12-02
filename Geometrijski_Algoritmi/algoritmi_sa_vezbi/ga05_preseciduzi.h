#ifndef GA05_PRESECIDUZI_H
#define GA05_PRESECIDUZI_H

#include <set>

#include "algoritambaza.h"
#include "pomocnefunkcije.h"

#include "ga05_datastructures.h"

class PreseciDuzi : public AlgoritamBaza
{
public:
    PreseciDuzi(QWidget *pCrtanje,
                int pauzaKoraka,
                std::string imeDatoteke = "",
                int broj_tacaka = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam();
    void crtajAlgoritam(QPainter *painter) const;
    void pokreniNaivniAlgoritam();

    /* Ucitavanje podataka. */
    std::vector<QLineF> generisiNasumicneDuzi(int brojDuzi) const;
    std::vector<QLineF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;

private:
    std::vector<QLineF> _duzi;
    double y_brisuce_prave;
    std::vector<QPointF> _preseci;

    std::vector<QPointF> _naivniPreseci;
    std::set<tackaDogadjaja, poredjenjeDogadjaja> _redDogadjaja;
    std::set<QLineF*, poredjenjeDuzi> _redDuzi;
};

#endif // GA05_PRESECIDUZI_H
