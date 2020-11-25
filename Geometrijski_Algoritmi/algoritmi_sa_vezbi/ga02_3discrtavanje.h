#ifndef DISCRTAVANJE_H
#define DISCRTAVANJE_H

#include "algoritambaza.h"

class Discrtavanje : public AlgoritamBaza
{
public:
    Discrtavanje(QWidget *pCrtanje,
                 int pauzaKoraka,
                 std::string imeDatoteke = "",
                 int broj_tacaka = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam();
    void crtajAlgoritam(QPainter *painter) const;
    void pokreniNaivniAlgoritam();

private:
    std::vector<QVector3D> generisiNasumicneTacke(int broj_tacaka = BROJ_NASUMICNIH_TACAKA) const;

    std::vector<QVector3D> _tacke;
    unsigned int _n;
};

#endif // DISCRTAVANJE_H
