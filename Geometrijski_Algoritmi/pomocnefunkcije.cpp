#include "pomocnefunkcije.h"
#include "math.h"

/* Ne prevelika tolerancija na numericku gresku */
#define EPS 1e-6

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
    return (fabs((c.z() - a.z()) * (b.y() - a.y()) -
              (b.z() - a.z()) * (c.y() - a.y())) < EPS) &&
           (fabs((b.z() - a.z()) * (c.x() - a.x()) -
              (b.x() - a.x()) * (c.z() - a.z())) < EPS) &&
           (fabs((b.x() - a.x()) * (c.y() - a.y()) -
              (b.y() - a.y()) * (c.x() - a.x())) < EPS);
}


double  pomocneFunkcije::zapremina(const QVector3D& a, const QVector3D& b, const QVector3D& c, const QVector3D& d)
{
    double vol;
    double bxdx, bydy, bzdz, cxdx, cydy, czdz;

    /* Zapremina se racuna kao determinanta
     * 6V(Tetraedar) = |ax ay az 1|
     *                 |bx by bz 1|
     *                 |cx cy cz 1|
     *                 |dx dy dz 1|
     */
    bxdx=b.x()-d.x();
    bydy=b.y()-d.y();
    bzdz=b.z()-d.z();
    cxdx=c.x()-d.x();
    cydy=c.y()-d.y();
    czdz=c.z()-d.z();
    vol =    (a.z()-d.z())*(bxdx*cydy-bydy*cxdx)
          +  (a.y()-d.y())*(bzdz*cxdx-bxdx*czdz)
          +  (a.x()-d.x())*(bydy*czdz-bzdz*cydy);

    if (fabs(vol) < EPS)
        return 0;
    else
        return vol;
}
