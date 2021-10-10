#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H
#include <set>
#include <list>
#include <float.h>
#include"algoritambaza.h"
#include "pomocnefunkcije.h"
class CvorD;
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
    std::vector<CvorD*> pronadjiVidljiveSusede(CvorD *p);
    void sortirajNizPoTacki(CvorD*);
    bool vidljivCvor(CvorD *p, CvorD *wi, int i, CvorD* wi_1);
    void inicijalizujRedCvorova(CvorD* p);
    void InicijalizujDuzinuGranama();
    std::vector<CvorD*> getNajkraciPut();
    double getDuzinaNajkracegPuta();
    double povrsinaTrougla(QPointF, QPointF, QPointF);

private:
    CvorD* _pocetak, *_kraj;
    std::vector<CvorD*> _cvorovi_za_graf;
    std::vector<PoluIvica*> _ivice_za_graf;
    std::vector<Poligon*> _poligoni;
    std::set<PoluIvica*, PoredjenjeZaStabloPretrage>* _stablo_pretrage;
    PoluIvica* _prava_za_poredjene;
    std::vector<CvorD*> _najkraci_put;
    double _duzina;
};

enum class TipCvora{
    NEPOSECEN,
    VIDLJIV,
    NEVIDLJIV,
    POSECEN
};

class CvorD{
public:
    CvorD();
    CvorD(const double x, const double y);
    CvorD(const QPointF& koordinate, PoluIvica* susednaIvica=nullptr);

    double x();
    double y();
    const QPointF& koordinate() const;
    void postaviKoordinate(const QPointF& koordinate);
    PoluIvica* susednaIvica() const;
    void postaviSusednuIvicu(PoluIvica* susednaIvica);
    TipCvora tip() const;
    void postaviTipCvora(TipCvora tip);
    void postaviIdPoligona(int id);
    int id_poligona(){return _id_poligona;}
    void postaviSledeciCvor(CvorD*);

private:
    QPointF _koordinate;
    PoluIvica* _susednaIvica;
    TipCvora _tip;
    int _id_poligona;
    CvorD* _sledeci;
};

class PoluIvica{
public:
    PoluIvica();
    PoluIvica(CvorD* origin, PoluIvica* tvin=nullptr, PoluIvica* sledeca = nullptr, PoluIvica* prethodna = nullptr);
    CvorD* origin() const;
    void postaviOrigin(CvorD* origin);
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
    CvorD* _origin;
    PoluIvica* _tvin;
    PoluIvica* _sledeca;
    PoluIvica* _prethodna;
    double _duzina;
};

class Poligon{
public:
    Poligon(){}

    virtual ~Poligon();
    CvorD* cvor(unsigned long  i) const;
    const std::vector<CvorD*> &cvorovi() const;
    void dodajCvor(CvorD* c);
    PoluIvica* poluIvica(unsigned long  i) const;
    const std::vector<PoluIvica*> &ivice() const;
    void dodajPoluIvicu(PoluIvica* c);
    PoluIvica* pronadjiIvicu(CvorD* pocetak, CvorD* kraj);
    bool pronadjiPresekSaLinijom(PoluIvica*) const;
    PoluIvica* pronadjiIvicuPoCvoru(CvorD*);

private:
    std::vector<CvorD*> _cvorovi;
    std::vector<PoluIvica*> _ivice;
};

struct PoredjenjeZaStabloPretrage{
private:
    PoluIvica** _prava_za_poredjenje;
public:
    PoredjenjeZaStabloPretrage(PoluIvica **prava = nullptr):_prava_za_poredjenje(prava){}

    bool operator()(PoluIvica* prva, PoluIvica* druga) const;
};

struct PoredjenjeZaCvorove{
private:
    CvorD** _cvor_za_poredjenje;
public:
    PoredjenjeZaCvorove(CvorD** cvor = nullptr);

    bool operator()(CvorD* c1, CvorD* p2);

};

class Graf{
public:
    Graf(std::vector<CvorD*>, std::vector<PoluIvica*>);

    std::vector<CvorD*> najkraciPut(CvorD*, CvorD*);

    double duzina(CvorD* c){ return _duzine_najkracih_puteva_do_cvora[c];}
private:
    std::map<CvorD*, std::vector<std::pair<CvorD*, double>>> _susedi;
    std::list<CvorD*> _svi_cvorovi_grafa;
    std::map<CvorD*, double> _duzine_najkracih_puteva_do_cvora;
    std::map<CvorD*, CvorD*> _roditelji;
};
#endif // SHORTESTPATH_H
