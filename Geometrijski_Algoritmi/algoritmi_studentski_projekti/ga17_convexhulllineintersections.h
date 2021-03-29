#ifndef GA17_CONVEXHULLLINEINTERSECTIONS_H
#define GA17_CONVEXHULLLINEINTERSECTIONS_H

#include "algoritambaza.h"
#include <set>
#include <map>
#include <iostream>

class ConvexHullLineIntersections : public AlgoritamBaza
{
public:
    ConvexHullLineIntersections(QWidget *pCrtanje,
                                int pauzaKoraka,
                                const bool &naivni = false,
                                std::string imeDatoteke = "",
                                int brojDuzi = BROJ_SLUCAJNIH_OBJEKATA);

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
    const std::vector<QPointF> &getKonveksniOmotac();
    const std::vector<QPointF> &getNaivniKonveksniOmotacGrem();
    const std::vector<QLineF> &getNaivniKonveksniOmotac();
    void generateAngles(const int N, const double minAngleDegree, const double maxAngleDegree, std::set<double> &angles);
    std::vector<QLineF> generisiNasumicneLinije(int brojDuzi = BROJ_SLUCAJNIH_OBJEKATA);
    bool getGREMOV_NAIVNI() const;

private:
    std::vector<QLineF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;
    std::vector<QPointF> vratiRazapinjuceTacke(double angle, double n) const;
    void pretvoriDuziUMapu()
    {
        for (auto &d: _duzi){
            auto angle = d.angle();

            /* kad ih je vise od 2 paralelne, onda mora da se nekako vidi koje su ekstremne (na krajevima)
             * tu moram da izbacim sve ove izmedju, ali tu ne upadam u slozenost nlogn ako sortiram kroz mapu
             */
            if (_mapaUgaoDuzi[angle].size() < 2) {
                _mapaUgaoDuzi[angle].push_back(d);
            }
            else {
                /* paralelne sa y osom */
                if (pomocneFunkcije::bliski(d.x1(), d.x2())) {
                    auto element1 = _mapaUgaoDuzi[angle].at(0);
                    auto element2 = _mapaUgaoDuzi[angle].at(1);

                    bool prviVeci = false;
                    if (element1.x2() >= element2.x2())
                        prviVeci = true;

                    if (prviVeci) {
                        if (d.x2() > element1.x2()) {
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].push_back(element2);
                            _mapaUgaoDuzi[angle].push_back(d);
                        }
                        else if (d.x2() < element2.x2()) {
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].push_back(element1);
                            _mapaUgaoDuzi[angle].push_back(d);
                        }
                    }
                    else {
                        if (d.x2() > element2.x2()){
                           _mapaUgaoDuzi[angle].pop_back();
                           _mapaUgaoDuzi[angle].pop_back();
                           _mapaUgaoDuzi[angle].push_back(element1);
                           _mapaUgaoDuzi[angle].push_back(d);
                        }
                        else if (d.x2() < element1.x2()) {
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].push_back(d);
                            _mapaUgaoDuzi[angle].push_back(element2);
                        }
                   }
                }
                /* paralelne sa x osom */
                else if (pomocneFunkcije::bliski(d.y1(), d.y2())){
                    auto element1 = _mapaUgaoDuzi[angle].at(0);
                    auto element2 = _mapaUgaoDuzi[angle].at(1);

                    bool prviVeci = false;
                    if (element1.y2() > element2.y2())
                        prviVeci = true;

                    if (prviVeci) {
                        if (d.y2() > element1.y2()) {
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].push_back(d);
                            _mapaUgaoDuzi[angle].push_back(element2);
                        }
                        else if (d.y2() < element2.y2()) {
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].push_back(element1);
                            _mapaUgaoDuzi[angle].push_back(d);
                        }
                    }
                    else {
                        if (d.y2() > element2.y2()){
                           _mapaUgaoDuzi[angle].pop_back();
                           _mapaUgaoDuzi[angle].pop_back();
                           _mapaUgaoDuzi[angle].push_back(d);
                           _mapaUgaoDuzi[angle].push_back(element1);
                        }
                        else if (d.y2() < element1.y2()) {
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].pop_back();
                            _mapaUgaoDuzi[angle].push_back(element2);
                            _mapaUgaoDuzi[angle].push_back(d);
                        }
                   }
                }
            }
        }
    }

    bool presekLinija(const QLineF& l1, const QLineF& l2, QPointF& presek);
    void naglasiTrenutno(QPainter *painter, const QLineF *d, unsigned long i, const char *s) const;
    void gremovAlgoritam(bool naivni);

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

    bool _GREMOV_NAIVNI = true;
};

#endif // GA17_CONVEXHULLLINEINTERSECTIONS_H
