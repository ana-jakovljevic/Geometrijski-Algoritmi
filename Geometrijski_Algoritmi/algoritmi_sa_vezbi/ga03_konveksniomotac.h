#ifndef GA03_KONVEKSNIOMOTAC_H
#define GA03_KONVEKSNIOMOTAC_H

#include "algoritambaza.h"

class konveksniomotac : public AlgoritamBaza
{
public:
    konveksniomotac(QWidget *pCrtanje,
                    int pauzaKoraka,
                    QCheckBox *const naivni = nullptr,
                    std::string imeDatoteke = "",
                    int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    std::vector<QPoint> getKonveksniOmotac() const;
    std::vector<QPoint> getNaivniOmotac() const;

private:
    std::vector<QPoint> _tacke;
    std::vector<QPoint> _konveksniOmotac;
    std::vector<QPoint> _naivniOmotac;
    QPoint _maxTacka;
};

#endif // GA03_KONVEKSNIOMOTAC_H
