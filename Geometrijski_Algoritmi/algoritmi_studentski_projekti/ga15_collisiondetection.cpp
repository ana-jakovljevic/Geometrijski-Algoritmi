#include "ga15_collisiondetection.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <QPainterPath>

CollisionDetection::CollisionDetection(QWidget *pCrtanje,
                                       int pauzaKoraka,
                                       const bool &naivni,
                                       std::string imeDatoteke,
                                       int brojTacaka)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni),
      _sweepLineY(0), _eventQueue(),
      _leftPolygonEdgeQueue(edgeComparison(&_sweepLineY, WhichPolygon::LEFT)),
      _rightPolygonEdgeQueue(edgeComparison(&_sweepLineY, WhichPolygon::RIGHT)),
      _collisionVertex(),
      _minDistance(DBL_INFINITY),
      _horizontalLineY(), _collisionVertexNaive(),
      _minDistanceNaive(DBL_INFINITY),
      _edge(), _intersectionPoint(), _vertex()
{
    if (imeDatoteke == "")
        generateRandomPolygons(brojTacaka);
    else
        loadPolygonsFromFile(imeDatoteke);
}


void CollisionDetection::pokreniAlgoritam()
{
    AlgoritamBaza_updateCanvasAndBlock();

    fillEventQueue(WhichPolygon::LEFT);
    fillEventQueue(WhichPolygon::RIGHT);


    while (!_eventQueue.empty())
    {
        auto it = _eventQueue.begin();

        _vertex = *it->vertex;
        _sweepLineY = it->vertex->y();

        std::set<QLine*, edgeComparison> &edgeQueueToUpdate = it->whichPolygon == WhichPolygon::LEFT ?
                                                              _leftPolygonEdgeQueue : _rightPolygonEdgeQueue;
        std::set<QLine*, edgeComparison> &edgeQueueToFindCollision = it->whichPolygon == WhichPolygon::LEFT ?
                                                              _rightPolygonEdgeQueue : _leftPolygonEdgeQueue;

        double dist = DBL_INFINITY;

        switch(it->vertexType) {
            case EventType::TOP_VERTEX:
                edgeQueueToUpdate.emplace(it->edge1);
                edgeQueueToUpdate.emplace(it->edge2);
                break;
            case EventType::MIDDLE_VERTEX:
                edgeQueueToUpdate.erase(it->edge1);
                edgeQueueToUpdate.emplace(it->edge2);
                delete(it->edge1);
                break;
            case EventType::BOTTOM_VERTEX:
                edgeQueueToUpdate.erase(it->edge1);
                edgeQueueToUpdate.erase(it->edge2);
                delete(it->edge1);
                delete(it->edge2);
                break;
        }

        if (edgeQueueToFindCollision.begin() != edgeQueueToFindCollision.end())
        {
            _edge = **edgeQueueToFindCollision.begin();
            dist = horizontalDistance(*it->vertex, **edgeQueueToFindCollision.begin());
        }

        AlgoritamBaza_updateCanvasAndBlock();

        if(dist < _minDistance)
        {
            _minDistance = dist;
            _collisionVertex = it->vertex;
        }

        _eventQueue.erase(it);
    }

    _sweepLineY = 0;
    setEdgeToNull();
    setIntersectionPointToNull();
    setVertexToNull();

    _minDistance = sqrt(_minDistance);
    if(_pCrtanje)
        shiftLeftPolygon(_minDistance);

    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

void CollisionDetection::crtajAlgoritam(QPainter *painter) const
{
    if(!painter) return;

    QPen pen = painter->pen();

    drawPolygons(painter);

    if (_sweepLineY != 0)
    {
        QLineF sweepLine(0, _sweepLineY, _pCrtanje->width(), _sweepLineY);
        drawLine(painter, sweepLine, Qt::green, 2);
    }
    if(!_edge.isNull())
        drawLine(painter, _edge, Qt::yellow, 4);
    if (_collisionVertex)
        drawPoint(painter, *_collisionVertex, Qt::red, 15);
    if(!_intersectionPoint.isNull())
        drawPoint(painter, _intersectionPoint, Qt::blue, 10);
    if(!_vertex.isNull())
        drawPoint(painter, _vertex, Qt::darkGreen, 10);
}

void CollisionDetection::pokreniNaivniAlgoritam()
{
    AlgoritamBaza_updateCanvasAndBlock();

    findCollisionVertexInPolygonNaive(WhichPolygon::LEFT);
    findCollisionVertexInPolygonNaive(WhichPolygon::RIGHT);

    _minDistanceNaive = sqrt(_minDistanceNaive);
    if(_pCrtanje)
        shiftLeftPolygon(_minDistanceNaive);

    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

void CollisionDetection::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    drawPolygons(painter);

    if(_horizontalLineY != 0)
    {
        QLineF horizontalLine(0, _horizontalLineY, _pCrtanje->width(), _horizontalLineY);
        drawLine(painter, horizontalLine, Qt::green, 2);
    }
    if(!_edge.isNull())
        drawLine(painter, _edge, Qt::yellow, 4);
    if(_collisionVertexNaive)
        drawPoint(painter, *_collisionVertexNaive, Qt::red, 15);
    if(!_intersectionPoint.isNull())
        drawPoint(painter, _intersectionPoint, Qt::blue, 10);
    if(!_vertex.isNull())
        drawPoint(painter, _vertex, Qt::darkGreen, 10);
}

void CollisionDetection::fillEventQueue(WhichPolygon whichPolygon)
{
    QPolygon &polygon = whichPolygon == WhichPolygon::LEFT ? _leftPolygon : _rightPolygon;

    int n = polygon.size();
    auto *prevVertex = &polygon[n-1];
    QLine *prevEdge = new QLine(polygon[0], *prevVertex);
    auto firstEdge = prevEdge;

    for (int i = 0; i < n; i++)
    {
        QPoint* vertex = &polygon[i];

        QPoint *nextVertex = &polygon[(i+1)%n];
        QLine *nextEdge = i == n-1 ? firstEdge : new QLine(*vertex, *nextVertex);
        if(pomocneFunkcije::ispod(*prevVertex, *vertex) &&
           pomocneFunkcije::ispod(*nextVertex, *vertex))
        {
            _eventQueue.emplace(vertex, EventType::TOP_VERTEX,
                                whichPolygon, prevEdge, nextEdge);
        }
        else if (pomocneFunkcije::ispod(*vertex, *prevVertex) &&
                 pomocneFunkcije::ispod(*vertex, *nextVertex))
        {
            _eventQueue.emplace(vertex, EventType::BOTTOM_VERTEX,
                                whichPolygon, prevEdge, nextEdge);
        }
        else if (pomocneFunkcije::ispod(*vertex, *prevVertex) &&
                pomocneFunkcije::ispod(*nextVertex, *vertex))
        {
            _eventQueue.emplace(vertex, EventType::MIDDLE_VERTEX,
                                whichPolygon, prevEdge, nextEdge);
        }
        else /*if (pomocneFunkcije::ispod(prevVertex, vertex) &&
                pomocneFunkcije::ispod(vertex, nextVertex))*/
        {
            _eventQueue.emplace(vertex, EventType::MIDDLE_VERTEX,
                                whichPolygon, nextEdge, prevEdge);

        }
        prevEdge = nextEdge;
        prevVertex = vertex;
    }

}

void CollisionDetection::findCollisionVertexInPolygonNaive(WhichPolygon whichPolygon)
{
    QPolygon &polygon1 = whichPolygon == WhichPolygon::LEFT ? _leftPolygon : _rightPolygon;
    QPolygon &polygon2 = whichPolygon == WhichPolygon::LEFT ? _rightPolygon : _leftPolygon;

    int n = polygon2.size();
    for(QPoint &vertex : polygon1)
    {
        _vertex = vertex;
        _horizontalLineY = vertex.y();
        for (int i = 0; i < n; i++)
        {
            _edge.setP1(polygon2[i]);
            _edge.setP2(polygon2[(i+1)%n]);

            double dist = horizontalDistance(vertex, _edge);

            if (dist < _minDistanceNaive)
            {
                _minDistanceNaive = dist;
                _collisionVertexNaive = &vertex;
            }

            AlgoritamBaza_updateCanvasAndBlock();
        }
    }

    setIntersectionPointToNull();
    setEdgeToNull();
    setHorizontalLineY(0);
    setVertexToNull();
}

double CollisionDetection::horizontalDistance(const QPointF &point, const QLineF &edge)
{
    double inf = _pCrtanje ? _pCrtanje->width() : CANVAS_WIDTH;
    QLineF horizontalLine(0, point.y(), inf, point.y());

    double dist;
    if (pomocneFunkcije::paralelneDuzi(edge, horizontalLine) &&
        pomocneFunkcije::bliski(edge.y1(), horizontalLine.y1()))
    {
        double d1 = pomocneFunkcije::distanceKvadratF(point, edge.p1());
        double d2 = pomocneFunkcije::distanceKvadratF(point, edge.p2());
        if (d1 < d2)
        {
            dist = d1;
            _intersectionPoint = edge.p1();
        }
        else
        {
            dist = d2;
            _intersectionPoint = edge.p2();
        }
    }
    if (pomocneFunkcije::presekDuzi(edge, horizontalLine, _intersectionPoint))
        dist = pomocneFunkcije::distanceKvadratF(point, _intersectionPoint);
    else
        dist = DBL_INFINITY;

    return dist;
}

void CollisionDetection::shiftLeftPolygon(double distanceToShift)
{
    if (distanceToShift == DBL_INFINITY)
        distanceToShift = _pCrtanje ? _pCrtanje->width() : CANVAS_WIDTH;

    double step = 30;
    bool stop = false;
    for (double shifted = step; !stop; shifted += step)
    {
        if (shifted > distanceToShift)
        {
            step -= (shifted-distanceToShift);
            stop = true;
        }
        _leftPolygon.translate(step, 0);
        AlgoritamBaza_updateCanvasAndBlock();
    }
}

void CollisionDetection::generateRandomPolygons(int numberOfPoints)
{
    std::vector<QPoint> points = AlgoritamBaza::generisiNasumicneTacke(numberOfPoints);
    std::vector<QPoint> leftPoints;
    std::vector<QPoint> rightPoints;

    for (int i = 0; i < numberOfPoints; i++)
    {
        int width = _pCrtanje ? _pCrtanje->width() : CANVAS_WIDTH;

        if (points[i].x() < width/2)
            leftPoints.emplace_back(points[i]);
        else
            rightPoints.emplace_back(points[i]);
    }

    pomocneFunkcije::sortirajTackeZaProstPoligon(leftPoints);
    pomocneFunkcije::sortirajTackeZaProstPoligon(rightPoints);

    for (QPoint &point : leftPoints)
        _leftPolygon.append(point);

    for (QPoint &point : rightPoints)
        _rightPolygon.append(point);
}

void CollisionDetection::loadPolygonsFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName);
    std::string line1, line2;
    std::getline(inputFile, line1);
    std::getline(inputFile, line2);

    _leftPolygon = parsePolygonFromString(line1);
    _rightPolygon = parsePolygonFromString(line2);
}

