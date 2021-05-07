#include "pomocnefunkcije.h"

#include <QtGlobal>

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




std::pair<QPolygonF::const_iterator, QPolygonF::const_iterator> pomocneFunkcije::tackeOslonca(
        const QPointF &refTacka, const QPolygonF &poligon)
{
    auto ugaoPosle = ugaoIzmedjuTriTacke(refTacka, *poligon.begin(), *(poligon.begin() + 1));
    auto ugaoPre = ugaoIzmedjuTriTacke(refTacka, *poligon.begin(), *poligon.end());
    PravaOsloncaSlucaj slucaj;
    if (ugaoPosle > 0) {
        if (ugaoPre > 0) slucaj = PravaOsloncaSlucaj::RASTE_OPADA;
        else slucaj = PravaOsloncaSlucaj::RASTE_OPADA_RASTE;
    } else {
        if (ugaoPre < 0) slucaj = PravaOsloncaSlucaj::OPADA_RASTE;
        else slucaj = PravaOsloncaSlucaj::OPADA_RASTE_OPADA;
    }

    auto first = poligon.begin();
    auto second = poligon.end();
    switch (slucaj) {
        case PravaOsloncaSlucaj::RASTE_OPADA:
            second = binarnaPretragaUglovaPoligona(refTacka, poligon, true);
            break;
        case PravaOsloncaSlucaj::RASTE_OPADA_RASTE:
            break;
        case PravaOsloncaSlucaj::OPADA_RASTE:
            second = binarnaPretragaUglovaPoligona(refTacka, poligon, false);
            break;
        case PravaOsloncaSlucaj::OPADA_RASTE_OPADA:
            break;
    }
    return std::make_pair(first, second);
}


qreal pomocneFunkcije::ugaoIzmedjuTriTacke(const QPointF &O, const QPointF &X, const QPointF &Y)
{
    return QLineF(X, O).angleTo(QLineF(Y, O));
}

#include <QDebug>
QPolygonF::const_iterator pomocneFunkcije::binarnaPretragaUglovaPoligona(const QPointF &refTacka,
                                                                   const QPolygonF &poligon, bool findMax)
{
    uint16_t l = 0, d = poligon.size()-1;
    while (l < d) {
        auto mid = (l + d) / 2;
        auto midUgao = ugaoIzmedjuTriTacke(refTacka, poligon[mid], poligon[0]);
        auto lUgao = ugaoIzmedjuTriTacke(refTacka, poligon[mid-1], poligon[0]);
        auto dUgao = ugaoIzmedjuTriTacke(refTacka, poligon[mid+1], poligon[0]);
        if (findMax) {
            if (l + 1 == d && midUgao < dUgao) {
               return poligon.begin() + d + 1;
            }
            if (lUgao < midUgao && midUgao > dUgao)
                return (poligon.begin() + mid + 1);
            else if (lUgao < midUgao && midUgao < dUgao)
                l = mid;

            else
                d = mid;
        } else {
            if (lUgao > midUgao && midUgao < dUgao)
                return (poligon.begin() + mid);
            else if (lUgao < midUgao && midUgao < dUgao)
                d = mid;

            else
                l = mid;
        }
    }

    return poligon.begin();
}
