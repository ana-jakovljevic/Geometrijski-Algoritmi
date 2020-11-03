#ifndef DEMOISCRTAVANJA_H
#define DEMOISCRTAVANJA_H

#include "algoritambaza.h"

class DemoIscrtavanja : public AlgoritamBaza
{
public:
    DemoIscrtavanja(QWidget *pCrtanje,
                    int pauzaKoraka,
                    std::string imeDatoteke = "",
                    int broj_tacaka = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam();
    void crtajAlgoritam(QPainter *painter) const;
    void pokreniNaivniAlgoritam();

private:
    std::vector<QPoint> _tacke;
    unsigned int _n;
};

#endif // DEMOISCRTAVANJA_H
