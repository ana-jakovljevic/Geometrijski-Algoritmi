#ifndef POMOCNEFUNKCIJE_H
#define POMOCNEFUNKCIJE_H

#include <QPoint>
#include <QPointF>
#include <QLineF>
#include <QVector3D>
#include <cmath>

#include <config.h>

namespace pomocneFunkcije {
bool pripada(QPointF a, QPointF c, QPointF b);
int orijentacija(QPointF a, QPointF b, QPointF c);
bool bliski(float a, float b);
bool bliski(double a, double b);
int povrsinaTrougla(const QPoint& A, const QPoint& B, const QPoint& C);
int distanceKvadrat(const QPoint& A, const QPoint& B);
bool presekDuzi(const QLineF& l1, const QLineF& l2, QPointF& presek);
bool paralelneDuzi(const QLineF& l1, const QLineF& l2);
double distanceKvadratF(const QPointF& A, const QPointF& B);
bool ispod(const QPointF& A, const QPointF& B);
bool konveksan(const QPointF& A, const QPointF& B, const QPointF& C);

///
/// \brief sortirajTackeZaProstPoligon - rasporedjuje tacke kao temena prostog poligona
///
void sortirajTackeZaProstPoligon(std::vector<QPoint> &tacke);
void sortirajTackeZaProstPoligon(std::vector<QPointF> &tacke);

/* Pomocni metodi za 3D. */
bool kolinearne3D(const QVector3D& a, const QVector3D& b, const QVector3D& c);
float zapremina(const QVector3D& a, const QVector3D& b, const QVector3D& c, const QVector3D& d);

/* Pomocni metodi za ConvexHullLineIntersections */
qreal ugaoDuzi(const QLineF& line);
double povrsinaTrouglaF(const QPointF& A, const QPointF& B, const QPointF& C);

}

#endif // POMOCNEFUNKCIJE_H
