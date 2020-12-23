#ifndef COINSONSHELF_H
#define COINSONSHELF_H

#include "algoritambaza.h"
#include <QRandomGenerator>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <set>
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

private:
    double _radius;
    QRandomGenerator _randomGen = QRandomGenerator::securelySeeded();
};

class compClass {
public:
    bool operator() (Disk *d1, Disk *d2) const {
        return d1->radius() >= d2->radius();
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

private:
    vector<Disk*> _discs;
    list<Disk*> _shelf;
    AlgoType _algorithm;
    unsigned _n;
};

#endif // COINSONSHELF_H
