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

    void generateRandomVerts(std::vector<QPointF>& tacke);

    void delaunay(std::vector<QPointF> points);
    EdgePartition triangulate(std::vector<QPointF> points);
    bool in_circle(QPointF a, QPointF b, QPointF c, QPointF d);
    bool right_of(EdgeDQ* e, QPointF p);
    bool left_of( EdgeDQ* e, QPointF p);
    EdgeDQ* make_edge(QPointF org, QPointF dest);
    void splice(EdgeDQ* a, EdgeDQ* b);
    void delete_edge(EdgeDQ* e);

    void DrawEdge(EdgeDQ* e, QPainter* p) const;
    QuadList GetVoronoi();
    QPointF Circumcenter(QPointF a, QPointF b, QPointF c);

private:
    std::vector<QPointF> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;
    /* Funkcija generise nasumicne tacke */
    std::vector<QPointF> generisiNasumicneTacke(int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA) const;
    /* Ucitane nasumicne tacke se pripremaju za pravljenje prostog poligona
     * (zadatog u smeru suprotno od kazaljke na satu) */
    std::vector<QPointF> ucitajNasumicneTacke(int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA) const;





public:
    std::vector<QPointF> tacke;
    //std::vector<EdgeDQ*> edges;
    PointsList vertices_;
    QuadList edges_;
    QuadList quads_;
    PointsPartition SplitPoints(const PointsList& points);
    EdgeDQ* MakeEdgeBetween(int a, int b, const PointsList& points);
    EdgeDQ* Connect(EdgeDQ* a, EdgeDQ* b);
    void Kill(EdgeDQ* edge);
    EdgePartition LinePrimitive(const PointsList& points);
    EdgePartition TrianglePrimitive(const PointsList& points);
    EdgeDQ* LowestCommonTangent(EdgeDQ*& left_inner, EdgeDQ*& right_inner);
    EdgeDQ* LeftCandidate(EdgeDQ* base_edge);
    EdgeDQ*	RightCandidate(EdgeDQ* base_edge);
    void MergeHulls(EdgeDQ*& base_edge);
    QuadList getTriangulation();
    bool CCW(QPointF a, QPointF b, QPointF c);
    double Det4x4(double* col_0, double* col_1, double* col_2, double* col_3);
    double Det3x3(double* col_0, double* col_1, double* col_2);
    bool Valid(EdgeDQ* e, EdgeDQ *base_edge);
    EdgeDQ* Make(std::vector<QuadEdge*>& list);
};

#endif // TRIANGULATIONDQ_H

