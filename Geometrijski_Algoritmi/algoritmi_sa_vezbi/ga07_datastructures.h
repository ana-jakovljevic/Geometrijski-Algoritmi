#ifndef GA07_DATASTRUCTURES_H
#define GA07_DATASTRUCTURES_H

#include "ga06_dcel.h"
#include "pomocnefunkcije.h"

///
/// \brief The EventQueueComp struct
/// Brisuca prava se krece od jedne ka drugoj EventTacki
///
struct EventQueueCompTriangulation
{
    bool operator()(const Vertex* levi, const Vertex* desni) const
    {
        return levi->coordinates().y() > desni->coordinates().y() ||
              (fabs(levi->coordinates().y() - desni->coordinates().y()) < EPS &&
               levi->coordinates().x() < desni->coordinates().y());
    }
};

///
/// \brief The StatusQueueCompTriangulation struct
///
/// Status za Monotono Particionisanje
///
struct StatusQueueCompTriangulation
{
private:
    const double *const yBrisucaPrava;

public:
    StatusQueueCompTriangulation(double *y)
        : yBrisucaPrava(y)
    {}

    bool operator()(const HalfEdge* line1, const HalfEdge* line2) const
    {

       QLineF brisucaPrava(0, *yBrisucaPrava, 5, *yBrisucaPrava);
       QPointF presek1;
       QPointF presek2;

       QLineF line11(line1->origin()->coordinates(), line1->twin()->origin()->coordinates());
       QLineF line22(line2->origin()->coordinates(), line2->twin()->origin()->coordinates());

       pomocneFunkcije::presekDuzi(brisucaPrava, line11, presek1);
       pomocneFunkcije::presekDuzi(brisucaPrava, line22, presek2);

       return presek1.x() < presek2.x();
    }
};

struct DiagonalsComp
{
public:
    bool operator()(const HalfEdge* line1, const HalfEdge* line2) const
    {
        assert(line1->origin() == line2->origin());
        float x1 = line1->twin()->origin()->x() - line1->origin()->x();
        float y1 = line1->twin()->origin()->y() - line1->origin()->y();
        float x2 = line2->twin()->origin()->x() - line2->origin()->x();
        float y2 = line2->twin()->origin()->y() - line2->origin()->y();
        if(y1 < 0){
            if(y2 < 0){
                return x1/y1 > x2/y2;
            }
            else{
                return false;
            }
        }
        else if(y1 > 0){
            if(y2 > 0){
                return x1/y1 > x2/y2;
            }
            else if(y2 < 0){
                return true;
            }
            else{
                return x2 < 0;
            }
        }
        else{
            if(y2 == 0)
                return x1 > x2;
            else if(y2 < 0)
                return true;
            else
                return x1 > 0;
        }
    }
};

///
/// \brief The EventQueueCompTriangulation2 struct
/// U Event za Triangulaciju pamtimo HalfEdge, a ne vertex
/// (kao sto je dato u knjizi)
/// zbog problema u implementaciji koji postoje ako se koristi vertex
///
/// TODO
struct EventQueueCompTriangulation2
{
    bool operator()(const HalfEdge* lhs, const HalfEdge* rhs) const
    {
        return (lhs->origin()->y() > rhs->origin()->y())||
                (lhs->origin()->y() == rhs->origin()->y() && lhs->origin()->x()<rhs->origin()->x());

    }
};


#endif // GA07_DATASTRUCTURES_H
