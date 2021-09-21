#include <fstream>
#include <iostream>
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


      generateRandomVerts(tacke);
      for(auto& tacka : tacke){
          std::cout << tacka.x() << " " << tacka.y() << std::endl;
      }

}
void triangulationDQ::pokreniAlgoritam(){
    delaunay(tacke);

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


    for(const auto &pt : tacke){
        painter->setPen(regular);
        painter->drawPoint(pt);
    }

    for(const auto &edge : edges){
        painter->setPen(magneta);
        if(edge->data == false)
         painter->drawLine(edge->_org, edge->_dest);

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

void triangulationDQ::delaunay(std::vector<QPointF> points){
    if(points.size() < 2){
        return;
    }
    edges.clear();

    triangulate(points);

   /* for(auto e : edges){
        std::cout << e;
        std::cout << e.sym;
        std::cout << e.onext;
        std::cout << e.oprev;
    }*/

    //std::remove_if(edges.begin(), edges.end(), [](const auto &i){return i->data==false;});

}
std::vector<EdgeDQ*> triangulationDQ::triangulate(std::vector<QPointF> points){
    if(points.size() == 2){
        EdgeDQ* a = make_edge(points[0], points[1]);
        std::vector<EdgeDQ*> v;
        v.push_back(a);
        v.push_back(a->sym);
        return v;
    }
    if(points.size() == 3){
        auto p1 = points[0], p2 = points[1], p3 =  points[2];
        EdgeDQ *a = make_edge(p1, p2);
        EdgeDQ* b = make_edge(p2, p3);
        splice(a->sym, b);
        if(right_of(p3, a)){
            connect(b,a);
            std::vector<EdgeDQ*> v;
            v.push_back(a);
            v.push_back(b->sym);
            return v;
        }else if(left_of(p3, a)){
            EdgeDQ* c = connect(b,a);
            std::vector<EdgeDQ*> v;
            v.push_back(c->sym);
            v.push_back(c);
            return v;
        }else{
            std::vector<EdgeDQ*> v;
            v.push_back(a);
            v.push_back(b->sym);
            return v;
        }
    }else{
        unsigned long m = (points.size()+1) / 2;
        std::vector<QPointF> L(points.begin(), points.begin()+m);
        std::cout << "L" << std::endl;
        for(auto& tacka : L){
            std::cout << tacka.x() << " " << tacka.y() << std::endl;
        }

        std::cout << "R" << std::endl;

        std::vector<QPointF> R(points.begin()+m, points.end());
        for(auto& tacka : R){
            std::cout << tacka.x() << " " << tacka.y() << std::endl;
        }

        std::vector<EdgeDQ*> ld = triangulate(L);
        std::vector<EdgeDQ*> rd = triangulate(R);
        EdgeDQ *ldo = ld[0], *ldi = ld[1];
        EdgeDQ *rdi = rd[0], *rdo = rd[1];

        while(true){
            if(right_of(rdi->_org, ldi)){
                ldi = ldi->sym->onext;
            }else if(left_of(ldi->_org, rdi)){
                rdi = rdi->sym->oprev;
            }else{
                break;
            }
        }
        EdgeDQ* base = connect(ldi->sym,rdi);
        if((ldi->_org.x() == ldo->_org.x()) && ldi->_org.y() == ldo->_org.y()){
            ldo = base;
        }
        if(rdi->_org.x() == rdo->_org.x() && rdi->_org.y() == rdo->_org.y()){
            rdo = base->sym;
        }
        // merge
        while(true){
            EdgeDQ *rcand = base->sym->onext;
            EdgeDQ *lcand = base->oprev;
            bool v_rcand = right_of(rcand->_dest, base);
            bool v_lcand = right_of(lcand->_dest, base);
            if(! (v_rcand || v_lcand)){
                break;
            }
            if(v_rcand){
                while(right_of(rcand->onext->_dest, base) && in_circle(base->_dest, base->_org, rcand->_dest, rcand->onext->_dest)){
                    EdgeDQ* t = rcand->onext;
                    delete_edge(rcand);
                    rcand = t;
                }
            }
            if(v_lcand){
                while(right_of(lcand->oprev->_dest, base) && in_circle(base->_dest, base->_org, lcand->_dest, lcand->oprev->_dest)){
                    EdgeDQ* t = lcand->oprev;
                    delete_edge(lcand);
                    lcand = t;
                }
            }
            if(!v_rcand || (v_lcand && in_circle(rcand->_dest, rcand->_org, lcand->_org, lcand->_dest))){
                base = connect(lcand, base->sym);
            }else{
                base = connect(base->sym, rcand->sym);
            }
        }
        std::vector<EdgeDQ*> v;
        v.push_back(ldo);
        v.push_back(rdo);
        return v;
    }


}
bool triangulationDQ::in_circle(QPointF a, QPointF b, QPointF c, QPointF d){
    auto a1 = a.x()-d.x();
    auto a2 = a.y() - d.y();
    auto b1 = b.x() - d.x();
    auto b2 = b.y() - d.y();
    auto c1 = c.x() - d.x();
    auto c2 = c.y() - d.y();
    auto a3 = a1 * a1 + a2 * a2;
    auto b3 = b1 * b1 + b2 * b2;
    auto c3 = c1 * c1 + c2 * c2;
    auto det = a1*b2*c3 + a2*b3*c1 + a3*b1*c2 - (a3*b2*c1 + a1*b3*c2 + a2*b1*c3);
    return det < 0;
}
bool triangulationDQ::right_of(QPointF p, EdgeDQ* e){
    QPointF a = e->_org;
    QPointF b = e->_dest;
    auto det = (a.x()-p.x())*(b.y()-p.y()) - (a.y()-p.y())*(b.x()-p.x());
    return det > 0;
}
bool triangulationDQ::left_of(QPointF p, EdgeDQ* e){
    QPointF a = e->_org;
    QPointF b = e->_dest;
    auto det = (a.x()-p.x())*(b.y()-p.y()) - (a.y()-p.y())*(b.x()-p.x());
    return det < 0;
}
EdgeDQ* triangulationDQ::make_edge(QPointF org, QPointF dest){
    EdgeDQ *e = new EdgeDQ(org, dest);
    EdgeDQ *es = new EdgeDQ (dest, org);
    e->sym = es;
    es->sym = e;
    e->onext = e;
    e->oprev = e;
    es->onext = es;
    es->oprev = es;
    edges.push_back(e);

    return e;

}
void triangulationDQ::splice(EdgeDQ* a, EdgeDQ* b){
    if(a == b){
        return;
    }
    a->onext->oprev = b;
    b->onext->oprev = a;
    a->onext = b->onext;
    b->onext = a->onext;
    AlgoritamBaza_updateCanvasAndBlock()

    emit animacijaZavrsila();
}
EdgeDQ* triangulationDQ::connect(EdgeDQ* a, EdgeDQ* b){
    EdgeDQ * e = make_edge(a->_dest,b->_org);
    splice(e, a->sym->oprev);
    splice(e->sym, b);
    return e;
}
void triangulationDQ::delete_edge(EdgeDQ* e){
    splice(e, e->oprev);
    splice(e->sym, e->sym->oprev);
    e->data = true;
    e->sym->data = true;
    AlgoritamBaza_updateCanvasAndBlock()

    emit animacijaZavrsila();
}
