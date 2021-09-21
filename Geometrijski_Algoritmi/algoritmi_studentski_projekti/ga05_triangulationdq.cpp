#include <fstream>
#include <iostream>

#include "algoritambaza.h"
#include "ga05_triangulationdq.h"


triangulationDQ::triangulationDQ(QWidget *pCrtanje,
                                 int pauzaKoraka,
                                 const bool &naivni,
                                 std::string imeDatoteke,
                                 int brojTacaka) : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{

        /* Ucitavanje tacka */
        if (imeDatoteke != "")
            tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
        else
            tacke = ucitajNasumicneTacke(brojTacaka);

      edges_ = QuadList();
      generateRandomVerts(tacke);
      vertices_ = tacke;
/*
      for(auto& tacka : vertices_){
          std::cout << tacka.x() << " " << tacka.y() << std::endl;
      }*/

}
void triangulationDQ::pokreniAlgoritam(){
    std::cout << "Pozovi triangulate" << std::endl;
    EdgePartition tuple = triangulate(vertices_);
    std::cout << "Pozovi GetVoronoi" << std::endl;
    GetVoronoi();
    std::cout << "Velicina edges_" << std::endl;
    std::cout << edges_.size() << std::endl;
    AlgoritamBaza_updateCanvasAndBlock()

    emit animacijaZavrsila();

}
void triangulationDQ::DrawEdge(EdgeDQ *e, QPainter* p) const{
    if(e->draw==true)
        p->drawLine(e->origin(), e->destination());
}
QuadList triangulationDQ::GetVoronoi(){
    for (auto i = edges_.begin(); i != edges_.end(); i++)
        {
            EdgeDQ* e = (*i)->edges;

            // If we're not on the exterior
            if (CCW(e[0].origin(), e[0].destination(), e[0].Onext()->destination())
                && CCW(e[0].origin(), e[0].Oprev()->destination(), e[0].destination()))
            {
                e[1].setOrigin((Circumcenter(e[0].origin(), e[0].destination(), e[0].Onext()->destination())));
                e[3].setOrigin((Circumcenter(e[0].origin(), e[0].Oprev()->destination(), e[0].destination())));
            }
        }

        return edges_;
}
QPointF triangulationDQ::Circumcenter(QPointF a, QPointF b, QPointF c){
    auto d = 2* (a.x() * (b.y() - c.y()) + b.x() * (c.y() - a.y()) + c.x() * (a.y() - b.y()));
    auto a_lengthsquared = a.x()*a.x() + a.y()*a.y();
    auto b_lengthsquared = b.x()*b.x() + b.y()*b.y();
    auto c_lengthsquared = c.x()*c.x() + c.y()*c.y();

    qreal x = static_cast<qreal>(a_lengthsquared * (b.y() - c.y()) + b_lengthsquared * (c.y() - a.y()) + c_lengthsquared * (a.y() - b.y())) / d;
    qreal y = static_cast<qreal>(a_lengthsquared * (c.x() - b.x()) + b_lengthsquared * (a.x() - c.x()) + c_lengthsquared * (b.x() - a.x())) / d;
    return QPointF(x,y);
}
void triangulationDQ::crtajAlgoritam(QPainter *painter) const{
    if(!painter) return;
    QPen regular = painter->pen();
    regular.setColor(Qt::black);
    regular.setWidth(10);

    QPen magneta = painter->pen();
    magneta.setColor(Qt::magenta);
    magneta.setWidth(2);

    QPen blue = painter->pen();
    blue.setColor(Qt::blue);
    magneta.setWidth(2);


    for(const auto &pt : vertices_){
        painter->setPen(regular);
        painter->drawPoint(pt);
    }


    for(auto i=edges_.begin(); i != edges_.end(); ++i){
        painter->setPen(magneta);

        if((*i)->edges[0].draw)
            painter->drawLine((*i)->edges->origin(),(*i)->edges->destination());
    }


}


