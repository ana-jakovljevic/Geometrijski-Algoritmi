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

}

void CollisionDetection::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;
}

void CollisionDetection::pokreniNaivniAlgoritam()
{

}

void CollisionDetection::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;
}

void CollisionDetection::generateRandomPolygons(int brojTacaka)
{
    _polygon1 = generisiNasumicneTackeZaPoligon(brojTacaka);
    scalePolygonAlongXAxis(_polygon1, 0.5);
    _polygon2 = generisiNasumicneTackeZaPoligon(brojTacaka);
    scalePolygonAlongXAxis(_polygon2, 0.5);
    shiftPolygonAlongXAxis(_polygon2, CANVAS_WIDTH/2);
}

void CollisionDetection::scalePolygonAlongXAxis(std::vector<QPoint> &polygon, double factor)
{
    int n = polygon.size();
    for (int i = 0; i < n; i++)
    {
//        polygon[i].setX(factor*polygon[i].x());
        polygon[i].rx() *= factor;
    }
}

void CollisionDetection::shiftPolygonAlongXAxis(std::vector<QPoint> &polygon, int d)
{
    int n = polygon.size();
    for (int i = 0; i < n; i++)
    {
//        polygon[i].setX(polygon[i].x() + d);
        polygon[i].rx() += d;
    }
}

void CollisionDetection::loadPolygonsFromFile(std::string imeDatoteke)
{
    std::ifstream inputFile(imeDatoteke);
    std::string line1, line2;
    std::getline(inputFile, line1);
    std::getline(inputFile, line2);

    _polygon1 = parsePointsFromLine(line1);
    _polygon2 = parsePointsFromLine(line2);
}

std::vector<QPoint> CollisionDetection::parsePointsFromLine(std::string line)
{
    std::stringstream ss;
    ss << line;
    std::vector<QPoint> polygon;
    int x, y;
    while(!ss.eof())
    {
        ss >> x >> y;
        polygon.emplace_back(x, y);
    }
    return polygon;
}





