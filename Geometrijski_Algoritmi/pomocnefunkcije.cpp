#include "pomocnefunkcije.h"

#include <QtGlobal>

//funkcija koja odredjuje da li tacka C pripada segmentu AB
bool pomocneFunkcije::pripada(QPointF A, QPointF C, QPointF B) {
    if (C.x() <= std::max(A.x(), B.x()) && C.x() >= std::min(A.x(), B.x()) &&
        C.y() <= std::max(A.y(), B.y()) && C.y() >= std::min(A.y(), B.y()))
       return true;

    return false;
}
//funkcija vraca 0 ako su tacke kolinearne, 1 ako su orijentisane
//u smeru kazaljke na satu i 2 inace
int pomocneFunkcije::orijentacija(QPointF A, QPointF B, QPointF C) {
    double br = (C.x() - B.x())*(B.y() - A.y()) -
                (B.x() - A.x()) * (C.y() - B.y());

    if (bliski(br, 0)) return 0;
    else if(br>0) return 1;
    return 2;
}

bool pomocneFunkcije::bliski(float a, float b)
{
    return fabsf(a - b) < EPSf;
}

bool pomocneFunkcije::bliski(double a, double b)
{
    return fabs(a - b) < EPS;
}

int pomocneFunkcije::povrsinaTrougla(const QPoint& A, const QPoint& B, const QPoint& C)
{
    /* (Dvostruka) Povrsina trougla.
     * 2P(Trougla) = |ax ay 1|
     *               |bx by 1|
     *               |cx cy 1|
     */
    return (B.x() - A.x())*(C.y() - A.y()) - (C.x() - A.x())*(B.y() - A.y());
}

int pomocneFunkcije::distanceKvadrat(const QPoint& A, const QPoint& B)
{
    return (A.x() - B.x())*(A.x() - B.x()) + (A.y() - B.y())*(A.y() - B.y());
}

bool pomocneFunkcije::kolinearne3D(const QVector3D& a, const QVector3D& b, const QVector3D& c)
{
    /* Proverava se povrsina trougla. Zaparvo da li su sva tri unakrsna proizvoda
     * vektora jednaki nuli.
     * Kako je u pitanju float vrednost, gledamo da li je apsultna vrednost manja od
     * nekog malog broja.
     * Tj. proveravamo sledece
     * |  i      j      k  |
     * |ax-bx  ay-by  az-bz| = (0, 0, 0)
     * |ax-cx  ay-cy  az-cz|
     */
    return bliski((c.z() - a.z()) * (b.y() - a.y()),
                  (b.z() - a.z()) * (c.y() - a.y())) &&
           bliski((b.z() - a.z()) * (c.x() - a.x()),
                  (b.x() - a.x()) * (c.z() - a.z())) &&
           bliski((b.x() - a.x()) * (c.y() - a.y()),
                  (b.y() - a.y()) * (c.x() - a.x()));
}


float pomocneFunkcije::zapremina(const QVector3D& a, const QVector3D& b, const QVector3D& c, const QVector3D& d)
{
    /* Zapremina se racuna kao determinanta
     * 6V(Tetraedar) = |ax ay az 1|
     *                 |bx by bz 1|
     *                 |cx cy cz 1|
     *                 |dx dy dz 1|
     */
    float bxdx = b.x() - d.x();
    float bydy = b.y() - d.y();
    float bzdz = b.z() - d.z();
    float cxdx = c.x() - d.x();
    float cydy = c.y() - d.y();
    float czdz = c.z() - d.z();
    float vol =    (a.z() - d.z()) * (bxdx*cydy - bydy*cxdx)
                +  (a.y() - d.y()) * (bzdz*cxdx - bxdx*czdz)
                +  (a.x() - d.x()) * (bydy*czdz - bzdz*cydy);

    if (fabsf(vol) < EPSf)
        return 0;
    else
        return vol;
}

bool pomocneFunkcije::presekDuzi(const QLineF& l1, const QLineF& l2, QPointF& presek)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return l1.intersects(l2, &presek) == QLineF::BoundedIntersection;
#else
    return l1.intersect(l2, &presek) == QLineF::BoundedIntersection;
#endif
}

bool pomocneFunkcije::paralelneDuzi(const QLineF &l1, const QLineF &l2)
{
    QPointF presek;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return l1.intersects(l2, &presek) == QLineF::NoIntersection;
#else
    return l1.intersect(l2, &presek) == QLineF::NoIntersection;
#endif
}

double pomocneFunkcije::distanceKvadratF(const QPointF& A, const QPointF& B)
{
    return (A.x() - B.x())*(A.x() - B.x()) + (A.y() - B.y())*(A.y() - B.y());
}

double pomocneFunkcije::povrsinaTrouglaF(const QPointF& A, const QPointF& B, const QPointF& C)
{
    /* (Dvostruka) Povrsina trougla.
     * 2P(Trougla) = |ax ay 1|
     *               |bx by 1|
     *               |cx cy 1|
     */
    return (B.x() - A.x())*(C.y() - A.y()) - (C.x() - A.x())*(B.y() - A.y());
}

bool pomocneFunkcije::ispod(const QPointF &A, const QPointF &B)
{
    if (A.y() < B.y())
        return true;
    else if (bliski(A.y(), B.y()))
        return A.x() > B.x();
    else return false;
}

bool pomocneFunkcije::konveksan(const QPointF &A, const QPointF &B, const QPointF &C)
{
    double P = (B.x() - A.x())*(C.y() - A.y()) - (C.x() - A.x())*(B.y() - A.y());

     return (P > 0) ||
            (fabs(P) < EPS && pomocneFunkcije::distanceKvadratF(A, B)
                            < pomocneFunkcije::distanceKvadratF(A, C));
}

void pomocneFunkcije::sortirajTackeZaProstPoligon(std::vector<QPoint> &tacke)
{
    /*
     *  Sortiramo tacke tako da kada se obilaze redom predstavljaju
     *  temena PROSTOG poligona u smeru suprotnom od kazaljke na satu.
     */
    QPoint maxTacka = tacke[0];

    for (auto i = 1ul; i < tacke.size(); i++) {
        if (tacke[i].x() > maxTacka.x() ||
           (tacke[i].x() == maxTacka.x() && tacke[i].y() < maxTacka.y()))
            maxTacka = tacke[i];
    }

    std::sort(tacke.begin(), tacke.end(), [&](const auto& lhs, const auto& rhs) {
        return pomocneFunkcije::konveksan(maxTacka, lhs, rhs);
    });
}


void pomocneFunkcije::sortirajTackeZaProstPoligon(std::vector<QPointF> &tacke)
{
    /*
     *  Sortiramo tacke tako da kada se obilaze redom predstavljaju
     *  temena PROSTOG poligona u smeru suprotnom od kazaljke na satu.
     */
    QPointF maxTacka = tacke[0];

    for (auto i = 1ul; i < tacke.size(); i++) {
        if (tacke[i].x() > maxTacka.x() ||
           (pomocneFunkcije::bliski(tacke[i].x(), maxTacka.x()) && tacke[i].y() < maxTacka.y()))
            maxTacka = tacke[i];
    }

    std::sort(tacke.begin(), tacke.end(), [&](const auto& lhs, const auto& rhs) {
        return pomocneFunkcije::konveksan(maxTacka, lhs, rhs);
    });
}

qreal pomocneFunkcije::ugaoDuzi(const QLineF& line)
{
    return line.angle();
}
