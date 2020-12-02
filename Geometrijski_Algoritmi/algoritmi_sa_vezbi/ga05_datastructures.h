#ifndef GA05_DATASTRUCTURES_H
#define GA05_DATASTRUCTURES_H

#include <QPointF>
#include <QLineF>

#include "pomocnefunkcije.h"

enum class tipDogadjaja {
    POCETAK_DUZI,
    KRAJ_DUZI,
    PRESEK
};

struct tackaDogadjaja {

    QPointF tacka;
    tipDogadjaja tip;
    QLineF* duz1;
    QLineF* duz2;
};

struct poredjenjeDogadjaja {

    bool operator()(const tackaDogadjaja &prva, const tackaDogadjaja &druga) const {

        return (prva.tacka.y() > druga.tacka.y()) ||
               (fabs(prva.tacka.y() - druga.tacka.y()) < EPSd && prva.tacka.x() < druga.tacka.x());
    }
};

struct poredjenjeDuzi {

    double *yBrisucaPrava;

    poredjenjeDuzi(double *y)
        :yBrisucaPrava(y)
    {}

    bool operator()(const QLineF* duz1, const QLineF* duz2) const {
        QLineF brisucaPrava = QLineF(0, *yBrisucaPrava-0.5, 10, *yBrisucaPrava-0.5);

        QPointF presek1;
        QPointF presek2;
        pomocneFunkcije::presekDuzi(*duz1, brisucaPrava, &presek1);
        pomocneFunkcije::presekDuzi(*duz2, brisucaPrava, &presek2);

        return presek1.x() < presek2.x();

    }

};

#endif // GA05_DATASTRUCTURES_H
