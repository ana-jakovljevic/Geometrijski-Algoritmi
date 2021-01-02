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


    void pokreniAlgoritam() = 0;
    void crtajAlgoritam(QPainter *painter) const = 0;
    void pokreniNaivniAlgoritam() = 0;
    void crtajNaivniAlgoritam(QPainter *painter) const = 0;

private:
    std::vector<QPoint> _polygon1;
    std::vector<QPoint> _polygon2;

    void generateRandomPolygons(int brojTacaka);
    void loadPolygonsFromFile(std::string imeDatoteke);

    std::vector<QPoint> parsePointsFromLine(std::string line);
    void scalePolygonAlongXAxis(std::vector<QPoint> &polygon, double factor = 1);
    void shiftPolygonAlongXAxis(std::vector<QPoint> &polygon, int d = 0);
};

#endif // COLLISIONDETECTION_H
