#ifndef GA10_UNITDISKCOVER_H
#define GA10_UNITDISKCOVER_H

#include "algoritambaza.h"
#include "pomocnefunkcije.h"
#include <iostream>
#include <fstream>
#include <sys/time.h>

#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>

#define DISK_RADIUS 30

enum BLMSEventType
{
    SiteEvent,
    DeletionEvent
};

/* structure for event points: site events and deletion events */
struct EventPoint
{
    EventPoint(QPointF* point,
               const BLMSEventType eventType,
               EventPoint* sitePoint)
        : point(point),eventType(eventType), sitePoint(sitePoint)
    {}

    QPointF* point;
    BLMSEventType eventType;
    /* sitePoint is nullptr for site events */
    EventPoint* sitePoint;
};

/* BLMS: comparator for event queue */
struct EventQueueComp
{
    bool operator()(const EventPoint* left, const EventPoint* right) const
    {
        return (left->point->x() < right->point->x())
                || (left->point->x() == right->point->x() && left->point->y() < right->point->y())
                || (left->point->x() == right->point->x() && left->point->y() == right->point->y() && left->eventType == SiteEvent && right->eventType == DeletionEvent);
    }
};

/* BLMS: comparator for BST structure _status */
struct StatusComp
{
    bool operator()(const EventPoint* left, const EventPoint* right) const
    {
        return (left->point->y() < right->point->y())
                || (left->point->y() == right->point->y() && left->point->x() < right->point->x());
    }
};

/* G1991: comparator for sets of vertical stripes */
struct StripComp
{
    bool operator()(const QPointF& left, const QPointF& right) const
    {
        return (left.x() < right.x()) || (left.x() == right.x() && left.y() < right.y());
    }
};



class UnitDiskCover : public AlgoritamBaza
{
public:
    UnitDiskCover(QWidget *pCrtanje,
                  int pauzaKoraka,
                  const bool &naivni = false,
                  std::string imeDatoteke = "",
                  int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    unsigned long coverSize() const;

private:

    std::vector<QPointF> _points;
    std::vector<QPointF> _cover;

    void BLMS2017();
    std::set<EventPoint*, EventQueueComp> _eventQueue;
    std::set<EventPoint*, StatusComp> _status;

    void LL2014();
    double _xOfRestrictionLine;

    void GHS2019();
    void G1991();
};

#endif // GA10_UNITDISKCOVER_H
