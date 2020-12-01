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

    std::vector<QPoint> get_konveksni_omotac() const;
    std::vector<QPoint> get_naivni_konveksni_omotac() const;

private:
    std::vector<QPoint> _tacke;
    std::vector<QPoint> konveksni_omotac;
    std::vector<QPoint> naivni_konveksni_omotac;
    QPoint maks_tacka;
};

#endif // GA03_KONVEKSNIOMOTAC_H
