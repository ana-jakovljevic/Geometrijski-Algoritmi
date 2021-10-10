#ifndef GA10_UNITDISKCOVER_H
#define GA10_UNITDISKCOVER_H

#include "algoritambaza.h"
#include "pomocnefunkcije.h"

#include <iostream>
#include <fstream>
#include <random>
#include <sys/time.h>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>

enum BLMSEventType
{
    SiteEvent,
    DeletionEvent
};

/* structure for event points: site events and deletion events */
struct EventPoint
{
    EventPoint(QPointF* pointc,
               const BLMSEventType eventTypec,
               EventPoint* sitePointc)
        : point(pointc),eventType(eventTypec), sitePoint(sitePointc)
    {}

    QPointF* point;
    BLMSEventType eventType;

    /* sitePoint is nullptr for site events */
    EventPoint* sitePoint;
};

/* [BLMS] comparator for event queue */
struct EventQueueComp
{
    bool operator()(const EventPoint* left, const EventPoint* right) const
    {
        return (left->point->x() < right->point->x())
                || (pomocneFunkcije::bliski(left->point->x(), right->point->x()) && left->point->y() < right->point->y())
                || (pomocneFunkcije::bliski(left->point->x(), right->point->x()) && pomocneFunkcije::bliski(left->point->y(), right->point->y()) && left->eventType == SiteEvent && right->eventType == DeletionEvent);
    }
};

/* [BLMS] comparator for BST structure _status */
struct StatusComp
{
    bool operator()(const EventPoint* left, const EventPoint* right) const
    {
        return (left->point->y() < right->point->y())
                || (pomocneFunkcije::bliski(left->point->y(), right->point->y()) && left->point->x() < right->point->x());
    }
};

/* [G1991] comparator for vertical strips */
struct StripComp
{
    bool operator()(const QPointF& left, const QPointF& right) const
    {
        return (left.x() < right.x()) || (pomocneFunkcije::bliski(left.x(), right.x()) && left.y() < right.y());
    }
};

enum AlgorithmType {
    G1991,
    LL2014,
    BLMS2017,
    GHS2019,
    RANDOM1,
    RANDOM2
};

class UnitDiskCover : public AlgoritamBaza
{
public:
    UnitDiskCover(QWidget *pCrtanje,
                  int pauzaKoraka,
                  const bool &naivni = false,
                  std::string imeDatoteke = "",
                  int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA,
                  int diskRadius = 50,
                  AlgorithmType algorithm = AlgorithmType::GHS2019
                  );

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    /* returns number of circles in cover */
    unsigned long coverSize() const;
    unsigned long naiveCoverSize() const;

    /* checking that all points are covered (returns true if they are) */
    bool checkCoverage() const;
    bool checkNaiveCoverage() const;

private:

    /* fixed radius of disks */
    int _diskRadius;

    /* type of algorithm */
    int _algorithm;

    /* points to be covered */
    std::vector<QPointF> _points;

    /* center position of every circle in cover */
    std::vector<QPointF> _cover;
    std::vector<QPointF> _naiveCover;

    /* return number of uncovered points by current disks in cover */
    unsigned long countUncovered(const std::vector<QPointF>& cover) const;

    /* Gonzalez algorithm */
    void G1991();
    void paintG1991(QPainter* painter) const;
    std::map<int, std::set<QPointF, StripComp>> _S;
    std::vector<QPointF> _squares;

    /* Liu, Lu algorithm */
    void LL2014();
    void paintLL2014(QPainter* painter) const;
    std::vector<QPointF> _LLcover;
    double _xOfRestrictionLine = 0;
    double _right = 0;

    /* Biniaz, Liu, Maheshwari, Smid algorithm */
    void BLMS2017();
    void paintBLMS2017(QPainter* painter) const;
    std::set<EventPoint*, EventQueueComp> _eventQueue;
    std::set<EventPoint*, StatusComp> _status;
    double _sweepLine = 0;

    /* Ghosh, Hicks, Shevchenko algorithm */
    void GHS2019();
    void paintGHS2019(QPainter* painter) const;

    void randomAlgorithm1();
    void randomAlgorithm2();
    /* for naive random algorithm */
    QPointF generateRandomPoint();
};

#endif // GA10_UNITDISKCOVER_H
