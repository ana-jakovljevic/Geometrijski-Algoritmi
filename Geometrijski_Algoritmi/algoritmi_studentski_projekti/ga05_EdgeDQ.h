#ifndef EDGEDQ_H
#define EDGEDQ_H
#include "algoritambaza.h"
#include "pomocnefunkcije.h"

#include <iostream>

class QuadEdge;

class EdgeDQ{
private:
    QPointF _org;
    int index_;
    EdgeDQ* _next;
    friend QuadEdge;

public:
    QPointF _dest;
    bool draw = false;

    EdgeDQ(){}
    ~EdgeDQ(){}
    EdgeDQ(QPointF org) : _org(org){}
    EdgeDQ(QPointF org, QPointF dest) : _org(org), _dest(dest){}

    void setNext(EdgeDQ* next){
        _next = next;
    }
    QPointF origin() const {return _org;}
    const QPointF destination()  {
        _dest = Sym()->origin();
        return Sym()->origin();}

    EdgeDQ* Rot() {return (index_ < 3) ? (this + 1) : (this - 3);}
    EdgeDQ* InvRot(){return (index_ > 0) ? (this - 1) : (this + 3);}
    EdgeDQ* Sym() {	return (index_ < 2) ? (this + 2) : (this - 2);}
    EdgeDQ* Onext(){return _next;}

    // These guys can be derived from the four primitive operations

    EdgeDQ* Oprev(){ return Rot()->Onext()->Rot(); }
    EdgeDQ* Dnext(){ return Sym()->Onext()->Sym(); }
    EdgeDQ* Dprev(){ return InvRot()->Onext()->InvRot(); }
    EdgeDQ* Lnext(){ return InvRot()->Onext()->Rot(); }
    EdgeDQ* Lprev(){ return Onext()->Sym(); }
    EdgeDQ* Rnext(){ return Rot()->Onext()->InvRot(); }
    EdgeDQ* Rprev(){return Sym()->Onext();}

    void setIndex(int index){index_ = index;}
    void setOrigin(QPointF p){
        _org = p;
        draw=true;
    }
    void setDestination(QPointF dest)
    {
        //draw = true;
        EdgeDQ* sym = Sym();
        sym->setOrigin(dest);
    }
    int index() const {
        return index_;
    }

    //pomocne funkcije za testiranje
    const EdgeDQ getSym() {return *this->Sym();}
    friend std::ostream& operator<<(std::ostream& out, EdgeDQ& edge){
        out << "org (" <<edge.origin().x() << ", " << edge.origin().y() << ") ";
        out << "dest (" << edge.destination().x() << ", " << edge.destination().y() << ") " << std::endl;

        return out;
    }
    bool operator==(const EdgeDQ &l) const {
        return l.origin() ==this->origin() && this->_dest == l._dest;
    }

};



class QuadEdge{
public:
    EdgeDQ edges[4];
    QuadEdge(){
        // Make sure the edges know their own indices for memory magic
            edges[0].setIndex(0);
            edges[1].setIndex(1);
            edges[2].setIndex(2);
            edges[3].setIndex(3);

            // By default, don't render them
            edges[0].draw = false;
            edges[1].draw = false;
            edges[2].draw = false;
            edges[3].draw = false;

            // Set them up to point to each other
            edges[0].setNext((edges + 0));
            edges[1].setNext((edges + 3));
            edges[2].setNext((edges + 2));
            edges[3].setNext((edges + 1));
    }
};
/*
class Triangle{
private:
    QPointF pi_, pj_, pk_;
public:
    Triangle(QPointF i, QPointF j, QPointF k) : pi_(i), pj_(j), pk_(k){}
    Triangle(){}
    ~Triangle(){}
    QPointF getPi(){return pi_;}
    QPointF getPj(){return pj_;}
    QPointF getPk(){return pk_;}

    bool inTriangle(QPointF point){
        float A = area(pi_, pj_, pk_);
        float A1 = area(point, pj_, pk_);
        float A2 = area(pi_, point, pk_);
        float A3 = area(pi_, pj_, point);
        float sum = fabs(A1 + A2 + A3);
        return pomocneFunkcije::bliski(A, sum);
    }


    float area(QPointF pi, QPointF pj, QPointF pk){
        auto x1=pi.x(), y1=pi.y(), x2=pj.x(), y2=pj.y(), x3=pk.x(), y3=pk.y();
           return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
    }
    bool operator==(Triangle& t){
        return t.pi_ == pi_ && t.pj_ == pj_ && t.pk_ == pk_;
    }



};*/

#endif // EDGEDQ_H