QPolygon CollisionDetection::parsePolygonFromString(std::string line)
{
    std::stringstream ss;
    ss << line;
    QPolygon polygon;
    int x, y;
    while(!ss.eof())
    {
        ss >> x >> y;
        polygon.append(QPoint(x, y));
    }
    return polygon;
}

double CollisionDetection::getMinDistance() const
{
    return _minDistance;
}

double CollisionDetection::getMinDistanceNaive() const
{
   return _minDistanceNaive;
}

void CollisionDetection::setIntersectionPointToNull()
{
    _intersectionPoint.setX(0);
    _intersectionPoint.setY(0);
}

void CollisionDetection::setEdgeToNull()
{
    _edge.setLine(0, 0, 0, 0);
}

void CollisionDetection::setVertexToNull()
{
    _vertex.setX(0);
    _vertex.setY(0);
}

void CollisionDetection::setHorizontalLineY(double y)
{
    _horizontalLineY = y;
}

void CollisionDetection::drawPolygons(QPainter *painter) const
{
    if(!painter) return;

    QPen pen = painter->pen();
    QBrush brush = painter->brush();

    // draw polygons
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter->setPen(pen);
    painter->drawPolygon(_leftPolygon);
    painter->drawPolygon(_rightPolygon);

    // fill polygons
    brush.setColor(Qt::gray);
    brush.setStyle(Qt::Dense3Pattern);
    QPainterPath path;
    path.addPolygon(_leftPolygon);
    path.addPolygon(_rightPolygon);
    painter->fillPath(path, brush);
}

void CollisionDetection::drawPoint(QPainter *painter, const QPointF &point,
                                   const QColor &color, const int width) const
{
    if(!painter) return;

    QPen pen = painter->pen();
    pen.setColor(color);
    pen.setWidth(width);
    painter->setPen(pen);
    painter->drawPoint(point);
}

void CollisionDetection::drawLine(QPainter *painter, const QLineF &line,
                                 const QColor &color, const int width) const
{
    if(!painter) return;

    QPen pen = painter->pen();
    pen.setColor(color);
    pen.setWidth(width);
    painter->setPen(pen);
    painter->drawLine(line);
}
