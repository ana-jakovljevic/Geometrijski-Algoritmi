#ifndef GA15_DATASTRUCTURES_H
#define GA15_DATASTRUCTURES_H

#include <QPointF>
#include <QLineF>
#include <QPolygon>
#include <algorithm>

#include "pomocnefunkcije.h"

enum class WhichPolygon {
    LEFT, // left polygon
    RIGHT // right polygon
};

enum class EventType {
    TOP_VERTEX, // both edges incident to the vertex are bellow the vertex
    BOTTOM_VERTEX, // both edges are above
    MIDDLE_VERTEX // one edge bellow, one edge above
};

// Events are vertices with supplementary info
struct eventPoint {
    eventPoint(QPoint *v,
               const EventType &type,
               const WhichPolygon &which,
               QLine *e1,
               QLine *e2)
        : vertex(v), vertexType(type),
          whichPolygon(which),
          edge1(e1), edge2(e2) {}

    QPoint *vertex;
    const EventType vertexType;
    // is vertex in left or right polygon
    const WhichPolygon whichPolygon;
    // edges incident to the vertex
    QLine *edge1;
    QLine *edge2;

};

// comparator for eventPoints
// compares vertices by its y coordinate
struct eventComparison {
    bool operator()(const eventPoint &a, const eventPoint &b) const {

        return (a.vertex->y() > b.vertex->y()) ||
               (a.vertex->y() == b.vertex->y() && a.vertex->x() < b.vertex->x());
    }
};

// comparator for pointers to edges
// compares x coordinate using the intersection with the sweepline
// edges from left polygon should be sorted in ascending order and from the right in descending order
struct edgeComparison {
    const double *const sweepLineY;
    WhichPolygon whichPolygon; // are edges from left or right polygon

    edgeComparison(double *y, WhichPolygon which)
        : sweepLineY(y), whichPolygon(which)
    {}

    bool operator()(const QLine *edge1, const QLine *edge2) const {

        // we have to translate the line by some small value eps
        // for the comparison to work well with the edges that are incident
        // to the TOP vertex
        // eps must be wisely choosen, it must not be arbitrary value
        double minY1 = std::min(edge1->y1(), edge1->y2());
        double minY2 = std::min(edge2->y1(), edge2->y2());
        double eps = std::min(*sweepLineY-minY1, *sweepLineY-minY2) / 2;
        QLineF sweepLine( 0, *sweepLineY-eps, 1, *sweepLineY-eps);

        QPointF intersection1;
        QPointF intersection2;
        pomocneFunkcije::presekDuzi(*edge1, sweepLine, intersection1);
        pomocneFunkcije::presekDuzi(*edge2, sweepLine, intersection2);

        // if the lines are collinear the intersection point is undefined
        // so we have to set it manually
        // we can, for example, take the center of the edge
        // (or any other point on the edge except vertices)
        if (pomocneFunkcije::paralelneDuzi(*edge1, sweepLine) &&
            pomocneFunkcije::bliski(edge1->y1(), sweepLine.y1()))
                intersection1 = edge1->center();
        if (pomocneFunkcije::paralelneDuzi(*edge2, sweepLine) &&
            pomocneFunkcije::bliski(edge2->y1(), sweepLine.y1()))
                intersection1 = edge2->center();

        if (whichPolygon == WhichPolygon::RIGHT)
            return intersection1.x() < intersection2.x();
        /*else if (whichPolygon == WhichPolygon::LEFT)*/
        return intersection1.x() > intersection2.x();
    }
};

#endif // GA15_DATASTRUCTURES_H
