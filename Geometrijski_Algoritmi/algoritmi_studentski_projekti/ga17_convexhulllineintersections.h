#ifndef GA17_CONVEXHULLLINEINTERSECTIONS_H
#define GA17_CONVEXHULLLINEINTERSECTIONS_H

#include "algoritambaza.h"
#include <set>
#include <map>

class ConvexHullLineIntersections : public AlgoritamBaza
{
public:
    ConvexHullLineIntersections(QWidget *pCrtanje,
                                int pauzaKoraka,
                                const bool &naivni = false,
                                std::string imeDatoteke = "",
                                int brojDuzi = BROJ_SLUCAJNIH_OBJEKATA);

    //~ConvexHullLineIntersections();

    struct poredjenjeDuzi {
        bool operator()(const QLineF &duz1, const QLineF &duz2) const {
            return duz1.angle() < duz2.angle();
        }
    };

    struct poredjenjeTacaka {
        bool operator()(const QPointF &tacka1, const QPointF &tacka2) const {
            return (tacka1.x() < tacka2.x()) ||
                   ( fabs(tacka1.x() - tacka2.x()) < static_cast<double>(EPSf) && tacka1.y() < tacka2.y());
        }
    };

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    const std::vector<QPointF> &getPreseci() const;
    const std::vector<QPointF> &getNaivniPreseci() const;
    const std::vector<QPointF> &getKonveksniOmotac() const;
    const std::vector<QPointF> &getNaivniKonveksniOmotacGrem() const;
    const std::vector<QLineF> &getNaivniKonveksniOmotac() const;
    void generateAngles(const int N, const double minAngleDegree, const double maxAngleDegree, std::set<double> &angles);
private:
    std::vector<QLineF> generisiNasumicneLinije(int brojDuzi = BROJ_SLUCAJNIH_OBJEKATA);
    std::vector<QLineF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;
    std::vector<QPointF> vratiRazapinjuceTacke(double angle, double n) const;
    void pretvoriDuziUMapu()
    {
        for (auto &d: _duzi){
            auto angle = d.angle();
            _mapaUgaoDuzi[angle].push_back(d);
        }
    }

    bool presekLinija(const QLineF& l1, const QLineF& l2, QPointF& presek);
    void naglasiTrenutno(QPainter *painter, const QLineF *d, unsigned long i, const char *s) const;
    void gremovAlgoritam();

    std::vector<QLineF> _duzi;
    std::map<double, std::vector<QLineF>> _mapaUgaoDuzi;
    std::set<QLineF, poredjenjeDuzi> _sortiraneDuziPoUglu;

    std::vector<QPointF> _preseci;
    std::set<QPointF, poredjenjeTacaka> _preseciSet;
    std::vector<QPointF> _sviPreseci;

    std::vector<QPointF> _konveksniOmotac;
    std::vector<QPointF> _naivniOmotacGrem;

    std::vector<QLineF> _naivniOmotac;
    unsigned long _naivnoI, _naivnoJ, _naivnoK;
    double _naivnoPovrsina;
};

#endif // GA17_CONVEXHULLLINEINTERSECTIONS_H
