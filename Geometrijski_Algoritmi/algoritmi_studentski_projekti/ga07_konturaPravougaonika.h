#ifndef GA07_KONTURA_H
#define GA07_KONTURA_H

#include <QPointF>
#include <QLineF>
#include <QRect>
#include <set>

#include "algoritambaza.h"
#include "pomocnefunkcije.h"

enum class _tip {
    ULAZ,
    IZLAZ
};

struct ivica {
    ivica(const _tip &t, QLineF *const d) : tip(t), duz(d) {}

    const _tip tip;
    QLineF *const duz;
};

struct vertikalnaIvica {
    vertikalnaIvica(int _y1, int _y2, int _x, const _tip &t) : y1(_y1), y2(_y2), x(_x), tip(t) {}
    int y1, y2, x;
    const _tip tip;
};

struct tacka1d {
    tacka1d(const _tip &_t, float _vr) : tip(_t), vr(_vr) {}
    const _tip tip;
    float vr;
};

struct poredjenje1d {
    bool operator()(const tacka1d* prva, const tacka1d* druga) const {
        return prva->vr < druga->vr;
    }
};

struct poredjenjeIvicaPoY {
    bool operator()(const ivica &prva, const ivica &druga) const {
        return prva.duz->y1() < druga.duz->y1(); /*||
               (pomocneFunkcije::bliski(prva.tacka.y(), druga.tacka.y()) &&
                                        prva.tacka.x() < druga.tacka.x());*/
    }
};

struct poredjenjeIvicaPoX {
    bool operator()(const ivica* prva, const ivica* druga) const {
        return prva->duz->x1() < druga->duz->x1();
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
    void pocetakPravougaonika(ivica* iv);
    void krajPravougaonika(ivica* iv);
    void dodajVertikalnuIvicu(ivica* iv);

    std::vector<QRectF> _pravougaonici;
    std::vector<QPointF*> _kontura;
    double _brisucaPravaY;

    std::set<ivica, poredjenjeIvicaPoY> _ivice;
    std::multiset<ivica*, poredjenjeIvicaPoX> ph1_vertikalneIvice;
    // koristim multiset umesto set jer sa set samo prvi poziv dodajVertikalnuIvicu uspesno ubacuje element
    std::set<tacka1d*, poredjenje1d> ph1_tackeUKonturi;
    std::set<QPointF*, poredjenjeTacakaPoX> _najlevlje;
    std::set<QPointF*, poredjenjeTacakaPoX> _najdesnije;
};

#endif // GA07_KONTURA_H
