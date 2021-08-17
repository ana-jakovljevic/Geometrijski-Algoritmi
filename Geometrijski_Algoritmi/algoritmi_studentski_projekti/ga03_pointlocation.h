#ifndef GA03_POINTLOCATION_H
#define GA03_POINTLOCATION_H
#include "algoritambaza.h"

class ga03_pointlocation : public AlgoritamBaza
{
public:
    ga03_pointlocation(QWidget *pCrtanje,
                        int pauzaKoraka,
                        const bool &naivni = false,
                        std::string imeDatoteke = "",
                        int brojDuzi = BROJ_SLUCAJNIH_OBJEKATA);
    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *) const final;

private:
    void generisiNasumicnuMapu(int);
    std::vector<QPoint> _tacke;
    int velicina;
};

#endif // GA03_POINTLOCATION_H
