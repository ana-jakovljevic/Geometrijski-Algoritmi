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
    std::vector<QPoint> _polygon1;
    std::vector<QPoint> _polygon2;

    void generateRandomPolygons(int brojTacaka);
    void loadPolygonsFromFile(std::string imeDatoteke);

    std::vector<QPoint> parsePointsFromLine(std::string line);
};

#endif // COLLISIONDETECTION_H
