#ifndef BRISUCAPRAVA_H
#define BRISUCAPRAVA_H

#include "algoritambaza.h"

class BrisucaPrava : public AlgoritamBaza
{
public:
    BrisucaPrava(QWidget *pCrtanje,
                 int pauzaKoraka,
                 QCheckBox *const naivni = nullptr,
                 std::string imeDatoteke = "",
                 int broj_tacaka = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

private:
    std::vector<QPoint> _tacke;
    int _yPoz;
};

#endif // BRISUCAPRAVA_H
