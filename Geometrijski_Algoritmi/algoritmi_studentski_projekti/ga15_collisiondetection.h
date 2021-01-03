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

    enum class WhichPolygon {
        LEFT,
        RIGHT
    };

    QLineF _horizontalLine;
    QLineF _edge;
    QPointF _intersectionPoint;
    QPoint *_collisionVertex;
    double _minDistance;

    void findCollisionVertexInPolygon(WhichPolygon whichPolygon);
    void shiftLeftPolygonAlongXAxis();

    void generateRandomPolygons(int numberOfPoints);
    void loadPolygonsFromFile(std::string fileName);
    QPolygon parsePolygonFromLine(std::string line);

};

#endif // COLLISIONDETECTION_H
