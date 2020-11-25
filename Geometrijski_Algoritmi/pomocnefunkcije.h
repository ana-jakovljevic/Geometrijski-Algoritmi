#ifndef POMOCNEFUNKCIJE_H
#define POMOCNEFUNKCIJE_H

#include<QPoint>
#include<QVector3D>

#include <config.h>

namespace pomocneFunkcije {
int povrsinaTrougla(const QPoint& A, const QPoint& B, const QPoint& C);
int distanceKvadrat(const QPoint& A, const QPoint& B);

/* Pomocni metodi za 3D. */
bool kolinearne3D(const QVector3D& a, const QVector3D& b, const QVector3D& c);
float zapremina(const QVector3D& a, const QVector3D& b, const QVector3D& c, const QVector3D& d);
}

#endif // POMOCNEFUNKCIJE_H
