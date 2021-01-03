#include "ga15_collisiondetection.h"

#include <fstream>
#include <sstream>

CollisionDetection::CollisionDetection(QWidget *pCrtanje,
                                       int pauzaKoraka,
                                       const bool &naivni,
                                       std::string imeDatoteke,
                                       int brojTacaka)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    if (imeDatoteke == "")
        generateRandomPolygons(brojTacaka);
    else
        loadPolygonsFromFile(imeDatoteke);
}

void CollisionDetection::pokreniAlgoritam()
{
    AlgoritamBaza_updateCanvasAndBlock();

    emit animacijaZavrsila();
}

void CollisionDetection::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    painter->drawPolygon(_leftPolygon);
    painter->drawPolygon(_rightPolygon);
}

void CollisionDetection::pokreniNaivniAlgoritam()
{

}

void CollisionDetection::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;

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



