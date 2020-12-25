#ifndef GA10_DATASTRUCTURES_H
#define GA10_DATASTRUCTURES_H

#include <QPoint>
#include <QLineF>
#include "pomocnefunkcije.h"

enum EventType{
    SiteEvent,
    DeletionEvent
};

struct EventPoint{
    EventPoint(QPoint* point,
               const EventType eventType,
               EventPoint* sitePoint)
        : point(point),eventType(eventType), sitePoint(sitePoint)
    {}

    QPoint* point;
    EventType eventType;
    EventPoint* sitePoint; //postoji samo kada je eventType = DeletionEvent
};

struct EventQueueComp
{
    bool operator()(const EventPoint* left, const EventPoint* right) const
    {
        return (left->point->x() < right->point->x())
                || (left->point->x() == right->point->x() && left->point->y() < right->point->y())
                || (left->point->x() == right->point->x() && left->point->y() == right->point->y() && left->eventType == SiteEvent && right->eventType == DeletionEvent);
    }
};

struct StatusQueueComp
{
    bool operator()(const EventPoint* left, const EventPoint* right) const
    {
        return (left->point->y() > right->point->y())
                || (left->point->y() == right->point->y() && left->point->x() < right->point->x());
    }
};


#endif // GA10_DATASTRUCTURES_H
