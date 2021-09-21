#ifndef EDGEDQ_H
#define EDGEDQ_H
#include "algoritambaza.h"
#include <iostream>

class QuadEdge;

class EdgeDQ{
private:
    QPointF _org;
    int index_;
    EdgeDQ* _next;
    friend QuadEdge;

public:

    bool draw = false;

    EdgeDQ(){}
    ~EdgeDQ(){}
    EdgeDQ(QPointF org) : _org(org){}

    void setNext(EdgeDQ* next){
        _next = next;
    }
    QPointF origin() {return _org;}
    QPointF destination() {return Sym()->origin();}

    EdgeDQ* Rot(){return (index_ < 3) ? (this + 1) : (this - 3);}
    EdgeDQ* InvRot(){return (index_ > 0) ? (this - 1) : (this + 3);}
    EdgeDQ* Sym(){	return (index_ < 2) ? (this + 2) : (this - 2);}
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
    int index() {return index_;
                }
  /*  friend std::ostream& operator<<(std::ostream& out, const EdgeDQ& edge) {
        out << edge.origin() <<
        return out;
    }*/
    //static EdgeDQ* Make(std::vector<QuadEdge*>& list);


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


#endif // EDGEDQ_H
