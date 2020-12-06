#ifndef DISCRTAVANJE_H
#define DISCRTAVANJE_H

#include "algoritambaza.h"

class Discrtavanje : public AlgoritamBaza
{
public:
    Discrtavanje(QWidget *pCrtanje,
                 int pauzaKoraka,
                 QCheckBox *const naivni = nullptr,
                 std::string imeDatoteke = "",
                 int broj_tacaka = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

private:
    std::vector<QVector3D> generisiNasumicneTacke(int broj_tacaka = BROJ_NASUMICNIH_TACAKA) const;

    std::vector<QVector3D> _tacke;
    unsigned int _n;
};

#endif // DISCRTAVANJE_H