void triangulationDQ::pokreniNaivniAlgoritam(){
    emit animacijaZavrsila();
}
void triangulationDQ::crtajNaivniAlgoritam(QPainter *painter) const{
   if(!painter) return;
}
std::vector<QPointF> triangulationDQ::ucitajPodatkeIzDatoteke(std::string imeDatoteke) const{
    std::ifstream inputFile(imeDatoteke);
    std::vector<QPointF> points;
    int x, y;
    while(inputFile >> x >> y)
        points.emplace_back(x, y);
    return points;
}
std::vector<QPointF> triangulationDQ::generisiNasumicneTacke(int brojTacaka) const{
    std::vector<QPoint> randomPointsInt = AlgoritamBaza::generisiNasumicneTacke(brojTacaka);
    std::vector<QPointF> randomPoints{std::cbegin(randomPointsInt),
                                      std::cend(randomPointsInt)};
    return randomPoints;
}

std::vector<QPointF> triangulationDQ::ucitajNasumicneTacke(int brojTacaka) const{
    std::vector<QPointF> randomPoints = generisiNasumicneTacke(brojTacaka);
    std::sort(randomPoints.begin(), randomPoints.end(),[](auto& f, auto& s)->bool{
        if(f.x() < s.x()){
            return true;
        }else if(f.x() > s.x()){
            return false;
        }else{
            if(f.y() < s.y()){
                return true;
            }else{
                return false;
            }
        }
    } );
    //pomocneFunkcije::sortirajTackeZaProstPoligon(randomPoints);


    return randomPoints;
}

void triangulationDQ::generateRandomVerts(std::vector<QPointF>& tacke){
    //pomocneFunkcije::sortirajTackeZaProstPoligon(tacke);
    tacke.erase(std::unique(tacke.begin(), tacke.end()), tacke.end());

}


EdgePartition triangulationDQ::triangulate(std::vector<QPointF> points){
    if(points.size()==2){
        return LinePrimitive(points);
    }
    if (points.size() == 3)
    {
        return TrianglePrimitive(points);
    }
    PointsPartition partition = SplitPoints(points);

    EdgePartition left = triangulate(std::get<0>(partition));
    EdgePartition right = triangulate(std::get<1>(partition));

    EdgeDQ* right_inner = std::get<0>(right)[0];
    EdgeDQ* left_inner = std::get<1>(left)[0];

    EdgeDQ* left_outer = std::get<0>(left)[0];
    EdgeDQ* right_outer = std::get<1>(right)[0];

    EdgeDQ* base_edge = LowestCommonTangent(left_inner, right_inner);

    if (left_inner->origin() == left_outer->origin())
    {
        left_outer = base_edge->Sym();
    }
    if (right_inner->origin() == right_outer->origin())
    {
        right_outer = base_edge;
    }
    MergeHulls(base_edge);
    return EdgePartition({left_outer}, {right_outer});
}
bool triangulationDQ::in_circle(QPointF a, QPointF b, QPointF c, QPointF d){
    auto a_lengthsquared = a.x()*a.x()+a.y()*a.y();
    auto b_lengthsquared = b.x()*b.x()+b.y()*b.y();
    auto c_lengthsquared = c.x()*c.x()+c.y()*c.y();
    auto d_lengthsquared = d.x()*d.x()+d.y()*d.y();
    double m[4][4] = {	{ a.x(), b.x(), c.x(), d.x() },
                         { a.y(), b.y(), c.y(), d.y() },
                         { a_lengthsquared, b_lengthsquared, c_lengthsquared, d_lengthsquared},
                            { 1, 1, 1, 1 } };

        // Return true if our determinant is positive
        return Det4x4(m[0], m[1], m[2], m[3]) > 0;
}
double triangulationDQ::Det4x4(double *col_0, double *col_1, double *col_2, double *col_3){
    double a = col_0[0];
    double b = col_1[0];
    double c = col_2[0];
    double d = col_3[0];
    double e = col_0[1];
    double f = col_1[1];
    double g = col_2[1];
    double h = col_3[1];
    double i = col_0[2];
    double j = col_1[2];
    double k = col_2[2];
    double l = col_3[2];
    double m = col_0[3];
    double n = col_1[3];
    double o = col_2[3];
    double p = col_3[3];

    // Compute 3x3 determinants
    double adet = a * ((f * k * p) - (f * l * o) - (g * j * p) + (g * l * n) + (h * j * o) - (h * k * n));
    double bdet = b * ((e * k * p) - (e * l * o) - (g * i * p) + (g * l * m) + (h * i * o) - (h * k * m));
    double cdet = c * ((e * j * p) - (e * l * n) - (f * i * p) + (f * l * m) + (h * i * n) - (h * j * m));
    double ddet = d * ((e * j * o) - (e * k * n) - (f * i * o) + (f * k * m) + (g * i * n) - (g * j * m));

    // Return their alternating sum
    double det = adet - bdet + cdet - ddet;
    return det;
}
double triangulationDQ::Det3x3(double* col_0, double* col_1, double* col_2)
{
    // Gets the determinant of a 3x3 matrix, where the arguments are 3-long column vectors

    // Names all the objects in the matrix for my convenience
    double a = col_0[0];
    double b = col_1[0];
    double c = col_2[0];
    double d = col_0[1];
    double e = col_1[1];
    double f = col_2[1];
    double g = col_0[2];
    double h = col_1[2];
    double i = col_2[2];

    // Return the alternating sum of the 2x2 determinants of the coproducts
    double det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    return det;
}

