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
                const bool &naivni = false,
                std::string imeDatoteke = "",
                int brojDuzi = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void naglasiTrenutnu(QPainter *painter, unsigned long i) const;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    /* Ucitavanje podataka. */
    std::vector<QLineF> generisiNasumicneDuzi(int brojDuzi) const;
    std::vector<QLineF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;

private:
    std::vector<QLineF> _duzi;
    double y_brisuce_prave;
    std::vector<QPointF> _preseci;

    std::set<tackaDogadjaja, poredjenjeDogadjaja> _redDogadjaja;
    std::set<QLineF*, poredjenjeDuzi> _redDuzi;

    /* Polja za naivni algoritam */
    std::vector<QPointF> _naivniPreseci;
    unsigned long _iNaivno;
    unsigned long _jNaivno;
};

#endif // GA05_PRESECIDUZI_H
