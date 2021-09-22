#ifndef TRIANGULATIONDQ_H
#define TRIANGULATIONDQ_H


#include "ga05_EdgeDQ.h"

typedef std::vector<EdgeDQ*> EdgeList;
typedef std::vector<QPointF> PointsList;
typedef std::vector<QuadEdge*> QuadList;
typedef std::tuple<EdgeList, EdgeList> EdgePartition;
typedef std::tuple<PointsList, PointsList>	PointsPartition;


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




private:
    /* funckije za ucitavanja tacaka */
    std::vector<QPointF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;
    /* Funkcija generise nasumicne tacke */
    std::vector<QPointF> generisiNasumicneTacke(int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA) const;
    /* sortira nasumicno odabrane tacke */
    std::vector<QPointF> ucitajNasumicneTacke(int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA) const;
    /* sortira tacke po x koordinati i eliminise duplikate */
    std::vector<QPointF> pripremiTacke(std::vector <QPointF>& points);





public:
    QuadList GetVoronoi();
    QuadList getTriangulation();

    EdgePartition triangulate(std::vector<QPointF> points);

    /*funkcije za triangulaciju */
    EdgePartition LinePrimitive(const PointsList& points);
    EdgePartition TrianglePrimitive(const PointsList& points);
    PointsPartition SplitPoints(const PointsList& points);
    EdgeDQ* LowestCommonTangent(EdgeDQ*& left_inner, EdgeDQ*& right_inner);
    void MergeHulls(EdgeDQ*& base_edge);

    /*funckije za manipulaciju grana */
    void splice(EdgeDQ* a, EdgeDQ* b);
    EdgeDQ* MakeEdgeBetween(int a, int b, const PointsList& points);
    EdgeDQ* Connect(EdgeDQ* a, EdgeDQ* b);
    void Kill(EdgeDQ* edge);
    EdgeDQ* LeftCandidate(EdgeDQ* base_edge);
    EdgeDQ*	RightCandidate(EdgeDQ* base_edge);
    EdgeDQ* Make(std::vector<QuadEdge*>& list);

public:
    //pomocne funkcije
    bool CCW(QPointF a, QPointF b, QPointF c);
    double Det4x4(double* col_0, double* col_1, double* col_2, double* col_3);
    double Det3x3(double* col_0, double* col_1, double* col_2);
    bool Valid(EdgeDQ* e, EdgeDQ *base_edge);
    QPointF Circumcenter(QPointF a, QPointF b, QPointF c);
    bool in_circle(QPointF a, QPointF b, QPointF c, QPointF d);
    bool right_of(EdgeDQ* e, QPointF p);
    bool left_of( EdgeDQ* e, QPointF p);

private:
    PointsList vertices_;
    QuadList edges_;

};

#endif // TRIANGULATIONDQ_H