bool triangulationDQ::right_of(EdgeDQ* e, QPointF p){
   return CCW(p, e->destination(), e->origin());
}
bool triangulationDQ::CCW(QPointF a, QPointF b, QPointF c){
    float a_x = a.x();
    float a_y = a.y();
    float b_x = b.x();
    float b_y = b.y();
    float c_x = c.x();
    float c_y = c.y();

    // Set up a matrix
    double m[3][3] = { { a_x, b_x, c_x }, { a_y, b_y, c_y }, { 1, 1, 1 } };

    // Return true if our determinant is positive
    return Det3x3(m[0], m[1], m[2]) > 0;
}
bool triangulationDQ::left_of( EdgeDQ* e, QPointF p ){
    return CCW(p, e->origin(), e->destination());
}

void triangulationDQ::splice(EdgeDQ* a, EdgeDQ* b){
    EdgeDQ* alpha = a->Onext()->Rot();
    EdgeDQ* beta = b->Onext()->Rot();

    EdgeDQ* t1 = b->Onext();
    EdgeDQ* t2 = a->Onext();
    EdgeDQ* t3 = beta->Onext();
    EdgeDQ* t4 = alpha->Onext();

    a->setNext(t1);
    b->setNext(t2);
    alpha->setNext(t3);
    beta->setNext(t4);

    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void triangulationDQ::Kill(EdgeDQ *edge){
    splice(edge, edge->Oprev());
    splice(edge->Sym(), edge->Sym()->Oprev());

   // Free the quad edge that the edge belongs to
    QuadEdge* raw = (QuadEdge*)(edge - (edge->index()));
    edges_.erase(std::remove(edges_.begin(), edges_.end(), raw));
    delete raw;
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}
PointsPartition triangulationDQ::SplitPoints(const PointsList &points){
    auto halfway = (points.size() / 2);

    PointsList left(points.begin(), points.begin() + halfway);
    PointsList right(points.begin() + halfway, points.end());

    return PointsPartition(left, right);
}
EdgeDQ* triangulationDQ::MakeEdgeBetween(int a, int b, const PointsList &points){
 EdgeDQ* e = Make(edges_);

 e->setOrigin(points[a]);
 e->setDestination(points[b]);
 return e;
}

EdgeDQ* triangulationDQ::Connect(EdgeDQ* a, EdgeDQ* b){
    EdgeDQ* e = Make(edges_);

    e->setOrigin(a->destination());
    // Set it to end at the beginning of a, thus giving it a coherent orientation
    e->setDestination(b->origin());
    // Perform splice operations -- I'm still not quite sure why
    splice(e, a->Lnext());
    splice(e->Sym(), b);

    // Return a pointer to our new edge
    return e;
}
EdgePartition triangulationDQ::LinePrimitive(const PointsList &points){
    EdgeDQ * e = MakeEdgeBetween(0,1,points);
    EdgeDQ * e_sym = e->Sym();
    return EdgePartition({e}, {e_sym});
}
EdgePartition triangulationDQ::TrianglePrimitive(const PointsList &points){
    EdgeDQ* a = MakeEdgeBetween(0, 1, points);
    EdgeDQ* b = MakeEdgeBetween(1, 2, points);

    // Do the splice thing; I'm not sure why
    splice(a->Sym(), b);

    // We want a consistent face orientation, so determine which way we're going here
    if (CCW(points[0], points[1], points[2]))
    {
        EdgeDQ* c = Connect(b, a);
        return EdgePartition({ a }, { b->Sym() });
    }
    else if (CCW(points[0], points[2], points[1]))
    {
        EdgeDQ* c = Connect(b, a);
        return EdgePartition({ c->Sym() }, { c });
    }
    else
    {
        // The points are collinear
        return EdgePartition({ a }, { b->Sym() });
    }
}
EdgeDQ* triangulationDQ::LowestCommonTangent(EdgeDQ *&left_inner, EdgeDQ *&right_inner){
        while (true)
        {
            if (left_of(left_inner,right_inner->origin()))
            {
                left_inner = left_inner->Lnext();
            }
            else if (right_of(right_inner,left_inner->origin()))
            {
                right_inner = right_inner->Rprev();
            }
            else
            {
                break;
            }
        }

        // Create the base edge once we hit the bottom
        EdgeDQ* base_edge = Connect(right_inner->Sym(), left_inner);
        return base_edge;
}

EdgeDQ* triangulationDQ::LeftCandidate(EdgeDQ *base_edge){
    EdgeDQ* left_candidate = base_edge->Sym()->Onext();

    if (Valid(left_candidate, base_edge))
    {
        while (in_circle(base_edge->destination(), base_edge->origin(),
                         left_candidate->destination(),
                         left_candidate->Onext()->destination()))
        {
            EdgeDQ* t = left_candidate->Onext();
            Kill(left_candidate);
            left_candidate = t;
        }
    }

    return left_candidate;
}
EdgeDQ* triangulationDQ::RightCandidate(EdgeDQ* base_edge)
{
    // Picks out a "candidate" edge from the right half of the domain
    EdgeDQ* right_candidate = base_edge->Oprev();

    if (Valid(right_candidate, base_edge))
    {
        while (in_circle(base_edge->destination(), base_edge->origin(),
                         right_candidate->destination(),
                         right_candidate->Oprev()->destination()))
        {
            EdgeDQ* t = right_candidate->Oprev();
            Kill(right_candidate);
            right_candidate = t;
        }
    }

    return right_candidate;
}
void triangulationDQ::MergeHulls(EdgeDQ *&base_edge){
    while (true)
    {
        // Get our candidate edges (really becaue we care about their vertices)
        EdgeDQ* left_candidate = LeftCandidate(base_edge);
        EdgeDQ* right_candidate = RightCandidate(base_edge);

        if (!Valid(left_candidate, base_edge) &&
            !Valid(right_candidate, base_edge))
        {
            // If neither is valid, we have nothing more to do because we've reached the top
            break;
        }
        else if (	!Valid(left_candidate, base_edge) ||
                    in_circle(left_candidate->destination(), left_candidate->origin(), right_candidate->origin(), right_candidate->destination()))
        {
            // Otherwise, if we can rule out the left guy, connect the right edge to the base and set the new base edge
            // This ruling out comes either from creating an invalid hypothetical triangle or from being beneath the base edge
            base_edge = Connect(right_candidate, base_edge->Sym());
        }
        else
        {
            // If we can't do that, then the left edge must be valid and we connect it to the base and set the new base edge
            base_edge = Connect(base_edge->Sym(), left_candidate->Sym());
        }
    }
}
bool triangulationDQ::Valid(EdgeDQ *e, EdgeDQ *base_edge){
    return right_of(base_edge,e->destination());
}
EdgeDQ* triangulationDQ::Make(std::vector<QuadEdge*>& list){
   //return new EdgeDQ();
   list.push_back(new QuadEdge());
   return list.back()->edges;
}

