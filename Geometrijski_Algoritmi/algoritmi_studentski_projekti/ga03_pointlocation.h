#ifndef GA03_POINTLOCATION_H
#define GA03_POINTLOCATION_H
#include "ga03_strukture.h"
class PointLocation : public AlgoritamBaza
{
public:
    PointLocation(QWidget *pCrtanje,
                    int pauzaKoraka,
                    const bool &naivni = false,
                    std::string imeDatoteke = "",
                    int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;
    void napraviMapu(std::vector<Duz>& segments);
    std::vector<Duz*> ucitajPodatke(std::string imeDatoteke);
    void prolaz(Cvor *node);
    Trapez * gettTrapez(){return tTrapez;}
private:
    double xt,yt;
    std::vector <Duz*> _duzi;
    std::set<Trapez*> listing;
    Trapez *tTrapez;
    TrapeznaMapa trapeznamapa;
};

#endif // GA03_POINTLOCATION_H
