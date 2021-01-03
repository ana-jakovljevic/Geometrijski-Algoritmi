#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "algoritambaza.h"
#include "pomocnefunkcije.h"

class CollisionDetection : public AlgoritamBaza
{
public:
    CollisionDetection(QWidget *pCrtanje,
                       int pauzaKoraka,
                       const bool &naivni = false,
                       std::string imeDatoteke = "",
                       int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);


    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

private:
    QPolygon _leftPolygon;
    QPolygon _rightPolygon;

    void generateRandomPolygons(int numberOfPoints);
    void loadPolygonsFromFile(std::string fileName);
    QPolygon parsePolygonFromLine(std::string line);

};

#endif // COLLISIONDETECTION_H
