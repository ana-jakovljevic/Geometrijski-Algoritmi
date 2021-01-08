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
      _horizontalLineY(), _edge(), _intersectionPoint(),
      _collisionVertexNaive(),
      _minDistanceNaive(DBL_INFINITY)
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

        AlgoritamBaza_updateCanvasAndBlock();

        std::set<QLineF*, edgeComparison> &edgeQueueToUpdate = it->whichPolygon == WhichPolygon::LEFT ?
                                                              _leftPolygonEdgeQueue : _rightPolygonEdgeQueue;
        std::set<QLineF*, edgeComparison> &edgeQueueToFindCollision = it->whichPolygon == WhichPolygon::LEFT ?
                                                              _rightPolygonEdgeQueue : _leftPolygonEdgeQueue;
        _sweepLineY = it->vertex->y();

        double dist = DBL_INFINITY;

        switch(it->vertexType) {
            case EventType::TOP_VERTEX:
                edgeQueueToUpdate.emplace(it->edge1);
                edgeQueueToUpdate.emplace(it->edge2);
                break;
            case EventType::MIDDLE_VERTEX:
                delete(it->edge1);
                edgeQueueToUpdate.erase(it->edge1);
                edgeQueueToUpdate.emplace(it->edge2);
                break;
            case EventType::BOTTOM_VERTEX:
                delete(it->edge1);
                delete(it->edge2);
                edgeQueueToUpdate.erase(it->edge1);
                edgeQueueToUpdate.erase(it->edge2);
                break;
        }

        if (edgeQueueToFindCollision.begin() != edgeQueueToFindCollision.end())
            dist = horizontalDistance(*it->vertex, **edgeQueueToFindCollision.begin());

        if(dist < _minDistance)
        {
            _minDistance = dist;
            _collisionVertex = it->vertex;
        }

        _eventQueue.erase(it);
    }

//    std::cout << "min distance " << _minDistance << std::endl;
    _sweepLineY = 0;

    if(_pCrtanje)
        shiftLeftPolygon(sqrt(_minDistance));

    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

void CollisionDetection::crtajAlgoritam(QPainter *painter) const
{
    if(!painter) return;

    QPen pen = painter->pen();

    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter->setPen(pen);
    painter->drawPolygon(_leftPolygon);
    painter->drawPolygon(_rightPolygon);

    if (_sweepLineY != 0)
    {
        pen.setColor(Qt::green);
        pen.setWidth(2);
        painter->setPen(pen);

        QLineF sweepLine(0, _sweepLineY, _pCrtanje->width(), _sweepLineY);
        painter->drawLine(sweepLine);
    }

    if (_collisionVertex)
    {
        pen.setColor(Qt::red);
        pen.setWidth(10);
        painter->setPen(pen);
        painter->drawPoint(*_collisionVertex);
    }

}

void CollisionDetection::pokreniNaivniAlgoritam()
{
    AlgoritamBaza_updateCanvasAndBlock();

    findCollisionVertexInPolygonNaive(WhichPolygon::LEFT);
    findCollisionVertexInPolygonNaive(WhichPolygon::RIGHT);

//    std::cout << "min distance naive " << _minDistanceNaive << std::endl;

    if(_pCrtanje)
        shiftLeftPolygon(sqrt(_minDistanceNaive));

    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

void CollisionDetection::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    QPen pen = painter->pen();

    // draw polygons
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter->setPen(pen);
    painter->drawPolygon(_leftPolygon);
    painter->drawPolygon(_rightPolygon);

    if (_collisionVertexNaive)
    {
        pen.setColor(Qt::red);
        pen.setWidth(10);
        painter->setPen(pen);
        painter->drawPoint(*_collisionVertexNaive);
    }

    if (!_intersectionPoint.isNull())
    {
        pen.setColor(Qt::blue);
        pen.setWidth(10);
        painter->setPen(pen);
        painter->drawPoint(_intersectionPoint);
    }

    if(_horizontalLineY != 0)
    {
        pen.setColor(Qt::green);
        pen.setWidth(2);
        painter->setPen(pen);

        QLineF horizontalLine(0, _horizontalLineY, _pCrtanje->width(), _horizontalLineY);
        painter->drawLine(horizontalLine);
    }

    if(!_edge.isNull())
    {
        pen.setColor(Qt::yellow);
        pen.setWidth(4);
        painter->setPen(pen);
        painter->drawLine(_edge);
    }
}

void CollisionDetection::fillEventQueue(WhichPolygon whichPolygon)
{
    QPolygon &polygon = whichPolygon == WhichPolygon::LEFT ? _leftPolygon : _rightPolygon;

    int n = polygon.size();
    auto *prevVertex = &polygon[n-1];
    QLineF *prevEdge = new QLineF(polygon[0], *prevVertex);
    auto firstEdge = prevEdge;

    for (int i = 0; i < n; i++)
    {
        QPoint* vertex = &polygon[i];

        QPoint *nextVertex = &polygon[(i+1)%n];
        QLineF *nextEdge = i == n-1 ? firstEdge : new QLineF(*vertex, *nextVertex);
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
}

double CollisionDetection::horizontalDistance(const QPoint &point, const QLineF &edge)
{
    double inf = _pCrtanje ? _pCrtanje->width() : CANVAS_WIDTH;
    QLineF horizontalLine(0, point.y(), inf, point.y());

    double dist;
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
        // TODO: is width ok?
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
    _edge.setLength(0);
}

void CollisionDetection::setHorizontalLineY(double y)
{
    _horizontalLineY = y;
}


