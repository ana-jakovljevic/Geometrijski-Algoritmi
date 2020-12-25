#include "ga10_unitdiskcover.h"

#include <iostream>
#include <typeinfo>

UnitDiskCover::UnitDiskCover(QWidget *pCrtanje,
                              int pauzaKoraka,
                              const bool &naivni,
                              std::string imeDatoteke,
                              int brojTacaka)
    : AlgoritamBaza(pCrtanje,pauzaKoraka,naivni)
{
    if(imeDatoteke == "")
        _tacke = generisiNasumicneTacke(brojTacaka);
    else
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);

    std::sort(_tacke.begin(),
              _tacke.end(),
              [](const QPoint& left, const QPoint& right){
                    return left.x() < right.x();
              });
}

void UnitDiskCover::pokreniAlgoritam()
{
    initialiseEventQueue();
    createCover();

    std::cout << "Broj krugova u _pokrivacu: " << _pokrivac.size() << std::endl;
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void UnitDiskCover::initialiseEventQueue()
{
    for(auto &tacka:_tacke)
    {
       _eventQueue.emplace(new EventPoint(&tacka,EventType::SiteEvent));
       _eventQueue.emplace(new EventPoint(new QPoint(tacka.x()+200,tacka.y()),EventType::DeletionEvent));
    }
}

void UnitDiskCover::createCover(){
    while (!_eventQueue.empty())
    {
        EventPoint *ep = *_eventQueue.begin();
        _eventQueue.erase(ep);
        if (ep->eventType == EventType::DeletionEvent)
        {
            //std::cout << "Delete event" << std::endl;
            _statusQueue.erase(ep);
        }
        else
        {
            bool fleg = true; // fleg za insert novog kruga u pokrivac

            auto it_next = _statusQueue.upper_bound(ep);
            if (it_next != _statusQueue.end()){
                fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_next)->point,*ep->point) > 40000);
                //std::cout << "next: " << fleg << std::endl;
                auto it_next_next = ++it_next;
                if(it_next_next != _statusQueue.end()){
                    fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_next_next)->point,*ep->point) > 40000);
                    //std::cout << "next_next: " << fleg << std::endl;
                }
            }

            if(it_next != _statusQueue.begin()){
                auto it = _statusQueue.lower_bound(ep);
                if(it != _statusQueue.begin()){
                    auto it_prev = --it;
                    fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_prev)->point,*ep->point) > 40000);
                    //std::cout << "prev: " << fleg << std::endl;
                    if(it_prev != _statusQueue.begin()){
                        auto it_prev_prev = --it_prev;
                        fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_prev_prev)->point,*ep->point) > 40000);
                        //std::cout << "prev prev: " << fleg << std::endl;
                    }
                }
            }


            if(fleg){
                _statusQueue.insert(ep);

                QPoint presek1,presek2;
                auto p = ep->point;
                presek1.setX(p->x() + 0.866025404*100); presek1.setY(p->y() - 0.5*100);
                presek2.setX(p->x() + 0.866025404*100); presek2.setY(p->y() + 0.5*100);

                _pokrivac.emplace_back(*ep->point);
                AlgoritamBaza_updateCanvasAndBlock();

                _pokrivac.emplace_back(QPoint(presek1.x(),presek1.y()-100));
                AlgoritamBaza_updateCanvasAndBlock();

                _pokrivac.emplace_back(QPoint(presek2.x(),presek2.y()+100));
                AlgoritamBaza_updateCanvasAndBlock();

                _pokrivac.emplace_back(QPoint(p->x()+ 2*sqrt(pomocneFunkcije::distanceKvadrat(*p,QLine(presek1,presek2).center())),p->y()));
                AlgoritamBaza_updateCanvasAndBlock();
            }
        }
    }
}

