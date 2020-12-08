#include "ga06_dcel.h"


#include <iostream>
#include <fstream>

/***********************************************************************/
/*                               DCEL                                  */
/***********************************************************************/

/*
DCEL::DCEL(std::string imeDatoteke, int h, int w)
    :_vertices{}, _edges{}, _fields{}
{
    std::ifstream in(imeDatoteke);
    std::string tmp;

    in >> tmp;
    if (tmp.compare("OFF") != 0)
    {
        std::cout << "Wrong file format: " << imeDatoteke << std::endl;
        exit(EXIT_FAILURE);
    }

    int vertexNum, edgeNum, fieldNum;
    in >> vertexNum >> edgeNum >> fieldNum;

    * Kreiranje temena. */
    /* Za ozbiljnije testranje:
     * Proveriti dokumentaciju za off format.
     * Ovde je prilagodjeno za 2d. I za velicinu naseg kanvasa.
     * Odnosno, podrazumeva se ispravan ulaz.

    float tmpx, tmpy, tmpz;
    int x, y;
    Vertex* v;
    for(int i=0; i<vertexNum; i++)
    {
        * Koordinate u off foramatu su realne, i najcesce [-1, 1]
         * (pp da je uvek tako, da ne ulazimo u detalje)
         * Ovde ih namestam da slika bude na sredini kanvasa.
         *
        in >> tmpx >> tmpy >> tmpz;
        x = static_cast<int>((tmpx + 1)/2.0f*w);
        y = static_cast<int>((tmpy + 1)/2.0f*h);
        v = new Vertex({{x, y}, NULL});
        _vertices.push_back(v);
    }
    * Pravljenje polja i poluivica. *
    int firstIdx;
    for(int i=0; i<fieldNum; i++)
    {
}
*/
DCEL::DCEL(const std::vector<QPointF> &tacke)
    :_vertices{}, _edges{}, _fields{}
{
    loadData(tacke);
}

DCEL::DCEL()
    :_vertices{}, _edges{}, _fields{}
{}

std::vector<Vertex *> DCEL::vertices() const
{
    return _vertices;
}


std::vector<HalfEdge *> DCEL::edges() const
{
    return _edges;
}


std::vector<Field *> DCEL::fields() const
{
    return _fields;
}


void DCEL::setFields(std::vector<Field *> &fileds)
{
    _fields = fileds;
}

void DCEL::loadData(const std::vector<QPointF> &tacke)
{

}

void DCEL::insertEdge(HalfEdge *e)
{
    _edges.push_back(e);
}

void DCEL::insertFiled(Field *f)
{
    _fields.push_back(f);
}

HalfEdge *DCEL::findEdge(Vertex *start, Vertex *end)
{
    for(HalfEdge* e : _edges)
    {
        if (e->origin() == start && e->twin()->origin() == end)
            return e;
    }

    return nullptr;
}
/*
***********************************************************************
*                             VERTEX                                  *
***********************************************************************
*/
Vertex::Vertex()
    :_coordinates{}, _incidentEdge{nullptr}
{}

Vertex::Vertex(QPoint coordinates, HalfEdge *incidentEdge)
    :_coordinates{coordinates}, _incidentEdge{incidentEdge}
{}

QPointF Vertex::coordinates() const
{
    return _coordinates;
}

void Vertex::setCoordinates(const QPoint &coordinates)
{
    _coordinates = coordinates;
}

HalfEdge *Vertex::incidentEdge() const
{
    return _incidentEdge;
}

void Vertex::setIncidentEdge(HalfEdge *incidentEdge)
{
    _incidentEdge = incidentEdge;
}
/*
***********************************************************************
*                           HALFEDGE                                 *
***********************************************************************
*/

HalfEdge::HalfEdge()
    :_origin{nullptr}, _twin{nullptr}, _next{nullptr}, _prev{nullptr}, _incidentFace{nullptr}
{}

HalfEdge::HalfEdge(Vertex *origin, HalfEdge *twin, HalfEdge *next, HalfEdge *prev, Field *incidentFace)
    :_origin(origin), _twin(twin), _next(next), _prev(prev), _incidentFace(incidentFace)
{}

Vertex *HalfEdge::origin() const
{
    return _origin;
}

void HalfEdge::setOrigin(Vertex *origin)
{
    _origin = origin;
}

HalfEdge *HalfEdge::twin() const
{
    return _twin;
}

void HalfEdge::setTwin(HalfEdge *twin)
{
    _twin = twin;
}

HalfEdge *HalfEdge::next() const
{
    return _next;
}

void HalfEdge::setNext(HalfEdge *next)
{
    _next = next;
}

HalfEdge *HalfEdge::prev() const
{
    return _prev;
}

void HalfEdge::setPrev(HalfEdge *prev)
{
    _prev = prev;
}

Field *HalfEdge::incidentFace() const
{
    return _incidentFace;
}

void HalfEdge::setIncidentFace(Field *incidentFace)
{
    _incidentFace = incidentFace;
}

/*
***********************************************************************
*                             FIELD                                   *
***********************************************************************
*/
Field::Field()
    :_outerComponent{nullptr}, _innerComponents{}
{}

Field::Field(HalfEdge *outerComponent, std::vector<HalfEdge *> inerComponent)
    :_outerComponent{outerComponent}, _innerComponents{inerComponent}
{}

HalfEdge *Field::outerComponent() const
{
    return _outerComponent;
}

void Field::setOuterComponent(HalfEdge *outerComponent)
{
    _outerComponent = outerComponent;
}

std::vector<HalfEdge *> Field::innerComponents() const
{
    return _innerComponents;
}

HalfEdge *Field::innerComponent() const
{
    if(innerComponents().empty())
    {
        return nullptr;
    }

    return _innerComponents[0];
}

void Field::setInnerComponents(std::vector<HalfEdge *> inerComponents)
{
    _innerComponents = inerComponents;
}

void Field::setInnerComponent(HalfEdge* innerComponent)
{
    _innerComponents.push_back(innerComponent);
}




