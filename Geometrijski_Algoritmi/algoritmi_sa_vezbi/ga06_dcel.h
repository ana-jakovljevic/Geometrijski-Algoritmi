#ifndef DCEL_H
#define DCEL_H

#include <QPoint>
#include <QWidget>

class Vertex;
class HalfEdge;
class Field;

///
/// \brief The DCEL class
/// Dvostruko povezna lista (Doubly-Connected Edge List)
/// Vise procitati u knjizi Computational Geometry
///
class DCEL
{
public:
    DCEL(std::string imeDatoteke, int h, int w);

    ///
    /// \brief DCEL
    /// Poligon zadatak tackama u pozitivnom matematickom smeru
    /// bez rupa
    ///
    DCEL(const std::vector<QPoint> &tacke);

    DCEL() = default;
    //....
private:
    //...
};

///
/// \brief The Vertex class
/// Teme poligona
/// Sadrzi koordinate
/// i jednu incident (proizvoljnu) HalfEdge
class Vertex{
public:


private:

};


///
/// \brief The HalfEdge class
/// Ovo je zapravo jedna ivica poligona, ali ivica moze imati dva smera, pa je
/// sa HalfEdge oznacen jedan smer, a ona na koju pokazuje (twin) je ista ivica, ali drugi smer
/// IncidentFace je face (strana) koja je sa leve strane posmatrane HalfEdge
/// Next i Prev su prethodna i sledeca HalfEdge
/// Origin je Vrtex koji je pocetak HalfEdge
/// Vise o ovoj strukturi procitati u Computational Geoemtry
///
class HalfEdge{
public:


private:
};

///
/// \brief The Field class
/// Sadrzi dva niza HalfEdge
/// Prvi je OuterComponent i pokazuje na one HalfEdge koje su na granici (boundary) posmatrane stranice
/// Za one stranice koje nemaju granicu, tj. koje su nepovezane taj niz je nil (tj. prazan)
/// Drugi niz je InnerComponent i pokazuje na one HalfEdge koje cine "rupu" u okviru posmatrane stranice
/// Moze biti prazan
/// Vise o ovoj strukturi procitati u Computational Geoemtry
///
class Field{
public:


private:
};

#endif // DCEL_H
