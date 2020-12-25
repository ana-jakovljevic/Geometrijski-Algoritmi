#ifndef GA10_UNITDISKCOVER_H
#define GA10_UNITDISKCOVER_H

#include "algoritambaza.h"
#include "pomocnefunkcije.h"
#include <iostream>
#include <complex>
#include <set>
#include <unordered_set>
#include "ga10_datastructures.h"

class UnitDiskCover : public AlgoritamBaza
{
public:
    UnitDiskCover(QWidget *pCrtanje,
                  int pauzaKoraka,
                  const bool &naivni = false,
                  std::string imeDatoteke = "",
                  int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

private:
    double rastojanje(const QPoint& p, const std::vector<QPoint>& _tacke);
    std::vector<QPoint> _tacke; //P
    std::vector<QPoint> _naivni_pokrivac; //C
    std::vector<QPoint> _nezavisanSkup; //I

    std::vector<QPoint> _pokrivac;

    std::set<EventPoint*,EventQueueComp> _eventQueue;
    std::set<EventPoint*, StatusQueueComp> _statusQueue;
    void initialiseEventQueue();
    void createCover();
};

#endif // GA10_UNITDISKCOVER_H
