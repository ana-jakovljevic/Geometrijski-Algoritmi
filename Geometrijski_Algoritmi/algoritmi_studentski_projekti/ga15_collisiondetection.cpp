#include "ga15_collisiondetection.h"

#include <fstream>
#include <sstream>

CollisionDetection::CollisionDetection(QWidget *pCrtanje,
                                       int pauzaKoraka,
                                       const bool &naivni,
                                       std::string imeDatoteke,
                                       int brojTacaka)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni), _leftPolygon(), _rightPolygon(),
      _horizontalLine(), _edge(), _intersectionPoint()
{
    _collisionVertex = nullptr;
    _minDistance = std::numeric_limits<double>::infinity();
    if (imeDatoteke == "")
        generateRandomPolygons(brojTacaka);
    else
        loadPolygonsFromFile(imeDatoteke);
}

void CollisionDetection::pokreniAlgoritam()
{}

void CollisionDetection::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;
}

void CollisionDetection::pokreniNaivniAlgoritam()
{
    AlgoritamBaza_updateCanvasAndBlock();

    findCollisionVertexInPolygon(WhichPolygon::LEFT);
    findCollisionVertexInPolygon(WhichPolygon::RIGHT);

    shiftLeftPolygonAlongXAxis();

    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

void CollisionDetection::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    QPen pen = painter->pen();

    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter->setPen(pen);
    painter->drawPolygon(_leftPolygon);
    painter->drawPolygon(_rightPolygon);

    if (_collisionVertex)
    {
        pen.setColor(Qt::red);
        pen.setWidth(10);
        painter->setPen(pen);
        painter->drawPoint(*_collisionVertex);
    }

    if (!_intersectionPoint.isNull())
    {
        pen.setColor(Qt::blue);
        pen.setWidth(10);
        painter->setPen(pen);
        painter->drawPoint(_intersectionPoint);
    }

    if(!_horizontalLine.isNull())
    {
        pen.setColor(Qt::green);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawLine(_horizontalLine);
    }

    if(!_edge.isNull())
    {
        pen.setColor(Qt::yellow);
        pen.setWidth(4);
        painter->setPen(pen);
        painter->drawLine(_edge);
    }
}

void CollisionDetection::findCollisionVertexInPolygon(WhichPolygon whichPolygon)
{
    QPolygon &polygon1 = whichPolygon == WhichPolygon::LEFT ? _leftPolygon : _rightPolygon;
    QPolygon &polygon2 = whichPolygon == WhichPolygon::LEFT ? _rightPolygon : _leftPolygon;

    int n = polygon2.size();
    for(QPoint &vertex : polygon1)
    {
        for (int i = 0; i < n; i++)
        {
            _edge.setP1(polygon2[i]);
            _edge.setP2(polygon2[(i+1)%n]);

            // TODO: is this ok?
            // when _pCrtanje is null we dont use crtajAlgoritam?
            double inf = _pCrtanje ? _pCrtanje->width() : std::numeric_limits<double>::infinity();
//            double inf = _pCrtanje ? _pCrtanje->width() : CANVAS_WIDTH;
            double endOfLineX = whichPolygon == WhichPolygon::LEFT ? inf : 0;
            _horizontalLine.setP1(vertex);
            _horizontalLine.setP2(QPointF(endOfLineX, vertex.y()));

            double dist;
            if (pomocneFunkcije::presekDuzi(_edge, _horizontalLine, _intersectionPoint))
                dist = pomocneFunkcije::distanceKvadratF(vertex, _intersectionPoint);
            else
                dist = std::numeric_limits<double>::infinity();

            if (dist < _minDistance)
            {
                _minDistance = dist;
                _collisionVertex = &vertex;
            }

            AlgoritamBaza_updateCanvasAndBlock();
        }
    }

    // set to null
    _intersectionPoint.setX(0);
    _intersectionPoint.setY(0);
    _horizontalLine.setP1(_intersectionPoint);
    _horizontalLine.setP2(_intersectionPoint);
    _edge.setP1(_intersectionPoint);
    _edge.setP2(_intersectionPoint);
}

void CollisionDetection::shiftLeftPolygonAlongXAxis()
{
    double distanceToShift = sqrt(_minDistance);
    double step = 20;
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
        int width;
        if (_pCrtanje) width = _pCrtanje->width();
        else width = CANVAS_WIDTH;

        if (points[i].x() < width/2)
            leftPoints.emplace_back(points[i]);
        else
            rightPoints.emplace_back(points[i]);
    }

    pomocneFunkcije::sortirajTackeZaProstPoligon(leftPoints);
    pomocneFunkcije::sortirajTackeZaProstPoligon(rightPoints);

    for (QPoint &point : leftPoints)
    {
        _leftPolygon.append(point);
    }
    for (QPoint &point : rightPoints)
    {
        _rightPolygon.append(point);
    }
}

void CollisionDetection::loadPolygonsFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName);
    std::string line1, line2;
    std::getline(inputFile, line1);
    std::getline(inputFile, line2);

    _leftPolygon = parsePolygonFromLine(line1);
    _rightPolygon = parsePolygonFromLine(line2);
}

QPolygon CollisionDetection::parsePolygonFromLine(std::string line)
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



