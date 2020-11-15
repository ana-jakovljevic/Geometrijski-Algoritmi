#ifndef POMOCNEFUNKCIJE_H
#define POMOCNEFUNKCIJE_H

#include<QPoint>
#include<QVector3D>

namespace pomocneFunkcije {
int povrsinaTrougla(const QPoint& A, const QPoint& B, const QPoint& C);
int distanceKvadrat(const QPoint& A, const QPoint& B);

/* Pomocni metodi za 3D. */
bool kolinearne3D(QVector3D a, QVector3D b, QVector3D c);
double zapremina(QVector3D a, QVector3D b, QVector3D c, QVector3D d);
}

#endif // POMOCNEFUNKCIJE_H
