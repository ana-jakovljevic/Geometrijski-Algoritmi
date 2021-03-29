#ifndef GA09_KLASTEROVANJE_H
#define GA09_KLASTEROVANJE_H

#include "algoritambaza.h"

//Struktura podataka koja cuva sve informacije o jednoj podeli skupa na klastere
struct Klasteri
{
    QPoint _prvaTacka;
    QPoint _drugaTacka;
    QPoint _tackaPrvogNiza;
    QPoint _tackaDrugogNiza;

    std::vector<QPoint> _prviNiz;
    std::vector<QPoint> _drugiNiz;

    std::vector<QPoint> _prviKonveksniOmotac;
    std::vector<QPoint> _drugiKonveksniOmotac;

    Klasteri(){}

    Klasteri(QPoint prvaTacka, QPoint drugaTacka, QPoint tackaPrvogNiza, QPoint tackaDrugogNiza,
             std::vector<QPoint> prviNiz, std::vector<QPoint> drugiNiz,
             std::vector<QPoint> prviKonveksniOmotac, std::vector<QPoint> drugiKonveksniOmotac):
        _prvaTacka(prvaTacka), _drugaTacka(drugaTacka), _tackaPrvogNiza(tackaPrvogNiza), _tackaDrugogNiza(tackaDrugogNiza),
        _prviNiz(prviNiz), _drugiNiz(drugiNiz), _prviKonveksniOmotac(prviKonveksniOmotac), _drugiKonveksniOmotac(drugiKonveksniOmotac)
    {}

};


class Klasterovanje: public AlgoritamBaza
{
public:
    Klasterovanje(QWidget *pCrtanje,
                  int pauzaKoraka,
                  const bool &naivni = false,
                  std::string imeDatoteke = "",
                  int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    //Glavne funkcije
    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    void crtanje(QPainter *painter, const Klasteri& klasteri) const;

    //Funkcije za preprocesiranje u optimalnom
    //Sortiranja po tackama
    void sortirajPoTacki(unsigned long indeks);
    void sortirajNizove();

    //Racunjanje distance dva skupa
    double izracunajDistancuSkupova(unsigned long i, unsigned long j);
    double izracunajDistancuSkupovaNaivni(unsigned long i, unsigned long j);

    //Nalazanje konveksnog omotaca
    std::vector<QPoint> nadjiKonveksniOmotac(std::vector<QPoint> niz, const QPoint& prvaTacka, const QPoint& drugaTacka, bool daLiJePrviNiz);
    std::vector<QPoint> nadjiKonveksniOmotacNaivni(std::vector<QPoint> niz);

   //Nalazenje tangenti i podnizova koje oni cine
   void nadjiTangente(QPoint tacka, std::vector<QPoint> konveksniOmotac, unsigned long& vl, unsigned long& vr);
   std::vector<QPoint> podniz(std::vector<QPoint> niz, unsigned long pocetak, unsigned long kraj);

   //Geometrijska rastojanja
    double rastojanjeTackeOdDuzi(const QPoint& tacka, const QPoint& tackaDuzi1, const QPoint& tackaDuzi2, QPoint& trazenaTacka);
    double rastojanjeDveDuzi(const QPoint& tackaPrveDuzi1, const QPoint& tackaPrveDuzi2, const QPoint& tackaDrugeDuzi1, const QPoint& tackaDrugeDuzi2, QPoint& trazenaTacka1, QPoint& trazenaTacka2);

    //Geteri
    double maksimalnoRastojanje() const;
    double maksimalnoRastojanjeNaivnog() const;

private:

    void naglasiTrenutno(QPainter *painter, unsigned long i, const char *s) const;

    std::vector<QPoint> _tacke;
    std::map<unsigned long, std::vector<QPoint>> _sortiraniNizovi;

    Klasteri _klasteri;
    Klasteri _klasteriNaivnog;

    Klasteri _trenutniKlasteri;
    Klasteri _trenutniKlasteriNaivnog;

    double _maksimalnoRastojanje = 0;
    double _maksimalnoRastojanjeNaivnog = 0;

    unsigned long _k;
};

#endif // GA09_KLASTEROVANJE_H

