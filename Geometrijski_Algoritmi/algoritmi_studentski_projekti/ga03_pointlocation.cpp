#include "ga03_pointlocation.h"

PointLocation::PointLocation(QWidget *pCrtanje,
                             int pauzaKoraka,
                             const bool &naivni,
                             std::string imeDatoteke,
                             int i) /* brojTacaka, odnosno nasumicno ne koristimo u ovom primeru */
              :AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    std::string pom = "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/";
    if (imeDatoteke == "")
        imeDatoteke = pom+ "input"+std::to_string(i)+".txt";
    _duzi.erase(_duzi.begin(),_duzi.end());
    _duzi=ucitajPodatke(imeDatoteke);
    trapeznamapa = TrapeznaMapa();
    trapeznamapa.novaMapa(_duzi);
    prolaz(trapeznamapa.koren);
    tTrapez=nullptr;
}

std::vector<Duz *> PointLocation::ucitajPodatke(std::string imeDatoteke)
{
    std::ifstream ulaz(imeDatoteke);
    unsigned n;
    ulaz >> n;
    std::vector<Duz*> duzi;
    for (unsigned i = 0; i < n; i++) {
        double x,y,x1,y1;
        ulaz >> x >> y >> x1 >> y1;
        QPointF t1(x,y);
        QPointF t2(x1,y1);
        Duz *d=new Duz(t1,t2);
        duzi.push_back(d);
    }
    ulaz>>xt>>yt;
    return duzi;
}
void PointLocation::prolaz(Cvor *c) {
    if(c==nullptr) return;
    if(c->procitajImeCvora()=="List"){
         listing.emplace(c->trapezCvora());
         return;
    }
    if (c->levi != nullptr)
        prolaz(c->levi);

    if (c->desni != nullptr)
        prolaz(c->desni);
    return;
}

void PointLocation::pokreniAlgoritam()
{
    tTrapez = trapeznamapa.pronadjiTrapez(QPointF(xt, yt));
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void PointLocation::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;
    QPen p = painter->pen();

    p.setWidth(3);
    p.setCapStyle(Qt::RoundCap);
    p.setColor(Qt::blue);
    painter->setPen(p);
    std::vector<QPointF> v;
    for(Trapez* t:listing){
        v=t->niztacaka();
        painter->drawLine(v[0],v[2]);
        painter->drawLine(v[1],v[3]);
    }
    p.setColor(Qt::black);
    painter->setPen(p);
    for(unsigned long i=0;i<_duzi.size();i++)
        painter->drawLine(_duzi[i]->leviKraj, _duzi[i]->desniKraj);
    if(tTrapez!=nullptr){
            p.setColor(Qt::red);
            painter->setPen(p);
            painter->drawEllipse(static_cast<int>(xt),
                                 static_cast<int>(yt),
                                 9, 9);
            v=tTrapez->niztacaka();
            painter->drawLine(v[0],v[2]);
            painter->drawLine(v[1],v[3]);
            painter->drawLine(v[2],v[3]);
            painter->drawLine(v[0],v[1]);
        }
}

void PointLocation::pokreniNaivniAlgoritam()
{
    for(Trapez* t:listing){
        if(xt>t->tleviKraj.x()&&xt<t->tdesniKraj.x()
                &&t->dDuz->iznadDuzi(QPointF(xt,yt))==1
                &&t->gDuz->iznadDuzi(QPointF(xt,yt))==-1)
            tTrapez=t;

    }
    emit animacijaZavrsila();
}

void PointLocation::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;
    QPen p = painter->pen();
    p.setWidth(3);
    p.setCapStyle(Qt::RoundCap);
    p.setColor(Qt::blue);
    painter->setPen(p);
    std::vector<QPointF> v;
    for(Trapez* t:listing){
        v=t->niztacaka();
        painter->drawLine(v[0],v[2]);
        painter->drawLine(v[1],v[3]);
    }
    p.setColor(Qt::black);
    painter->setPen(p);
    for(unsigned long i=0;i<_duzi.size();i++)
        painter->drawLine(_duzi[i]->leviKraj, _duzi[i]->desniKraj);
    if(tTrapez!=nullptr){
            p.setColor(Qt::red);
            painter->setPen(p);
            painter->drawEllipse(static_cast<int>(xt),
                                 static_cast<int>(yt),
                                 9, 9);
            v=tTrapez->niztacaka();
            painter->drawLine(v[0],v[2]);
            painter->drawLine(v[1],v[3]);
            painter->drawLine(v[2],v[3]);
            painter->drawLine(v[0],v[1]);
        }
}
