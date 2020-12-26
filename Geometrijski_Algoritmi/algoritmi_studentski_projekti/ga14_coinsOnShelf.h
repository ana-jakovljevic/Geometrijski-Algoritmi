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

enum AlgoType {
    SPECIAL,
    GENERAL
};

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

class compDiscs {
public:
    bool operator() (Disk *d1, Disk *d2) const {
        return d1->radius() >= d2->radius();
    }
};

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

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

private:
    void specialCaseEvenDiscs();
    void specialCaseOddDiscs();
    void generalCase();

    void debugShelf();
    void updateFootprintAB(Disk* A, Disk* B, bool directionBIsRightsideA);
    void printSpan(QPainter *painter, bool naive) const;
    void printSpanFinal(QPainter *painter) const;
    float calculateSpan();

    void naiveSpecialCase();
    void naiveGeneralCase();


private:
    vector<Disk*> _discs;
    list<Disk*> _shelf;
    list<Disk*> _shelfNaive;
    vector<Disk*> _shelfNaiveFinal;
    priority_queue<MaxGap*, vector<MaxGap*>, compMaxGaps> _queue;
    AlgoType _algorithm;
    unsigned _n;
    bool _ended = false;
    float _naiveMinSpan;
};

#endif // COINSONSHELF_H
