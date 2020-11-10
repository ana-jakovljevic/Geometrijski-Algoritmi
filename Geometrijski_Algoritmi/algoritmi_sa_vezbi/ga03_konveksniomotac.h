#ifndef GA03_KONVEKSNIOMOTAC_H
#define GA03_KONVEKSNIOMOTAC_H

#include "algoritambaza.h"

class konveksniomotac : public AlgoritamBaza
{
public:
    konveksniomotac(QWidget *pCrtanje,
                    int pauzaKoraka,
                    std::string imeDatoteke = "",
                    int broj_tacaka = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam();
    void crtajAlgoritam(QPainter *painter) const;
    void pokreniNaivniAlgoritam();

private:
    std::vector<QPoint> _tacke;
    std::vector<QPoint> konveksni_omotac;
    QPoint maks_tacka;
};

#endif // GA03_KONVEKSNIOMOTAC_H
