#ifndef GA07_KONTURA_H
#define GA07_KONTURA_H

#include <QPointF>
#include <QLineF>
#include <QRect>
#include <set>
#include <stack>
#include <math.h>

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
    tacka1d(const _tip &_t, float _vr) : tip(_t), vr(_vr), normalizovanY(-1) {}
    const _tip tip;
    float vr;
    int normalizovanY;
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

class Tacka : public QPointF {
    using QPointF::QPointF;
public:
    int normalizovanY = 0;
};
class st_Ivica {
public:
    Tacka* pt1;
    Tacka* pt2;
    _tip tip;

    st_Ivica(Tacka* _pt1, Tacka* _pt2, _tip __tip) : pt1(_pt1), pt2(_pt2), tip(__tip) {};
};

enum class SegStatus {
    EMPTY,
    PARTIAL,
    FULL,
};

struct Segment {
    float b;
    float e;
    int c = 0;
    SegStatus status = SegStatus::EMPTY;
    Segment* lson = 0;
    Segment* rson = 0;

    Segment(float _b, float _e) {
//         if (b > e) throw new Error(`B > E`);
        b = _b;
        e = _e;
        c = 0;
        if (e - b > 1) {
            lson = new Segment(b, floor((b + e) / 2));
            rson = new Segment(floor((b + e) / 2), e);
        }
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

    // Naivni algoritam - faza 1
    void bf_pocetakPravougaonika(ivica* iv);
    void bf_krajPravougaonika(ivica* iv);
    void dodajVertikalnuIvicu(ivica* iv);
    void dodajHorizontalnuIvicu(QLineF* duz);

    // Segmentno drvo - faza 1
    void comp(Segment* v, bool inFull);
    void updateStatus(Segment* v);
    void lp_insert(float b, float e, Segment* v, bool inFull = false);
    void lp_delete(float b, float e, Segment* v, bool inFull = false);
    std::vector<Tacka*> normalizeEdgesForSegmentTree(std::vector<st_Ivica*> inEdges, std::vector<st_Ivica*> outEdges);

    void faza2();

    std::vector<QRectF> _pravougaonici;
    std::vector<QPointF*> _kontura;
    double _brisucaPravaY;

    std::vector<QLineF*> iviceKonture;
    std::multiset<ivica*, poredjenjeIvicaPoX> ph1_vertikalneIvice;
    std::multiset<tacka1d*, poredjenje1d> bf_tackeUKonturi;
    std::vector<float> st_stack;
};

#endif // GA07_KONTURA_H
