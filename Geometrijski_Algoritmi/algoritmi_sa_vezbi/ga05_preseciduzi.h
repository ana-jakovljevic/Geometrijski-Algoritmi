#ifndef GA05_PRESECIDUZI_H
#define GA05_PRESECIDUZI_H

#include "algoritambaza.h"

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
    std::vector<QLineF> duzi;
    double y_brisuce_prave;
    std::vector<QPointF> preseci;

    // imacemo skup Event Points
    // imacemo skup posmatranih duzi
};

#endif // GA05_PRESECIDUZI_H
