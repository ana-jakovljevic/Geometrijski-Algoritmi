#include "ga06_dcel.h"


#include <iostream>
#include <fstream>
#include <map>
/***********************************************************************/
/*                               DCEL                                  */
/***********************************************************************/


DCEL::DCEL(std::string imeDatoteke, int h, int w)
    :_vertices{}, _edges{}, _fields{}
{
    std::ifstream in(imeDatoteke);
    std::string tmp;

    in >> tmp;
    if (tmp.compare("OFF") != 0) {
        std::cout << "Wrong file format: " << imeDatoteke << std::endl;
        exit(EXIT_FAILURE);
    }

    int vertexNum, edgeNum, fieldNum;
    in >> vertexNum >> fieldNum >> edgeNum;

    // citamo teme po teme iz fajla
    // i pamtimo ga u nizu temena
    float tmpx, tmpy, tmpz;
    float x, y;
    Vertex* v;
    for(int i=0; i<vertexNum; i++) {
        in >> tmpx >> tmpy >> tmpz;
        x = (tmpx + 1) / 2.0f * w;
        y = (tmpy + 1) / 2.0f * h;
        v = new Vertex({{x, y}, nullptr});
        _vertices.push_back(v);
    }

    // za svaki poligon(polje)
    // za svako teme tog poligona napravimo polustranicu sa tim temenom kao pocetkom
    // zatim prodjemo kroz napravljene polustranice i popunimo prethodnu i sledecu
    for(int i=0; i<fieldNum; i++) {
        Field* f = new Field();
        int broj_temena;
        in >> broj_temena;
        std::vector<HalfEdge*> edges;
        for(int j = 0; j < broj_temena; ++j){
            int indeksTemena;
            in >> indeksTemena;
            HalfEdge *new_halfedge = new HalfEdge(_vertices[indeksTemena], nullptr, nullptr, nullptr, nullptr);
            edges.push_back(new_halfedge);
            _edges.push_back(new_halfedge);
            _vertices[indeksTemena]->setIncidentEdge(new_halfedge);
        }
        for(int j = 0; j < broj_temena; j++){
            edges[j]->setNext(edges[(j + 1) % broj_temena]);
            edges[j]->setPrev(edges[(j - 1 + broj_temena) % broj_temena]);
            edges[j]->setIncidentFace(f);
        }
        f->setOuterComponent(edges[0]);
        _fields.push_back(f);
    }

    std::map<Vertex*, HalfEdge*> spoljasnje_ivice; // mapa u kojoj mozemo naci stranicu na osnovu temena na koji "pokazuje"
                                                  // u njoj cemo pamtiti spoljasnje ivice i koristimo je da bi ih kasnije povezali
    Field * spoljasnost = new Field();

    // za svaku polustranicu AB pokusavamo da nadjemo polustranicu BA
    // ako takva nepostoji to znaci da smo naisli na spoljasnju polustranicu
    // i nju smestamo u mapu spoljasnje ivice za kasniju obradu
    for(auto edge : _edges){
        auto uslov = [=](HalfEdge* e){
            return e->origin() == edge->next()->origin()
                   && e->next()->origin() == edge->origin();
        };
        auto twin = std::find_if(std::begin(_edges), std::end(_edges), uslov);
        if(twin == std::end(_edges)){
            HalfEdge *outer_edge = new HalfEdge();
            edge->setTwin(outer_edge);
            outer_edge->setIncidentFace(spoljasnost);
            outer_edge->setTwin(edge);
            outer_edge->setOrigin(edge->next()->origin());
            spoljasnje_ivice[edge->origin()] = outer_edge;
        }
        else{
            edge->setTwin(*twin);
        }
    }

    // za svaku spoljasnju ivicu trazimo spoljasnju koja joj prethodi
    // to mozemo jednostavno uraditi zbog nacina pravljenja mape spoljasnje_ivice
    // nakon sto zavrsimo "sredjivanje" te poluivice dodajemo je u listu poluivica DCEL strukture
    for(auto outer_edge : spoljasnje_ivice){
        outer_edge.second->setPrev(spoljasnje_ivice[outer_edge.second->origin()]);
        spoljasnje_ivice[outer_edge.second->origin()]->setNext(outer_edge.second);
        _edges.push_back(outer_edge.second);
    }

    spoljasnost->setInnerComponent(spoljasnje_ivice.begin()->second);
    _fields.push_back(spoljasnost);
}

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
    /* tacke moraju biti zadate u smeru suprotnom kazaljci na satu
       inace ova metoda nije validna */

    Field* _inner = new Field();
    Field* _outer = new Field();

    _fields.push_back(_inner);
    _fields.push_back(_outer);

    for(auto &tacka : tacke) {
        Vertex* _newVertex = new Vertex(tacka, nullptr);
        _vertices.push_back(_newVertex);

        HalfEdge* _newEdge = new HalfEdge(_newVertex, nullptr, nullptr, nullptr, _inner);
        _edges.push_back(_newEdge);

    }

    for(unsigned j = 0; j < _vertices.size(); ++j) {
        // Postavljanje odgovarajuce ivice za Vertex
        _vertices[j]->setIncidentEdge(_edges[j]);

        // Postavljanje next ivice
        if(j != _vertices.size()-1)
            _edges[j]->setNext(_edges[j+1]);
        else
            _edges[j]->setNext(_edges[0]);

        // Postavljanje prev ivice
        if(j != 0)
            _edges[j]->setPrev(_edges[j-1]);
        else
            _edges[j]->setPrev(_edges[_vertices.size()-1]);

        // Postavljamo twin ivice
        HalfEdge *_twinEdge = new HalfEdge(_vertices[(j+1) % _vertices.size() ], _edges[j], nullptr, nullptr, _outer);
        _edges[j]->setTwin(_twinEdge);
        _edges.push_back(_twinEdge);    // Sada smo poceli da stavljamo twin ivice, one pocinju od pozicije n+j u vektoru
    }

    for(unsigned k = _vertices.size(); k < _edges.size(); ++k) {
        // Postavljanje next ivice za twin
        if(k != _edges.size()-1)
            _edges[k]->setNext(_edges[k+1]);
        else
            _edges[k]->setNext(_edges[_vertices.size()]);

        // Postavljanje prev ivice za twin
        if(k != 0)
            _edges[k]->setPrev(_edges[k-1]);
        else
            _edges[k]->setPrev(_edges[_edges.size()-1]);
    }

    // Ovde je izabran _edges[0] umesto njegovog twin-a, zato sto smo izabrali
    // da radimo u smeru suprotnom kazaljci na satu
    _inner->setOuterComponent(_edges[0]);

    _outer->setInnerComponent(_edges[0]->twin());
    _outer->setOuterComponent(nullptr); // Outer nema granicu

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

Vertex::Vertex(QPointF coordinates, HalfEdge *incidentEdge)
    :_coordinates{coordinates}, _incidentEdge{incidentEdge}
{}

QPointF Vertex::coordinates() const
{
    return _coordinates;
}

void Vertex::setCoordinates(const QPointF &coordinates)
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




