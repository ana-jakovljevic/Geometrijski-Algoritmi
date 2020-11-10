#include "pomocnefunkcije.h"
using namespace pomocneFunkcije;

double povrsinaTrougla(const QPoint& A, const QPoint& B, const QPoint& C)
{
    // returns P/2
    return (B.x() - A.x())*(C.y() - A.y()) - (C.x() - A.x())*(B.y() - A.y());
}

double distanceKvadrat(const QPoint& A, const QPoint& B)
{
    return (A.x() - B.x())*(A.x() - B.x()) + (A.y() - B.y())*(A.y() - B.y());
}
