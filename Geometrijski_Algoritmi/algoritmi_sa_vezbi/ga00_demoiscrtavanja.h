#ifndef DEMOISCRTAVANJA_H
#define DEMOISCRTAVANJA_H

#include "algoritambaza.h"

class DemoIscrtavanja : public AlgoritamBaza
{
public:
    DemoIscrtavanja(QWidget *pCrtanje,
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
    unsigned int _n;
};

#endif // DEMOISCRTAVANJA_H
