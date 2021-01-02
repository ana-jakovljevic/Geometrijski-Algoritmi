#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "algoritambaza.h"

class CollisionDetection : public AlgoritamBaza
{
public:
    CollisionDetection(QWidget *pCrtanje,
                       int pauzaKoraka,
                       const bool &naivni = false,
                       std::string imeDatoteke = "",
                       int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);


    void pokreniAlgoritam() = 0;
    void crtajAlgoritam(QPainter *painter) const = 0;
    void pokreniNaivniAlgoritam() = 0;
    void crtajNaivniAlgoritam(QPainter *painter) const = 0;

private:
    std::vector<QPoint> _polygon1;
    std::vector<QPoint> _polygon2;



};

#endif // COLLISIONDETECTION_H
