#ifndef COINSONSHELF_H
#define COINSONSHELF_H

#include "algoritambaza.h"
#include <QRandomGenerator>
#include <set>

using namespace std;

class Disk {
public:
    Disk();
    double size();

private:
    double _size;
    QRandomGenerator _randomGen = QRandomGenerator::securelySeeded();
};

class compClass {
public:
    bool operator() (Disk* d1, Disk *d2) {
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
    vector<Disk*> _diskovi;
    multiset<Disk*, compClass> _diskoviSort;
};

#endif // COINSONSHELF_H
