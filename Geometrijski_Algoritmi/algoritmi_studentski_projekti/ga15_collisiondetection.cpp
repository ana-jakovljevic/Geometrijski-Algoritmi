#include "ga15_collisiondetection.h"

CollisionDetection::CollisionDetection(QWidget *pCrtanje,
                                       int pauzaKoraka,
                                       const bool &naivni,
                                       std::string imeDatoteke,
                                       int brojTacaka)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{

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


