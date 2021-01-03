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
    while(1)
        AlgoritamBaza_updateCanvasAndBlock();

    emit animacijaZavrsila();
}

void CollisionDetection::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    painter->drawPolygon(_polygon1.data(), _polygon1.size());
    painter->drawPolygon(_polygon2.data(), _polygon2.size());
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
    std::vector<QPoint> points = AlgoritamBaza::generisiNasumicneTacke(brojTacaka);

    for (int i = 0; i < brojTacaka; i++)
    {
        if (points[i].x() < (_pCrtanje->width())/2)
            _polygon1.emplace_back(points[i]);
        else
            _polygon2.emplace_back(points[i]);
    }

    pomocneFunkcije::sortirajTackeZaProstPoligon(_polygon1);
    pomocneFunkcije::sortirajTackeZaProstPoligon(_polygon2);
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





