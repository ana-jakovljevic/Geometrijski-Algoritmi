#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H
#include <set>
#include <list>
#include <float.h>
#include"algoritambaza.h"
#include "pomocnefunkcije.h"
class Cvor;
class PoluIvica;
class Poligon;
enum class TipIvica;
struct PoredjenjeZaStabloPretrage;
struct PoredjenjeZaCvorove;
class Graf;
class ShortestPath: public AlgoritamBaza
{
public:
    ShortestPath(QWidget *, int,
                 const bool & = false,
                 std::string = "",
                 int = BROJ_SLUCAJNIH_OBJEKATA);

    virtual ~ShortestPath() override;

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *) const final;

    void generisiNasumicnePoligone(int);
    void ucitajIzDatoteke(const std::string);
    std::vector<Cvor*> pronadjiVidljiveSusede(Cvor *p);
    void sortirajNizPoTacki(Cvor*);
    bool vidljivCvor(Cvor *p, Cvor *wi, int i, Cvor* wi_1);
    void inicijalizujRedCvorova(Cvor* p);
    void InicijalizujDuzinuGranama();
    std::vector<Cvor*> getNajkraciPut();
    double getDuzinaNajkracegPuta();
    double povrsinaTrougla(QPointF, QPointF, QPointF);

private:
    Cvor* _pocetak, *_kraj;
    std::vector<Cvor*> _cvorovi_za_graf;
    std::vector<PoluIvica*> _ivice_za_graf;
    std::vector<Poligon*> _poligoni;
    std::set<PoluIvica*, PoredjenjeZaStabloPretrage>* _stablo_pretrage;
    PoluIvica* _prava_za_poredjene;
    std::vector<Cvor*> _najkraci_put;
    double _duzina;
};

enum class TipCvora{
    NEPOSECEN,
    VIDLJIV,
    NEVIDLJIV,
    POSECEN
};

class Cvor{
public:
    Cvor();
    Cvor(const float x, const float y);
    Cvor(const QPointF& koordinate, PoluIvica* susednaIvica=nullptr);

    float x();
    float y();
    const QPointF& koordinate() const;
    void postaviKoordinate(const QPointF& koordinate);
    PoluIvica* susednaIvica() const;
    void postaviSusednuIvicu(PoluIvica* susednaIvica);
    TipCvora tip() const;
    void postaviTipCvora(TipCvora tip);
    void postaviIdPoligona(int id);
    int id_poligona(){return _id_poligona;}
    void postaviSledeciCvor(Cvor*);

private:
    QPointF _koordinate;
    PoluIvica* _susednaIvica;
    TipCvora _tip;
    int _id_poligona;
    Cvor* _sledeci;
};

class PoluIvica{
public:
    PoluIvica();
    PoluIvica(Cvor* origin, PoluIvica* tvin=nullptr, PoluIvica* sledeca = nullptr, PoluIvica* prethodna = nullptr);
    Cvor* origin() const;
    void postaviOrigin(Cvor* origin);
    PoluIvica* tvin() const;
    void postaviTvin(PoluIvica* tvin);
    PoluIvica* sledeca() const;
    void postaviSledecu(PoluIvica* sledeca);
    PoluIvica* prethodna() const;
    void postaviPrethodnu(PoluIvica* prethodna);
    double duzinaPoluIvice();
    void izracunajDuzinu();
    bool levo(PoluIvica* p1);
    bool desno(PoluIvica* p1);

    double povrsinaTrougla(QPointF, QPointF, QPointF);
private:
    Cvor* _origin;
    PoluIvica* _tvin;
    PoluIvica* _sledeca;
    PoluIvica* _prethodna;
    double _duzina;
};

class Poligon{
public:
    Poligon(){}

    virtual ~Poligon();
    Cvor* cvor(int i) const;
    const std::vector<Cvor*> &cvorovi() const;
    void dodajCvor(Cvor* c);
    PoluIvica* poluIvica(int i) const;
    const std::vector<PoluIvica*> &ivice() const;
    void dodajPoluIvicu(PoluIvica* c);
    PoluIvica* pronadjiIvicu(Cvor* pocetak, Cvor* kraj);
    bool pronadjiPresekSaLinijom(PoluIvica*);
    PoluIvica* pronadjiIvicuPoCvoru(Cvor*);

private:
    std::vector<Cvor*> _cvorovi;
    std::vector<PoluIvica*> _ivice;
};

struct PoredjenjeZaStabloPretrage{
private:
    PoluIvica** _prava_za_poredjenje;
public:
    PoredjenjeZaStabloPretrage(PoluIvica **prava = nullptr):_prava_za_poredjenje(prava){}

    bool operator()(PoluIvica* prva, PoluIvica* druga);
};

struct PoredjenjeZaCvorove{
private:
    Cvor** _cvor_za_poredjenje;
public:
    PoredjenjeZaCvorove(Cvor** cvor = nullptr);

    bool operator()(Cvor* c1, Cvor* p2);

};

class Graf{
public:
    Graf(std::vector<Cvor*>, std::vector<PoluIvica*>);

    std::vector<Cvor*> najkraciPut(Cvor*, Cvor*);

    double duzina(Cvor* c){ return _duzine_najkracih_puteva_do_cvora[c];}
private:
    std::map<Cvor*, std::vector<std::pair<Cvor*, double>>> _susedi;
    std::list<Cvor*> _svi_cvorovi_grafa;
    std::map<Cvor*, double> _duzine_najkracih_puteva_do_cvora;
    std::map<Cvor*, Cvor*> _roditelji;
};
#endif // SHORTESTPATH_H