void UnitDiskCover::crtajAlgoritam(QPainter *painter) const
{
    if(!painter) return;

    QPen p = painter->pen();
    p.setWidth(5);

    for(const QPoint& pt: _tacke){
        p.setColor(Qt::black);
        painter->setPen(p);
        painter->drawPoint(pt);
    }

    for(const QPoint& pt: _pokrivac){
        p.setWidth(3);
        p.setColor(Qt::red);
        painter->setPen(p);
        painter->drawEllipse(pt, 100, 100);
    }
}

void UnitDiskCover::crtajNaivniAlgoritam(QPainter *painter) const
{
    if(!painter) return;

        QPen p = painter->pen();
        p.setWidth(5);

        for(const QPoint &pt: _tacke){
            p.setColor(Qt::black);
            painter->setPen(p);
            painter->drawPoint(pt);
        }

        for(const QPoint &pt: _naivni_pokrivac){
            p.setWidth(3);
            p.setColor(Qt::red);
            painter->setPen(p);
            painter->drawEllipse(pt, 100, 100);
        }
}


void UnitDiskCover::pokreniNaivniAlgoritam()
{
    QPoint presek1,presek2;

    for(QPoint p:_tacke){
        if (UnitDiskCover::rastojanje(p,_nezavisanSkup) > 200){
            _nezavisanSkup.push_back(p);

            presek1.setX(p.x() + 0.866025404*100); presek1.setY(p.y() - 0.5*100);
            presek2.setX(p.x() + 0.866025404*100); presek2.setY(p.y() + 0.5*100);

            _naivni_pokrivac.emplace_back(QPoint(p.x(),p.y()));
            AlgoritamBaza_updateCanvasAndBlock();

            _naivni_pokrivac.emplace_back(QPoint(presek1.x(),presek1.y()-100));
            AlgoritamBaza_updateCanvasAndBlock();

            _naivni_pokrivac.emplace_back(QPoint(presek2.x(),presek2.y()+100));
            AlgoritamBaza_updateCanvasAndBlock();

            _naivni_pokrivac.emplace_back(QPoint(p.x()+ 2*sqrt(pomocneFunkcije::distanceKvadrat(p,QLine(presek1,presek2).center())),p.y()));

            AlgoritamBaza_updateCanvasAndBlock();
        }
    }

    std::cout << "Broj krugova u _pokrivac: " << _naivni_pokrivac.size() << std::endl;
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}


double UnitDiskCover::rastojanje(const QPoint &p, const std::vector<QPoint> &pokrivac)
{
    double minimalno = std::numeric_limits<unsigned int>::max();
    double trenutno = 0.0;

    for(QPoint q: pokrivac)
    {
        trenutno = sqrt(pomocneFunkcije::distanceKvadrat(p,q));
        if (trenutno < minimalno)
        {
            minimalno = trenutno;
        }
    }
    return minimalno;
}

/*
Unapredjenja algoritma (finalna verzija):
Umesto da se izracunava d(p,2) i onda proverava da li je <=2 nama je potrebno
zapravo da proverimo distancu od p--. p++ p+ i p- (ako je |pp+| <= 2 || itd ...)

Radi toga uvodimo brisucu pravu koja ce ici po tackama sortiranim po x
koordinatama i koja ce popunjavati BST i event queue.

BST - SL (brisuca prava=sweep line) sece neke polukrugove.
BST sadrzi centre polukrugova poluprecnika 2
event queue -state event (p=(px,py))- -deletion event (p'=(px+2,py))-
Prolazimo kroz red i na odgovarajuci nacin dodajemo elemente i brisemo elemente iz BST.
BST - tacke sortirane nerastuce po y koordinati

while L nije prazan:
    p = prvi element iz L
    u event_queue se dodaje p i p'

while event_queue nije prazan:
    p = prvi element iz BST
    sweep_line_y = p.y
    Ako je p delete event -> obrisati p iz BST
    Ako je p site evenet:
        pronaci u BST p--,p-,p+,p++ (to su tacke koje ispunjavaju odgovarajuce uslove)
        proveriti rastojanja od te 4 tacke
        Ako su sva rastojanja > 2:
            dodati p u BST
*/

