#ifndef POMOCNEFUNKCIJE_H
#define POMOCNEFUNKCIJE_H

#include <QPoint>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <QVector3D>
#include <cmath>

#include <config.h>

namespace pomocneFunkcije {
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




/**
 * @brief ugaoIzmedjuTriTacke racuna ugao XOY
 * @param O
 * @param X
 * @param Y
 * @return
 */
qreal inline ugaoIzmedjuTriTacke(const QPointF& O, const QPointF& X, const QPointF& Y);

enum class PravaOsloncaSlucaj {
    RASTE_OPADA,
    OPADA_RASTE,
    RASTE_OPADA_RASTE,
    OPADA_RASTE_OPADA
};
/**
 * @brief tackeOslonca Funkcija u vremenu O(log n) vraca presecne tacke pravih oslonca sa poligonom
 * @see https://en.wikipedia.org/wiki/Supporting_line
 * @see https://www.nikolaajzenhamer.rs/assets/pdf/kiaa2.pdf strana 15 zadatak 2.4
 * @param refTacka Referentna tačka u odnosu na koju se gledaju prave oslonca ka poligonu
 * @param poligon
 * @return
 */
std::pair<QPolygonF::const_iterator, QPolygonF::const_iterator> tackeOslonca(const QPointF& refTacka, const QPolygonF& poligon);

/**
 * @brief binarnaPretragaUglovaPoligona Pomocna funkcija za pronalazak tacke oslonca
 *          koja nalazi peak niza uglova binarnom pretragom
 *          na primer: [14, 22, 34, 65, 43, 38, 20]
 * @param refTacka
 * @param poligon
 * @param findMax
 * @return
 */
QPolygonF::const_iterator binarnaPretragaUglovaPoligona(const QPointF& refTacka, const QPolygonF& poligon, bool findMax);

}

#endif // POMOCNEFUNKCIJE_H
