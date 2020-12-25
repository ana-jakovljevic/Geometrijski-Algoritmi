#include "ga10_unitdiskcover.h"
#include <iostream>

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

    std::cout << "Broj krugova u _pokrivac: " << _pokrivac.size() << std::endl;
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void UnitDiskCover::initialiseEventQueue()
{
    for(auto &tacka:_tacke)
    {
        auto siteEvent = new EventPoint(&tacka,EventType::SiteEvent,nullptr);
       _eventQueue.emplace(siteEvent);
       _eventQueue.emplace(new EventPoint(new QPoint(tacka.x()+20,tacka.y()),EventType::DeletionEvent, siteEvent));
    }
}

void UnitDiskCover::createCover(){

    UnitDiskCover::pokreniNaivniAlgoritam();

    while (!_eventQueue.empty())
    {
        auto it = _eventQueue.begin();
        _eventQueue.erase(it);
        auto ep = *it;
        if (ep->eventType == EventType::DeletionEvent)
        {
            _statusQueue.erase(ep->sitePoint);
            delete ep->sitePoint;
            delete ep;
        }
        else
        {
            bool fleg = true; // fleg za insert novog kruga u pokrivac

            auto it_next = _statusQueue.upper_bound(ep);
            if (fleg && it_next != _statusQueue.end()){
                fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_next)->point,*ep->point) > 400);
                auto it_next_next = ++it_next;
                if(fleg && it_next_next != _statusQueue.end()){
                    fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_next_next)->point,*ep->point) > 400);
                }
            }

            if( fleg && it_next != _statusQueue.begin()){
                auto it = _statusQueue.lower_bound(ep);
                if(it != _statusQueue.begin()){
                    auto it_prev = --it;
                    fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_prev)->point,*ep->point) > 400);
                    if(fleg && it_prev != _statusQueue.begin()){
                        auto it_prev_prev = --it_prev;
                        fleg = fleg && (pomocneFunkcije::distanceKvadrat(*(*it_prev_prev)->point,*ep->point) > 400);
                    }
                }
            }

            if(fleg){
                _statusQueue.insert(ep);

                QPoint presek1,presek2;
                auto p = ep->point;
                presek1.setX(p->x() + 0.866025404*10); presek1.setY(p->y() - 0.5*10);
                presek2.setX(p->x() + 0.866025404*10); presek2.setY(p->y() + 0.5*10);

                _pokrivac.emplace_back(*ep->point);
                //AlgoritamBaza_updateCanvasAndBlock();

                _pokrivac.emplace_back(QPoint(presek1.x(),presek1.y()-10));
                //AlgoritamBaza_updateCanvasAndBlock();

                _pokrivac.emplace_back(QPoint(presek2.x(),presek2.y()+10));
                //AlgoritamBaza_updateCanvasAndBlock();

                _pokrivac.emplace_back(QPoint(p->x()+ 2*sqrt(pomocneFunkcije::distanceKvadrat(*p,QLine(presek1,presek2).center())),p->y()));
                //AlgoritamBaza_updateCanvasAndBlock();
            }
        }
    }
}

void UnitDiskCover::crtajAlgoritam(QPainter *painter) const
{
    if(!painter) return;

    QPen p = painter->pen();
    p.setWidth(2);

    for(const QPoint& pt: _tacke){
        p.setColor(Qt::black);
        painter->setPen(p);
        painter->drawPoint(pt);
    }

    for(const QPoint& pt: _pokrivac){
        p.setWidth(2);
        p.setColor(Qt::red);
        painter->setPen(p);
        painter->drawEllipse(pt, 10, 10);
    }
}

void UnitDiskCover::pokreniNaivniAlgoritam()
{
    QPoint presek1,presek2;
    for(QPoint p:_tacke){
        if (UnitDiskCover::rastojanje(p,_nezavisanSkup) > 400){
            _nezavisanSkup.push_back(p);

            presek1.setX(p.x() + 0.866025404*10); presek1.setY(p.y() - 0.5*10);
            presek2.setX(p.x() + 0.866025404*10); presek2.setY(p.y() + 0.5*10);

            _naivni_pokrivac.emplace_back(QPoint(p.x(),p.y()));
            //AlgoritamBaza_updateCanvasAndBlock();

            _naivni_pokrivac.emplace_back(QPoint(presek1.x(),presek1.y()-10));
            //AlgoritamBaza_updateCanvasAndBlock();

            _naivni_pokrivac.emplace_back(QPoint(presek2.x(),presek2.y()+10));
            //AlgoritamBaza_updateCanvasAndBlock();

            _naivni_pokrivac.emplace_back(QPoint(p.x()+ 2*sqrt(pomocneFunkcije::distanceKvadrat(p,QLine(presek1,presek2).center())),p.y()));
            //AlgoritamBaza_updateCanvasAndBlock();
        }
    }

    /*
    for(QPoint p:_tacke)
    {
        if (UnitDiskCover::rastojanje(p,_naivni_pokrivac) > 100)
        {
            _naivni_pokrivac.emplace_back(p);
        }
    }
    */

    std::cout << "Broj krugova u _naivni_pokrivac: " << _naivni_pokrivac.size() << std::endl << std::endl;
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}



void UnitDiskCover::crtajNaivniAlgoritam(QPainter *painter) const
{
    if(!painter) return;

        QPen p = painter->pen();
        p.setWidth(2);

        for(const QPoint &pt: _tacke){
            p.setColor(Qt::black);
            painter->setPen(p);
            painter->drawPoint(pt);
        }

        for(const QPoint &pt: _naivni_pokrivac){
            p.setWidth(2);
            p.setColor(Qt::red);
            painter->setPen(p);
            painter->drawEllipse(pt, 10, 10);
        }
}

double UnitDiskCover::rastojanje(const QPoint &p, const std::vector<QPoint> &pokrivac)
{
    double minimalno = std::numeric_limits<unsigned int>::max();
    double trenutno = 0.0;

    for(QPoint q: pokrivac)
    {
        trenutno = pomocneFunkcije::distanceKvadrat(p,q);
        if (trenutno < minimalno)
        {
            minimalno = trenutno;
        }
    }
    return minimalno;
}
