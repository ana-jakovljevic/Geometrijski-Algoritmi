#ifndef GA03_KONVEKSNIOMOTAC_H
#define GA03_KONVEKSNIOMOTAC_H

#include "algoritambaza.h"

class konveksniomotac : public AlgoritamBaza
{
public:
    konveksniomotac(QWidget *pCrtanje,
                    int pauzaKoraka,
                    const bool &naivni = false,
                    std::string imeDatoteke = "",
                    int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    std::vector<QPoint> getKonveksniOmotac() const;
    std::vector<QLine> getNaivniOmotac() const;

private:
    std::vector<QPoint> _tacke;
    std::vector<QPoint> _konveksniOmotac;
    QPoint _maxTacka;

    unsigned long _i;
    unsigned long _j;
    unsigned long _k;
    int _povrsina;
    std::vector<QLine> _naivniOmotac;
};

#endif // GA03_KONVEKSNIOMOTAC_H
