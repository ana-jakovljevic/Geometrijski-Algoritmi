#include "ga10_unitDiskCover.h"

UnitDiskCover::UnitDiskCover(QWidget *pCrtanje,
                              int pauzaKoraka,
                              const bool &naivni,
                              std::string imeDatoteke,
                              int brojTacaka,
                              int diskRadius,
                              AlgorithmType algorithm)
    : AlgoritamBaza(pCrtanje,pauzaKoraka,naivni), _diskRadius(diskRadius), _algorithm(algorithm)
{
    if(imeDatoteke == "")
    {
        std::vector<QPoint> points = generisiNasumicneTacke(brojTacaka);
        for(const QPoint& point: points)
        {
            _points.push_back(QPointF(point.x(), point.y()));
        }
    }
    else
    {
        double x, y;
        std::ifstream inputFile(imeDatoteke);
        while(inputFile >> x >> y)
        {
            _points.emplace_back(x, y);
        }
    }
}

unsigned long UnitDiskCover::coverSize() const
{
    return _cover.size();
}

unsigned long UnitDiskCover::naiveCoverSize() const
{
    return _naiveCover.size();
}

int UnitDiskCover::countUncovered(const std::vector<QPointF>& cover) const
{
    std::vector<QPointF> points(_points);
    for(const QPointF& disk: cover)
    {
        auto it = std::begin(points);
        while(it != std::end(points))
        {
            /* eliminate all points at distance _diskRadius (thay are covered) with allowed error EPS=1e-6 */
            if(pomocneFunkcije::distanceKvadratF(disk, *it) <= (_diskRadius*_diskRadius) + EPS)
            {
                it = points.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    /* number of uncovered points */
    return points.size();
}

bool UnitDiskCover::checkCoverage() const
{
   /* return true if there are no uncovered points */
   return countUncovered(_cover) == 0;
}

bool UnitDiskCover::checkNaiveCoverage() const
{
   /* return true if there are no uncovered points */
   return countUncovered(_naiveCover) == 0;
}

QPointF UnitDiskCover::generateRandomPoint()
{
    int xMax, yMax;

    if (_pCrtanje)
    {
        xMax = _pCrtanje->width() - 10;
        yMax = _pCrtanje->height() - 10;
    }
    else
    {
        xMax = CANVAS_WIDTH;
        yMax = CANVAS_HEIGHT;
    }

    int xMin = 10;
    int yMin = 10;

    std::vector<QPoint> randomPoints;

    int xDiff = xMax-xMin;
    int yDiff = yMax-yMin;

    randomPoints.emplace_back(xMin + rand()%xDiff, yMin + rand()%yDiff);

    return QPointF(xMin + rand()% xDiff, yMin +rand()% yDiff);
}

void UnitDiskCover::pokreniAlgoritam()
{
    //struct timeval begin, end;
    //gettimeofday(&begin, NULL);

    switch(_algorithm)
    {
    case AlgorithmType::G1991:
        G1991();
        break;
    case AlgorithmType::LL2014:
        LL2014();
        break;
    case AlgorithmType::BLMS2017:
        BLMS2017();
        break;
    case AlgorithmType::GHS2019:
        GHS2019();
        break;
     default:
        pokreniNaivniAlgoritam();
    };

    //gettimeofday(&end, NULL);

    //std::cout << "Time of execution: " << (end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec)/1000000.0 << std::endl;
    //std::cout << "Number of disks in _cover: " << _cover.size() << std::endl;

    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}


void UnitDiskCover::G1991()
{
    const double squareSize = _diskRadius*sqrt(2);

    std::vector<QPointF> P1;
    std::vector<QPointF> P2;

   /* partitioning points in two separate set based on their belonging to horizontal strip */
   for(QPointF point: _points)
   {
       if(int(floor(point.y()/squareSize)) % 2 == 1)
       {
           P1.push_back(point);
       }
       else
       {
           P2.push_back(point);
       }
   }

   /* same algorithm is performed on both set of horizontal strips (if they aren't empty) */
   std::vector<std::vector<QPointF>> sets;

   if(P1.size() != 0)
   {
       sets.push_back(P1);
   }

   if(P2.size() != 0)
   {
       sets.push_back(P2);
   }

   for(std::vector<QPointF> set: sets)
   {
       _S.clear();

       /* partitioning points in separate sets based on their belonging to vertical strips */
       for(QPointF &point: _points)
       {
           auto ixp = int(floor(point.x()/squareSize));
           _S[ixp].insert(point);
       }

       AlgoritamBaza_updateCanvasAndBlock();

       /* structure containing points from two neighbor strips */
       std::set<QPointF, StripComp> R;

       auto S1 = std::begin(_S);
       auto S2 = std::next(std::begin(_S));
       auto j = std::end(_S);

       R.merge(S1->second);
       if(std::next(S1) != std::end(_S))
       {
           j  = std::next(S2);
           R.merge(S2->second);
       }

       /* creation of square coverage */
       while(R.size() > 0)
       {
            /* smallest element in R along x axis */
            auto q = *R.begin();

            /* Q is the set of points in R at a distance <= sqrt(2) (with respect to x only) from q; R = R\Q */
            auto it = std::begin(R);
            while(it != std::end(R))
            {
                if(int(floor(it->y()/squareSize)) == int(floor(q.y()/squareSize)) && it->x() - q.x() <= squareSize)
                {
                    it = R.erase(it);
                }
                else if(it->x() - q.x() > squareSize)
                {
                    break;
                }
                else
                {
                    it++;
                }
            }

            /* Put square whose left boundary includes q and whose top boundary conicides with the top boundary of the slab in solution */
            _squares.push_back(QPointF(q.x(), floor(q.y()/squareSize)*squareSize));
            AlgoritamBaza_updateCanvasAndBlock();

            /* R contains elements from at most one of the sets of S*/
            bool flag = true;

            /* while there are non-visited S sets and while R containts elements from at most one of the sets of S: expand R */
            while(j != _S.end() && flag)
            {
                if(R.size() <= 0)
                    flag = true;
                else
                {
                    auto value = int(floor(std::begin(R)->x()/squareSize));

                    /* check for every element in R */
                    for(auto it=std::prev(std::end(R)); it != std::begin(R); it--)
                    {
                        /* there are elements from more than one set */
                        if(int(floor(it->x()/squareSize)) != value)
                        {
                            flag = false;
                            break;
                        }
                    }
                }

                /* expand R */
                if(flag)
                {
                    R.merge(j->second);
                    j++;
                }
            }
       }
    }

    /* for every square in solution place matching disk */
    for(QPointF &center: _squares)
    {
         _cover.emplace_back(center.x() + squareSize/2,
                             center.y() + squareSize/2);
    }


}

void UnitDiskCover::LL2014()
{
    /* preparing data */
    std::sort(_points.begin(),
              _points.end(),
             [](const QPointF& left, const QPointF& right){
                 return left.x() < right.x();
             });

    int n = _points.size();
    if (n == 0)
    {
        _cover = std::vector<QPointF>();
        return;
    }

    long unsigned minCoverSize = n+1;

    /* shiftng strategy */
    for(int i=0; i<6; i++)
    {
        _LLcover.clear();
        int current = 0;

        /* right boundary of strip  */
        _right = _points[current].x() + _diskRadius*i*sqrt(3)/6.0;

        while (current < n)
        {
            int index = current;
            auto segments = std::vector<std::pair<QPointF, QPointF>>();
            double y, d;
            _xOfRestrictionLine = _right - _diskRadius*sqrt(3)/2.0;

            AlgoritamBaza_updateCanvasAndBlock();

            /* finding all points belonging to current strip */
            while (_points[current].x() < _right && current < n)
            {
                current++;
            }

            /* for every point in current strip */
            for(int j = index; j < current; j++)
            {
                /* calculate the distance from restriction line */
                d = _points[j].x() - _xOfRestrictionLine;

                /* calculate position of the segment on restriction line */
                y = sqrt(_diskRadius*_diskRadius - d*d);
                auto upperPoint = QPointF(_xOfRestrictionLine, _points[j].y() + y);
                auto lowerPoint = QPointF(_xOfRestrictionLine, _points[j].y() - y);

                auto segment = std::pair<QPointF, QPointF>(upperPoint, lowerPoint);
                segments.push_back(segment);
            }

            /* sort segments based on y value of their lower point */
            std::sort(segments.begin(),
                      segments.end(),
                     [](const std::pair<QPointF, QPointF>& left, const std::pair<QPointF, QPointF>& right){
                         return (left.second.y() < right.second.y());
                     });

            while(segments.size() > 0)
            {
                /* taking last element which have largest value of lower point y coordinate */
                y = segments.back().second.y();

                /* position circle as low as possible while still covering the topmost uncovered point */
                _LLcover.push_back(QPointF(_xOfRestrictionLine, y));
                AlgoritamBaza_updateCanvasAndBlock();

                segments.pop_back();

                /* eliminate all segments that are covered by this circle */
                while(segments.size() > 0 && segments.back().first.y() >= y && segments.back().second.y() <= y)
                {
                    segments.pop_back();
                }
            }

            /* current is index of first point from next strip; moving to the next strip of size _diskRadius*sqrt(3) */
            _right += _diskRadius*sqrt(3);
            while((_points[current].x() - _right) > _diskRadius*sqrt(3))
            {
                _right += _diskRadius*sqrt(3);
            }
       }

        /* update best found cover based on size */
        if (_LLcover.size() < minCoverSize)
        {
            _cover = _LLcover;
            minCoverSize = _cover.size();
        }
    }
}

void UnitDiskCover::BLMS2017()
{
    /* initialization of event queue */
    for(auto &point: _points)
    {
        auto siteEvent = new EventPoint(&point, BLMSEventType::SiteEvent, nullptr);
       _eventQueue.insert(siteEvent);
       _eventQueue.insert(new EventPoint(new QPointF(point.x()+ 2*_diskRadius, point.y()), BLMSEventType::DeletionEvent, siteEvent));
    }

    /* visiting events from event queue */
    while (!_eventQueue.empty())
    {
        auto eventPoint = *(_eventQueue.begin());
        _eventQueue.erase(_eventQueue.begin());

        _sweepLine = eventPoint->point->x();
        AlgoritamBaza_updateCanvasAndBlock();

        /* if event is deletion event: erase corresponding site point from status */
        if (eventPoint->eventType == BLMSEventType::DeletionEvent)
        {
            _status.erase(eventPoint->sitePoint);
            delete eventPoint->sitePoint;
            delete eventPoint;
        }
        /* if event is site event: check its coverege */
        else
        {
            bool flag = true;

            /* searching two closest point above it by y coordinate and checking if they are covering current point */
            auto pPlus = _status.upper_bound(eventPoint);
            if (flag && pPlus != _status.end())
            {
                flag = flag && (pomocneFunkcije::distanceKvadratF(*(*pPlus)->point, *eventPoint->point) > (2*_diskRadius)*(2*_diskRadius));
                auto pPlusPlus = ++pPlus;
                if(flag && pPlusPlus != _status.end())
                {
                    flag = flag && (pomocneFunkcije::distanceKvadratF(*(*pPlusPlus)->point, *eventPoint->point) > (2*_diskRadius)*(2*_diskRadius));
                }
            }

            /* searching two closest point below it by y coordinate and checking if they are covering current point */
            if( flag && pPlus != _status.begin())
            {
                auto p = _status.lower_bound(eventPoint);
                if(p != _status.begin())
                {
                    auto pMinus = --p;
                    flag = flag && (pomocneFunkcije::distanceKvadratF(*(*pMinus)->point,*eventPoint->point) > (2*_diskRadius)*(2*_diskRadius));
                    if(flag && pMinus != _status.begin())
                    {
                        auto pMinuMinus = --pMinus;
                        flag = flag && (pomocneFunkcije::distanceKvadratF(*(*pMinuMinus)->point,*eventPoint->point) > (2*_diskRadius)*(2*_diskRadius));
                    }
                }
            }

            /* if point isn't alrady covered: insert four new disks */
            if(flag)
            {
                _status.insert(eventPoint);
                QPointF intersec1, intersec2;
                auto p = eventPoint->point;

                intersec1.setX(p->x() + sqrt(3)/2*_diskRadius);
                intersec1.setY(p->y() - 0.5*_diskRadius);
                intersec2.setX(p->x() + sqrt(3)/2*_diskRadius);
                intersec2.setY(p->y() + 0.5*_diskRadius);

                /* adding centers of new disks in _cover */
                _cover.emplace_back(*eventPoint->point);
                _cover.emplace_back(QPointF(intersec1.x(),intersec1.y()-_diskRadius));
                _cover.emplace_back(QPointF(intersec2.x(),intersec2.y()+_diskRadius));
                _cover.emplace_back(QPointF(p->x()+ 2*sqrt(pomocneFunkcije::distanceKvadratF(*p,QLineF(intersec1,intersec2).center())),p->y()));

                AlgoritamBaza_updateCanvasAndBlock();
            }
        }
    }
}

void UnitDiskCover::GHS2019()
{
    const double squareSize = _diskRadius*sqrt(2);

    /* hashTable contains positions of covered squares */
    std::unordered_map<int, std::unordered_set<int>> hashTable;

    for(QPointF point: _points)
    {
        int v = int(floor(point.x()/squareSize));
        int h = int(floor(point.y()/squareSize));
        double d = _diskRadius-squareSize/2.0;

        if(hashTable.find(v) != hashTable.end() && hashTable[v].find(h) != hashTable[v].end())
        {
            /* p is covered by the disk placed before */
        } else if(point.y() <= h*squareSize+squareSize-d && point.y() >= h*squareSize+d && point.x() <= v*squareSize+squareSize-d && point.x() >= v*squareSize+d)
        {
            /* point is not covered, so cover it now */
            hashTable[v].insert(h);
            _cover.push_back(QPointF(squareSize*v+squareSize/2.0, squareSize*h+squareSize/2.0));
            AlgoritamBaza_updateCanvasAndBlock();
        }
        else if( point.x() >= squareSize*(v+1.5)-_diskRadius && hashTable.find(v+1) != hashTable.end() && hashTable[v+1].find(h) != hashTable[v+1].end()
                   && sqrt(pomocneFunkcije::distanceKvadratF(point, QPointF(squareSize*(v+1)+squareSize/2.0, squareSize*h+squareSize/2.0))) <= _diskRadius)
        {
            /* p is covered by the disk placed right */
        } else if(point.x() <= squareSize*(v-0.5)+_diskRadius && hashTable.find(v-1) != hashTable.end() && hashTable[v-1].find(h) != hashTable[v-1].end()
                  && sqrt(pomocneFunkcije::distanceKvadratF(point, QPointF(squareSize*(v-1)+squareSize/2.0, squareSize*h+squareSize/2.0))) <= _diskRadius)
        {
            /* p is covered by the disk placed left */
        } else if(point.y() >= squareSize*(h+1.5)-_diskRadius && hashTable.find(v) != hashTable.end() && hashTable[v].find(h+1) != hashTable[v].end()
                  && sqrt(pomocneFunkcije::distanceKvadratF(point, QPointF(squareSize*v+squareSize/2.0, squareSize*(h+1)+squareSize/2.0))) <= _diskRadius)
        {
            /* p is covered by the disk placed above */
        } else if(point.y() <= squareSize*(h-0.5)+_diskRadius && hashTable.find(v) != hashTable.end() && hashTable[v].find(h-1) != hashTable[v].end()
                  && sqrt(pomocneFunkcije::distanceKvadratF(point, QPointF(squareSize*v+squareSize/2.0, squareSize*(h-1)+squareSize/2.0))) <= _diskRadius)
        {
            /* p is covered by the disk placed below */
        } else {
            /* p is not covered, so cover it now */
            hashTable[v].insert(h);
            _cover.push_back(QPointF(squareSize*v+squareSize/2.0, squareSize*h+squareSize/2.0));
            AlgoritamBaza_updateCanvasAndBlock();
        }
    }
}

void UnitDiskCover::crtajAlgoritam(QPainter *painter) const
{
    if(!painter) return;

    switch(_algorithm)
    {
    case AlgorithmType::G1991:
       paintG1991(painter);
       break;
    case AlgorithmType::LL2014:
        paintLL2014(painter);
        break;
    case AlgorithmType::BLMS2017:
        paintBLMS2017(painter);
        break;
    case AlgorithmType::GHS2019:
        paintGHS2019(painter);
        break;
     default:
        crtajNaivniAlgoritam(painter);
    }

    QPen p = painter->pen();

    p.setWidth(2);
    p.setColor(Qt::red);
    painter->setPen(p);
    for(const QPointF& pt: _cover)
    {
        painter->drawEllipse(pt, _diskRadius, _diskRadius);
    }

}

void UnitDiskCover::paintG1991(QPainter *painter) const
{
    QPen p = painter->pen();

    p.setWidth(1);
    p.setColor(Qt::black);
    p.setStyle(Qt::DashLine);
    painter->setPen(p);
    for(int i=0;i<CANVAS_HEIGHT;i++)
    {
        painter->drawLine(0,i*sqrt(2)*_diskRadius, 1100, i*sqrt(2)*_diskRadius);
    }

    p.setStyle(Qt::SolidLine);
    p.setColor(Qt::black);
    painter->setPen(p);
    for(auto s: _S)
    {
        painter->drawLine(s.first * sqrt(2)*_diskRadius, 2, s.first*sqrt(2)*_diskRadius, 590);
        painter->drawLine( (1 + s.first) * sqrt(2)*_diskRadius, 0, (1 + s.first*sqrt(2)*_diskRadius), 590);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(s.first * sqrt(2)*_diskRadius, 2, sqrt(2)*_diskRadius, 590);
    }

    p.setWidth(2);
    p.setColor(Qt::blue);
    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    for(const QPointF& point: _squares)
    {
        painter->drawRect(point.x(),point.y(), sqrt(2)*_diskRadius, sqrt(2)*_diskRadius);
    }

    p.setColor(Qt::black);
    painter->setPen(p);
    for(const QPointF& point: _points)
    {
        painter->drawPoint(point);
    }

}

void UnitDiskCover::paintLL2014(QPainter *painter) const
{
    QPen p = painter->pen();

    p.setWidth(1);
    p.setColor(Qt::blue);
    p.setStyle(Qt::DashLine);
    painter->setPen(p);
    painter->drawLine(_xOfRestrictionLine, 0, _xOfRestrictionLine, CANVAS_HEIGHT);

    p.setColor(Qt::black);
    p.setStyle(Qt::SolidLine);
    painter->setPen(p);
    painter->drawLine(_right, 0, _right, CANVAS_HEIGHT);
    painter->drawLine(_right - _diskRadius*sqrt(3), 0, _right - _diskRadius*sqrt(3), CANVAS_HEIGHT);

    p.setWidth(2);
    p.setColor(Qt::black);
    painter->setPen(p);
    for(const QPointF& pt: _points)
    {
        painter->drawPoint(pt);
    }

    for(const QPointF& pt: _LLcover)
    {
        painter->drawEllipse(pt, _diskRadius, _diskRadius);
    }
}

void UnitDiskCover::paintBLMS2017(QPainter* painter) const
{
    QPen p = painter->pen();
    p.setWidth(2);

    p.setColor(Qt::blue);
    p.setStyle(Qt::DashLine);
    painter->setPen(p);
    painter->drawLine(_sweepLine, 0, _sweepLine, 700);


    for(const QPointF& pt: _points)
    {
        if(pt.x() <= _sweepLine - 2*_diskRadius)
        {
            p.setColor(Qt::red);
            painter->setPen(p);
        }
        else
        {
            p.setColor(Qt::black);
            painter->setPen(p);
        }

        painter->drawPoint(pt);
    }

    for(EventPoint* pt: _status)
    {
        p.setColor(Qt::green);
        painter->setPen(p);
        painter->drawPoint(*pt->point);
    }

    p.setStyle(Qt::SolidLine);
    painter->setPen(p);
}

void UnitDiskCover::paintGHS2019(QPainter *painter) const
{
    QPen p = painter->pen();

    p.setWidth(1);
    p.setColor(Qt::black);
    painter->setPen(p);

    for(int i = 0; i < CANVAS_WIDTH; i++)
    {
        painter->drawLine(i*sqrt(2)*_diskRadius, 0, i*sqrt(2)*_diskRadius, CANVAS_HEIGHT);
        painter->drawLine(0,i*sqrt(2)*_diskRadius, CANVAS_WIDTH, i*sqrt(2)*_diskRadius);
    }

    p.setWidth(2);
    p.setColor(Qt::black);
    painter->setPen(p);
    for(const QPointF& pt: _points)
    {
        painter->drawPoint(pt);
    }
}

void UnitDiskCover::randomAlgorithm1()
{
    srand(static_cast<unsigned>(time(nullptr)));

    /* all points are uncovered at beginng */
    int numberOfUncoveredPoints = _points.size();

    /* generate random circles until all points are covered */
    while(true)
    {
        QPointF randomPoint = generateRandomPoint();

        _naiveCover.push_back(randomPoint);
        AlgoritamBaza_updateCanvasAndBlock();

        /* if added disk doesn't change number of covered points it should be eliminated */
        if(countUncovered(_naiveCover) == numberOfUncoveredPoints)
        {
            _naiveCover.pop_back();
            AlgoritamBaza_updateCanvasAndBlock();
        }
        /* if it covers some point it should be retained */
        else
        {
            numberOfUncoveredPoints = countUncovered(_naiveCover);
            AlgoritamBaza_updateCanvasAndBlock();
        }

        /* if all points are covered algorithm is done */
        if(numberOfUncoveredPoints == 0)
        {
            break;
        }
    }
}

void UnitDiskCover::randomAlgorithm2()
{
    std::default_random_engine generator(time(nullptr));
    std::uniform_real_distribution<double> distribution(-1*_diskRadius*sqrt(2)/2.0,_diskRadius*sqrt(2)/2.0);
    std::vector<QPointF> points(_points);

    while(points.size() > 0)
    {
        double r1 = distribution(generator);
        int randX = points.begin()->x() + r1;
        double r2 = distribution(generator);
        int randY = points.begin()->y() + r2;

        QPointF center(randX, randY);
        _naiveCover.push_back(center);
        //AlgoritamBaza_updateCanvasAndBlock();

        auto it = std::begin(points);
        it = points.erase(it);
        while(it != std::end(points))
        {
            if(pomocneFunkcije::distanceKvadratF(*it, center) <= _diskRadius*_diskRadius)
                it = points.erase(it);
            else
                it++;
        }
    }
}

void UnitDiskCover::pokreniNaivniAlgoritam()
{
    randomAlgorithm1();
    /*randomAlgorithm2();*/

    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void UnitDiskCover::crtajNaivniAlgoritam(QPainter *painter) const
{
    if(!painter) return;

    QPen p = painter->pen();

    p.setWidth(2);
    p.setColor(Qt::black);
    painter->setPen(p);
    for(const QPointF& pt: _points)
    {
        painter->drawPoint(pt);
    }

    p.setWidth(2);
    p.setColor(Qt::red);
    painter->setPen(p);

    for(const QPointF& pt: _naiveCover)
    {
        painter->drawEllipse(pt, _diskRadius, _diskRadius);
    }
}
