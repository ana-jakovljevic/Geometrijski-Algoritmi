#include <fstream>
#include <iostream>

#include "algoritambaza.h"
#include "ga05_triangulationdq.h"
#include "pomocnefunkcije.h"
#include "ga05_pomocneFunkcije.h"

triangulationDQ::triangulationDQ(QWidget *pCrtanje,
                                 int pauzaKoraka,
                                 const bool &naivni,
                                 std::string imeDatoteke,
                                 int brojTacaka) : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{

        /* Ucitavanje tacka */
        if (imeDatoteke != "")
            vertices_ = ucitajPodatkeIzDatoteke(imeDatoteke);
        else
            vertices_ = ucitajNasumicneTacke(brojTacaka);
      pripremiTacke(vertices_);
      edges_ = QuadList();

}
//citanje podataka
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
    return randomPoints;
}
std::vector<QPointF> triangulationDQ::pripremiTacke(std::vector <QPointF>& randomPoints){
    // sort points by x coordinate
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
    // remove unique points
    randomPoints.erase(std::unique(randomPoints.begin(),randomPoints.end()), randomPoints.end());

    return randomPoints;
}
//pokretanje i crtanje algoritma
void triangulationDQ::pokreniAlgoritam(){
    edges_.clear();
    AlgoritamBaza_updateCanvasAndBlock()
    if(vertices_.size() <= 1){
        return;
    }
    triangulate(vertices_);

    AlgoritamBaza_updateCanvasAndBlock()

    emit animacijaZavrsila();
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
      /*  std::cout << (*i)->edges[0];
        std::cout << (*i)->edges[1];
        std::cout << (*i)->edges[2];
        std::cout << (*i)->edges[3];*/

        if((*i)->edges[0].draw)
            painter->drawLine((*i)->edges->origin(),(*i)->edges->destination());
    }


}
void triangulationDQ::pokreniNaivniAlgoritam(){
    edges_naive.clear();
    auto size = vertices_.size();
    for(auto i=0u; i<size; i++){
        for(auto j=i+1; j<size; j++){
            for(auto k=j+1; k<size; k++){
                bool flag = true;
                for(auto r=0u; r<size; r++){
                    if(i==r || j==r || k==r){
                        if(in_circle(vertices_[i],vertices_[j], vertices_[k], vertices_[r])){
                            flag = false;
                            break;
                        }
                    }
                }
                if(flag){
                    edges_naive.push_back(new EdgeDQ{vertices_[i],vertices_[j]});
                    edges_naive.push_back(new EdgeDQ{vertices_[i],vertices_[k]});
                    edges_naive.push_back(new EdgeDQ{vertices_[k],vertices_[j]});

                }
            }
        }
    }
    AlgoritamBaza_updateCanvasAndBlock()

    emit animacijaZavrsila();
}
void triangulationDQ::crtajNaivniAlgoritam(QPainter *painter) const{
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

    for(auto i=edges_naive.begin(); i != edges_naive.end(); ++i){
        painter->setPen(magneta);
        painter->drawLine((*i)->origin(),(*i)->_dest);
    }


}

EdgePartition triangulationDQ::triangulate(std::vector<QPointF> points){

    if(points.size() == 2){
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

//funkcije za triangulate
EdgePartition triangulationDQ::LinePrimitive(const PointsList &points){
    EdgeDQ * e = MakeEdgeBetween(0,1,points);
    EdgeDQ * e_sym = e->Sym();
    return EdgePartition({e}, {e_sym});
}
EdgePartition triangulationDQ::TrianglePrimitive(const PointsList &points){
    EdgeDQ* a = MakeEdgeBetween(0, 1, points);
    EdgeDQ* b = MakeEdgeBetween(1, 2, points);

    // Do the splice thing
    splice(a->Sym(), b);

    // We want a consistent face orientation, so determine which way we're going here
    if (CCW(points[0], points[1], points[2]))
    {
        Connect(b, a);
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
PointsPartition triangulationDQ::SplitPoints(const PointsList &points){

    long halfway = (points.size() / 2);


    PointsList left(points.begin(), points.begin() + halfway);
    PointsList right(points.begin() + halfway, points.end());

    return PointsPartition(left, right);
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
void triangulationDQ::MergeHulls(EdgeDQ *&base_edge){
    while (true)
    {
        // Get our candidate edges
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
            AlgoritamBaza_updateCanvasAndBlock()


        }
        else
        {
            // If we can't do that, then the left edge must be valid and we connect it to the base and set the new base edge
            base_edge = Connect(base_edge->Sym(), left_candidate->Sym());
            AlgoritamBaza_updateCanvasAndBlock()



        }
        AlgoritamBaza_updateCanvasAndBlock()

    }
}


//funkcije za manipulaciju granam
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

}
void triangulationDQ::Kill(EdgeDQ *edge){
    splice(edge, edge->Oprev());
    splice(edge->Sym(), edge->Sym()->Oprev());

    // Free the quad edge that the edge belongs toW
    QuadEdge* raw = reinterpret_cast<QuadEdge*>(edge - (edge->index()));
    edges_.erase(std::remove(edges_.begin(), edges_.end(), raw));
    delete raw;
    AlgoritamBaza_updateCanvasAndBlock()
}
EdgeDQ* triangulationDQ::MakeEdgeBetween(unsigned long a, unsigned long b, const PointsList &points){
 EdgeDQ* e = Make(edges_);

 e->setOrigin(points[a]);
 e->setDestination(points[b]);

 return e;
}
EdgeDQ* triangulationDQ::Connect(EdgeDQ* a, EdgeDQ* b){
    EdgeDQ* e = Make(edges_);

    e->setOrigin(a->destination());
    // Set it to end at the beginning of,this is giving it a coherent orientation
    e->setDestination(b->origin());
    // Perform splice operations
    splice(e, a->Lnext());
    splice(e->Sym(), b);

    // Return a pointer to our new edge
    return e;
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
    // Picks out a candidate edge from the right half of the domain
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

EdgeDQ* triangulationDQ::Make(std::vector<QuadEdge*>& list){
   list.push_back(new QuadEdge());
   return list.back()->edges;
}

PointsList triangulationDQ::getVertices() const{
    return vertices_;
}
QuadList triangulationDQ::getEdges() const{
    return edges_;
}

bool triangulationDQ::isSorted()  {
    for(auto i=0u; i<(vertices_.size()-1); i++){
        if(vertices_[i].x() > vertices_[i+1].x()){
                return false;
        }
        if(pomocneFunkcije::bliski(vertices_[i].x(), vertices_[i+1].x())){
            if(vertices_[i].y() >  vertices_[i+1].y()){
                return false;
            }
        }
    }
    return true;
}
bool triangulationDQ::checkSym(){
    for(auto it=edges_.begin(); it!=edges_.end(); it++){
        // check if the third edge is Symetric to the first
        if((*it)->edges[0].Sym()->Sym() != (*it)->edges[2].Sym()){
            return false;
        }
        if(!(*it)->edges[0].draw)
            return false;
    }
    return true;
}
