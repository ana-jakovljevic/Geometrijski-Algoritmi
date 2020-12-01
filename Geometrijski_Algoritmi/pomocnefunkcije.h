#ifndef POMOCNEFUNKCIJE_H
#define POMOCNEFUNKCIJE_H

#include<QPoint>
#include<QLineF>
#include<QVector3D>

#include <config.h>

namespace pomocneFunkcije {
int povrsinaTrougla(const QPoint& A, const QPoint& B, const QPoint& C);
float povrsinaTrouglaF(const QPointF& A, const QPointF& B, const QPointF& C);
int distanceKvadrat(const QPoint& A, const QPoint& B);
QPointF presek_duzi(const QLineF& duz1, const QLineF& duz2);

/* Pomocni metodi za 3D. */
bool kolinearne3D(const QVector3D& a, const QVector3D& b, const QVector3D& c);
float zapremina(const QVector3D& a, const QVector3D& b, const QVector3D& c, const QVector3D& d);
}

#endif // POMOCNEFUNKCIJE_H
