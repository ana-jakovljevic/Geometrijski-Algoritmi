#ifndef TRIANGULATIONDQ_H
#define TRIANGULATIONDQ_H

#include "algoritambaza.h"

#include "ga05_EdgeDQ.h"



class triangulationDQ : public AlgoritamBaza
{
public:
    triangulationDQ(QWidget *pCrtanje,
                    int pauzaKoraka,
                    const bool &naivni = false,
                    std::string imeDatoteke = "",
                    int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    void generateRandomVerts(std::vector<QPointF>& tacke);

    void delaunay(std::vector<QPointF> points);
    std::vector<EdgeDQ*> triangulate(std::vector<QPointF> points);
    bool in_circle(QPointF a, QPointF b, QPointF c, QPointF d);
    bool right_of(QPointF p, EdgeDQ* e);
    bool left_of(QPointF p, EdgeDQ* e);
    EdgeDQ* make_edge(QPointF org, QPointF dest);
    void splice(EdgeDQ* a, EdgeDQ* b);
    EdgeDQ* connect(EdgeDQ* a, EdgeDQ* b);
    void delete_edge(EdgeDQ* e);

private:
    std::vector<QPointF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;
    /* Funkcija generise nasumicne tacke */
    std::vector<QPointF> generisiNasumicneTacke(int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA) const;
    /* Ucitane nasumicne tacke se pripremaju za pravljenje prostog poligona
     * (zadatog u smeru suprotno od kazaljke na satu) */
    std::vector<QPointF> ucitajNasumicneTacke(int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA) const;





public:
    std::vector<QPointF> tacke;
    std::vector<EdgeDQ*> edges;



};

#endif // TRIANGULATIONDQ_H

