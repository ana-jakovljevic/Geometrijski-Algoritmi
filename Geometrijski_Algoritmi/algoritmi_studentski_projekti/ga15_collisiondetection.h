#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <set>

#include "algoritambaza.h"
#include "pomocnefunkcije.h"

#include "ga15_datastructures.h"

class CollisionDetection : public AlgoritamBaza
{
public:
    CollisionDetection(QWidget *pCrtanje,
                       int pauzaKoraka,
                       const bool &naivni = false,
                       std::string imeDatoteke = "",
                       int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    // base class methods
    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    // getters and setters
    double getMinDistance() const;
    double getMinDistanceNaive() const;
    void setIntersectionPointToNull();
    void setEdgeToNull();
    void setVertexToNull();
    void setHorizontalLineY(double y);

private:
    QPolygon _leftPolygon;
    QPolygon _rightPolygon;

    // for efficient implementation
    double _sweepLineY;
    std::set<eventPoint, eventComparison> _eventQueue;
    std::set<QLineF*, edgeComparison> _leftPolygonEdgeQueue;
    std::set<QLineF*, edgeComparison> _rightPolygonEdgeQueue;

    QPoint *_collisionVertex;
    double _minDistance;

    void fillEventQueue(WhichPolygon which);

    // for naive implementation
    double _horizontalLineY;

    QPoint *_collisionVertexNaive;
    double _minDistanceNaive;

    void findCollisionVertexInPolygonNaive(WhichPolygon whichPolygon);

    double horizontalDistance(const QPoint &point, const QLineF &edge);
    void shiftLeftPolygon(double distance);

    // methods for parsing input
    void generateRandomPolygons(int numberOfPoints);
    void loadPolygonsFromFile(std::string fileName);
    QPolygon parsePolygonFromString(std::string line);

    // fields and methods for illustrations
    QLineF _edge;
    QPointF _intersectionPoint;
    QPoint _vertex;

    void drawPolygons(QPainter *painter) const;
    void drawPoint(QPainter *painter, const QPointF &point,
                   const QColor &color, int width) const;
    void drawLine(QPainter *painter, const QLineF &line,
                  const QColor &color, int width) const;
};

#endif // COLLISIONDETECTION_H
