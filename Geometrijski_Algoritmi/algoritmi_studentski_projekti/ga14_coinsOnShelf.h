#ifndef COINSONSHELF_H
#define COINSONSHELF_H

#include "algoritambaza.h"
#include <QRandomGenerator>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <set>
#include <queue>
#include <list>

using namespace std;

/* This enum selects what type of algorithm will be used */
enum AlgoType {
    SPECIAL,
    GENERAL
};


/* Class that will represent disc. Disc is characterised by its radius and footprint. Footprint
 * point is point on horizontal line (shelf) where disc touches it. It's used for drawing and
 * also for calcus
 */
class Disk {
public:
    Disk();
    double radius();
    void setRadius(double newRadius);
    double footprint();
    void setFootprint(double newFootprint);

private:
    double _radius;
    double _footprint = 0;
    QRandomGenerator _randomGen = QRandomGenerator::securelySeeded();
};

/* Class needed for STL sorting for Disk class */
class compDiscs {
public:
    bool operator() (Disk *d1, Disk *d2) const {
        return d1->radius() >= d2->radius();
    }
};

/* This class will represent gap between two discs. When we create new
 * gap, we immediatly calc maximum radius of disc that could be fitted
 * in that gap. Also we have option for manual setting this radius, that't
 * used in naive algorithm, where calc is somewhat different
 */
class MaxGap
{
public:
    MaxGap(Disk *A, Disk *B);
    float maxGapRadius();
    Disk* leftDisk();
    Disk* rightDisk();
    void setGapRadiusManual(float newRadius);
private:
    Disk *_A;
    Disk *_B;
    float _maxGapRadius;
};

/* Class for STL sorting of gaps */
class compMaxGaps {
public:
    bool operator() (MaxGap *g1, MaxGap *g2) const {
        return g1->maxGapRadius() <= g2->maxGapRadius();
    }
};

class CoinsOnShelf : public AlgoritamBaza
{
public:
    CoinsOnShelf(QWidget *pCrtanje,
                 int pauzaKoraka,
                 const bool &naivni = false,
                 std::string imeDatoteke = "",
                 int brojDiskova = 10);

    ~CoinsOnShelf();

    /* Base class methods */
    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    /* Methods that could be used for tests */
    int getShelfSize();
    int getNaiveShelfSize();
    list<Disk *> getShelfDisks();
    vector<Disk*> getShelfNaiveDisks();
    double getSpan();
    double getSpanNaive();

    /* Inner methods used by class */
    void specialCaseEvenDiscs();
    void specialCaseOddDiscs();
    void generalCase();
    void naiveSpecialCase();
    void naiveGeneralCase();

    /* Some helpful utility methods */
    void debugShelf();
    void updateFootprintAB(Disk* A, Disk* B, bool directionBIsRightsideA);
    void printSpan(QPainter *painter, bool naive) const;
    void printSpanFinal(QPainter *painter) const;
    float calculateSpan();


private:
    vector<Disk*> _discs;
    list<Disk*> _shelf;
    vector<Disk*> _shelfNaive;
    vector<Disk*> _shelfNaiveFinal;
    priority_queue<MaxGap*, vector<MaxGap*>, compMaxGaps> _queue;
    AlgoType _algorithm;
    unsigned _n;
    bool _ended = false;
    float _naiveMinSpan;
};

#endif // COINSONSHELF_H
