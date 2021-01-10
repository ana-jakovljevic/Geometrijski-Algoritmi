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
    /// returns the distance left polygon has to travel before the collision.
    /// obtained by efficient algorithm.
    double getDistance() const;
    /// returns the distance left polygon has to travel before the collision.
    /// obtained by naive algorithm.
    double getDistanceNaive() const;
    /// returns an arbitrary collision point.
    /// obtained by efficient algorithm.
    QPointF getCollisionPoint() const;
    /// returns an arbitrary collision point.
    /// obtained by naive algorithm.
    QPointF getCollisionPointNaive() const;

private:
    QPolygon _leftPolygon;
    QPolygon _rightPolygon;

    // for efficient implementation
    double _sweepLineY;
    /// set of events, events are vertices sorted by y coordinate
    std::set<eventPoint, eventComparison> _eventQueue;
    /// status - set of edges from left polygon currently intersected by the sweepline
    /// edges are sorted ascending by x coordinate
    std::set<QLine*, edgeComparison> _leftPolygonEdgeQueue;
    /// status - set of edges from right polygon currently intersected by the sweepline
    /// edges are sorted descending by x coordinate
    std::set<QLine*, edgeComparison> _rightPolygonEdgeQueue;

    /// collision vertex
    /// obtained by efficient algorithm
    QPoint *_collisionVertex;
    /// minimum of all distances beetwen vertices of one polygon and edges of opposite polygon
    /// at the end of the algorithm that will be the distance left polygon has to travel before the collision
    double _minDistance;

    /// fills event queue with all vertices
    void fillEventQueue(WhichPolygon which);

    // for naive implementation
    double _horizontalLineY;

    /// collision vertex
    /// obtained by naive algorithm
    QPoint *_collisionVertexNaive;
    /// minimum of all distances beetwen vertices of one polygon and edges of opposite polygon
    /// at the end of the algorithm that will be the distance that left polygon has to travel before the collision
    double _minDistanceNaive;

    /// finds candidate for collision vertex in one polygon
    /// we will apply the function to both left and right polygon
    void findCollisionVertexInPolygonNaive(WhichPolygon whichPolygon);

    /// horizontal distance beetwen point and edge
    double horizontalDistance(const QPointF &point, const QLineF &edge);


    // methods for parsing input
    void generateRandomPolygons(int numberOfPoints);
    void loadPolygonsFromFile(std::string fileName);
    QPolygon parsePolygonFromString(std::string line);

    // fields and methods for visualization
    QLine _edge;
    QPointF _intersectionPoint;
    QPoint _vertex;

    void setEdgeToNull();
    void setIntersectionPointToNull();
    void setVertexToNull();

    /// shifting left polygon to illustrate the collision
    void shiftLeftPolygon(double distance);

    void drawPolygons(QPainter *painter) const;
    void drawPoint(QPainter *painter, const QPointF &point,
                   const QColor &color, int width) const;
    void drawLine(QPainter *painter, const QLineF &line,
                  const QColor &color, int width) const;
};

#endif // COLLISIONDETECTION_H
