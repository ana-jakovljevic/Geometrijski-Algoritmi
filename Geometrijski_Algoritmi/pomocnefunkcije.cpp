#include "pomocnefunkcije.h"
#include "math.h"

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
    return (fabsf((c.z() - a.z()) * (b.y() - a.y()) -
              (b.z() - a.z()) * (c.y() - a.y())) < EPS) &&
           (fabsf((b.z() - a.z()) * (c.x() - a.x()) -
              (b.x() - a.x()) * (c.z() - a.z())) < EPS) &&
           (fabsf((b.x() - a.x()) * (c.y() - a.y()) -
              (b.y() - a.y()) * (c.x() - a.x())) < EPS);
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

    if (fabs(vol) < EPS)
        return 0;
    else
        return vol;
}

bool pomocneFunkcije::presekDuzi(QLineF l1, QLineF l2, QPointF &presek)
{
    double k1 = (l1.p1().y() - l1.p2().y())/(l1.p1().x() - l1.p2().x());
    double k2 = (l2.p1().y() - l2.p2().y())/(l2.p1().x() - l2.p2().x());

    double n1 = l1.p1().y() - k1*l1.p1().x();
    double n2 = l2.p1().y() - k2*l2.p1().x();

    double dx = (n2-n1)/(k1-k2);

    //Ovim smo izracunali presek dve prave, koje su odredjene duzima l1 i l2.
    //Treba proveriti da li presek pripada zaista duzima (moze biti na pravi, ali van duzi),
    //u suprotnom se ovaj presek zanemaruje
    //zato return na kraju
    presek = QPointF(dx, k1*dx + n1);

    return duzSadrziTacku(l1, presek) && duzSadrziTacku(l2, presek);
}

bool pomocneFunkcije::duzSadrziTacku(QLineF l, QPointF p)
{
    if(l.p1().x() < l.p2().x())
    {
        if(p.x() < l.p1().x() || p.x() > l.p2().x())
            return false;
    }else{
        if(p.x() > l.p1().x() || p.x() < l.p2().x())
            return false;
    }

    if(l.p1().y() < l.p2().y()){
        if(p.y() < l.p1().y() || p.y() > l.p2().y())
            return false;
    }else{
        if(p.y() > l.p1().y() || p.y() < l.p2().y())
            return false;
    }

    return true;
}
