#ifndef GA06_PRESEKPRAVOUGAONIKA_H
#define GA06_PRESEKPRAVOUGAONIKA_H

#include "algoritambaza.h"

class PresekPravougaonika : public AlgoritamBaza
{
public:
    PresekPravougaonika(QWidget *, int, std::string = "",
                        int = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam();
    void crtajAlgoritam(QPainter *) const;
    void pokreniNaivniAlgoritam();

private:
    void generisiNasumicnePravougaonike(int);
    void ucitajPodatkeIzDatoteke(std::string);

    QRect** _pravougaonici;
    unsigned int _n;
};

#endif // GA06_PRESEKPRAVOUGAONIKA_H
