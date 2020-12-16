#ifndef COINSONSHELF_H
#define COINSONSHELF_H

#include "algoritambaza.h"
#include <QRandomGenerator>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <set>

using namespace std;

enum AlgoType {
    SPECIAL,
    GENERAL
};

class Disk {
public:
    Disk();
    double size();
    void setSize(double newSize);

private:
    double _size;
    QRandomGenerator _randomGen = QRandomGenerator::securelySeeded();
};

class compClass {
public:
    bool operator() (Disk *d1, Disk *d2) const {
        return d1->size() >= d2->size();
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
    vector<Disk*> _discs;
    vector<Disk*> _shelf;
    AlgoType _algorithm;
    unsigned _i = 0;
    unsigned _n;
};

#endif // COINSONSHELF_H
