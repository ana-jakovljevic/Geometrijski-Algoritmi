#ifndef GA07_KONTURA_H
#define GA07_KONTURA_H

#include <QPointF>
#include <QLineF>
#include <QRect>
#include <set>

#include "algoritambaza.h"

enum class tipIvice {
    GORNJA_IVICA,
    DONJA_IVICA
};

struct ivica {
    ivica(const tipIvice &t, QLineF *const d) : tip(t), duz(d) {}

    const tipIvice tip;
    QLineF *const duz;
};

struct poredjenjeIvica {
    bool operator()(const ivica &prva, const ivica &druga) const {
        return prva.duz->y1() < druga.duz->y1(); /*||
               (pomocneFunkcije::bliski(prva.tacka.y(), druga.tacka.y()) &&
                                        prva.tacka.x() < druga.tacka.x());*/
    }
};

struct poredjenjeTacakaPoX {
    bool operator()(const QPointF *prva, const QPointF *druga) const {
        return prva->x() < druga->x();
    }
};

class KonturaPravougaonika : public AlgoritamBaza
{
public:
   KonturaPravougaonika(QWidget *pCrtanje,
                int pauzaKoraka,
                const bool &naivni = false,
                std::string imeDatoteke = "",
                int brojPravougaonika = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;
private:
    std::vector<QRectF> generisiNasumicnePravougaonike(int brojDuzi = BROJ_SLUCAJNIH_OBJEKATA) const;
    std::vector<QRectF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;

    std::vector<QRectF> _pravougaonici;
    std::vector<QPointF*> _kontura;
    double _brisucaPravaY;

    std::set<ivica, poredjenjeIvica> _ivice;
    std::set<QPointF*, poredjenjeTacakaPoX> _najlevlje;
    std::set<QPointF*, poredjenjeTacakaPoX> _najdesnije;
};

#endif // GA07_KONTURA_H
