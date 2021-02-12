#include "ga09_klasterovanje.h"
#include "pomocnefunkcije.h"

#include <algorithm>
#include <iostream>
#include <vector>

Klasterovanje::Klasterovanje(QWidget *pCrtanje,
                            int pauzaKoraka,
                            const bool &naivni,
                            std::string imeDatoteke,
                            int brojTacaka)
   : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{

   if (imeDatoteke == "")
       _tacke = generisiNasumicneTacke(brojTacaka);
   else
       _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
   _k = _tacke.size();
}

void Klasterovanje::pokreniAlgoritam()
{
   if (_k <= 3){
       _maksimalnoRastojanje = 0;
       return;
   }
   sortirajNizove();

   for (unsigned long i = 0; i < _k; i++) {
       for (unsigned long j = 0; j < _k; j++) {

           if(i != j){
               double trenutnoRastojanje = izracunajDistancuSkupova(i, j);
               AlgoritamBaza_updateCanvasAndBlock()

               if (trenutnoRastojanje > _maksimalnoRastojanje){
                   _maksimalnoRastojanje = trenutnoRastojanje;
                   _klasteri = _trenutniKlasteri;
               }
           }
       }
   }

   _trenutniKlasteri = _klasteri;
   AlgoritamBaza_updateCanvasAndBlock()
   emit animacijaZavrsila();
}

void Klasterovanje::crtajAlgoritam(QPainter *painter) const {
   crtanje(painter, _trenutniKlasteri);
}

void Klasterovanje::pokreniNaivniAlgoritam()
{

   if (_k <= 3){
       _maksimalnoRastojanje = 0;
       return;
   }

   for (unsigned long i = 0; i < _k; i++) {
       for (unsigned long j = 0; j < _k; j++) {

           if(i != j){
               double rastojanje = izracunajDistancuSkupovaNaivni(i,j);
               AlgoritamBaza_updateCanvasAndBlock()
               if (rastojanje > _maksimalnoRastojanjeNaivnog){
                   _maksimalnoRastojanjeNaivnog = rastojanje;
                   _klasteriNaivnog = _trenutniKlasteriNaivnog;
               }
           }
       }
   }

   _trenutniKlasteriNaivnog = _klasteriNaivnog;
   AlgoritamBaza_updateCanvasAndBlock()
   emit animacijaZavrsila();
}

void Klasterovanje::crtajNaivniAlgoritam(QPainter *painter) const
{
   crtanje(painter, _trenutniKlasteriNaivnog);
}

void Klasterovanje::crtanje(QPainter *painter, const Klasteri& klasteri) const
{
   if (!painter) return;

   auto pen = painter->pen();

   //Crtanje konveksnih omotaca
   pen.setWidth(5);
   pen.setColor(Qt::blue);
   painter->setPen(pen);
   for(auto i = 1ul; i < klasteri._prviKonveksniOmotac.size(); i++) {
      painter->drawLine(klasteri._prviKonveksniOmotac[i-1], klasteri._prviKonveksniOmotac[i]);
   }
   pen.setColor(Qt::blue);
   painter->setPen(pen);
   for(auto i = 1ul; i < klasteri._drugiKonveksniOmotac.size(); i++) {
      painter->drawLine(klasteri._drugiKonveksniOmotac[i-1], klasteri._drugiKonveksniOmotac[i]);
   }

   //Crtanje duzi na osnovu koje razdvajamo skupove
   pen.setWidth(7);
   pen.setColor(Qt::green);
   painter->setPen(pen);
   painter->drawLine(klasteri._prvaTacka, klasteri._drugaTacka);

   //Crtanje duzi koja je rastojanje omotaca
   pen.setWidth(8);
   pen.setColor(Qt::black);
   painter->setPen(pen);
   painter->drawLine(klasteri._tackaPrvogNiza, klasteri._tackaDrugogNiza);

   //Crtanje tacaka skupova
   pen.setWidth(10);
   pen.setColor(Qt::red);
   painter->setPen(pen);
   for (auto &tacka: klasteri._prviNiz){
       painter->drawPoint(tacka);
   }
   pen.setColor(Qt::yellow);
   painter->setPen(pen);
   for (auto &tacka: klasteri._drugiNiz){
       painter->drawPoint(tacka);
   }
}

//Prva faza Gremovog algoritma za maksimalnu tacku sa odredjenim indeksom
//Pre sortiranja moramo da izbacimo sve tacke koje su levo od date tacke inace sortiranje po uglovima nece raditi
void Klasterovanje::sortirajPoTacki(unsigned long indeks)
{
   QPoint maxTacka = _tacke[indeks];
   std::vector<QPoint> sortiraniNiz = {};

   for (auto point : _tacke){
       if (!(point.x() > maxTacka.x() || (point.x() == maxTacka.x() && point.y() < maxTacka.y())))
           sortiraniNiz.push_back(point);
   }

   std::sort(sortiraniNiz.begin(), sortiraniNiz.end(), [&](const auto& lhs, const auto& rhs) {
       int P = pomocneFunkcije::povrsinaTrougla(maxTacka, lhs, rhs);
       return  (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadrat(maxTacka, lhs)
                            < pomocneFunkcije::distanceKvadrat(maxTacka, rhs));
   });
   _sortiraniNizovi.emplace(indeks, sortiraniNiz);
}

//Pripremanje prve faze Gremovog algoritma posmatrajuci svaku tacku kao maksimalnu
void Klasterovanje::sortirajNizove()
{

   for(unsigned long i = 0; i < _k; i++){
       sortirajPoTacki(i);
   }
}

//Za date dve tacke razdvajamo tacke na one levo i desno od prave koju oni cine
//Zatim pravimo konveksne omotace tih skupova tacaka
//Onda trazimo tangente tih omotaca
//Trazimo minimalno rastojanje dve duzi tih omotaca tako sto proveravamo sledece kombinacije duzi:
//trenutna prvog i drugog omotaca, trenutna prvog i sledeca drugog, trenutna prvog i prethodna drugog...
//Na kraju auzirramo trenutne klastere, crtamo ih i vracamo rastojanje ta dva omotaca
double Klasterovanje::izracunajDistancuSkupova(unsigned long i, unsigned long j)
{
   QPoint prvaTacka = _tacke[i];
   QPoint drugaTacka = _tacke[j];

   std::vector<QPoint> prviNiz = {};
   std::vector<QPoint> drugiNiz = {};

   for (unsigned long l = 0; l < _k; l++) {

       QPoint trenutnaTacka = _tacke[l];
       int P = pomocneFunkcije:: povrsinaTrougla(prvaTacka, drugaTacka, trenutnaTacka);
       if (P <= 0) {
           prviNiz.push_back(trenutnaTacka);
       } else {
           drugiNiz.push_back(trenutnaTacka);
       }
   }

   if (prviNiz.size() <= 1 || drugiNiz.size() <= 1)
       return 0;

   std::vector<QPoint> prviKonveksniOmotac = nadjiKonveksniOmotac(prviNiz, prvaTacka, drugaTacka, true);
   std::vector<QPoint> drugiKonveksniOmotac = nadjiKonveksniOmotac(drugiNiz, prvaTacka, drugaTacka, false);

   unsigned long vlPrvog;
   unsigned long vrPrvog;
   unsigned long vlDrugog;
   unsigned long vrDrugog;

   nadjiTangente(drugiKonveksniOmotac[0], prviKonveksniOmotac, vlPrvog, vrPrvog);
   nadjiTangente(prviKonveksniOmotac[0], drugiKonveksniOmotac, vlDrugog, vrDrugog);

   std::vector<QPoint> prviPodniz = podniz(prviKonveksniOmotac, vrPrvog, vlPrvog);
   std::vector<QPoint> drugiPodniz = podniz(drugiKonveksniOmotac, vrDrugog, vlDrugog);

   unsigned long indeksPrvogOmotaca = 0;
   unsigned long indeksDrugogOmotaca = 0;
   unsigned long noviIndeksPrvogOmotaca = 0;
   unsigned long noviIndeksDrugogOmotaca = 0;
   double minRastojanje = pomocneFunkcije::distanceKvadrat(prviPodniz[0], drugiPodniz[0]);
   QPoint tackaPrvogPodniza = prviPodniz[0];
   QPoint tackaDrugogPodniza = drugiPodniz[0];

   while(true){
       QPoint trazenaTacka1;
       QPoint trazenaTacka2;
       double rastojanjeDveTacke;

       if(indeksDrugogOmotaca > 0){
           if(indeksPrvogOmotaca > 0){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca], prviPodniz[indeksPrvogOmotaca-1],
                           drugiPodniz[indeksDrugogOmotaca], drugiPodniz[indeksDrugogOmotaca-1],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca - 1;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca - 1;
               }
           }
           if(indeksPrvogOmotaca < prviPodniz.size() - 1){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca], prviPodniz[indeksPrvogOmotaca+1],
                           drugiPodniz[indeksDrugogOmotaca], drugiPodniz[indeksDrugogOmotaca-1],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca - 1;
               }
           }
           if(indeksPrvogOmotaca < prviPodniz.size() - 2){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca+1], prviPodniz[indeksPrvogOmotaca+2],
                           drugiPodniz[indeksDrugogOmotaca], drugiPodniz[indeksDrugogOmotaca-1],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca + 1;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca - 1;
               }
           }
       }
       if(indeksDrugogOmotaca < drugiPodniz.size() - 1){

           if(indeksPrvogOmotaca > 0){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca], prviPodniz[indeksPrvogOmotaca-1],
                           drugiPodniz[indeksDrugogOmotaca], drugiPodniz[indeksDrugogOmotaca+1],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca - 1;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca + 1;
               }
           }
           if(indeksPrvogOmotaca < prviPodniz.size() - 1){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca], prviPodniz[indeksPrvogOmotaca+1],
                           drugiPodniz[indeksDrugogOmotaca], drugiPodniz[indeksDrugogOmotaca+1],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca + 1;
               }
           }
           if(indeksPrvogOmotaca < prviPodniz.size() - 2){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca+1], prviPodniz[indeksPrvogOmotaca+2],
                           drugiPodniz[indeksDrugogOmotaca], drugiPodniz[indeksDrugogOmotaca+1],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca + 1;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca + 1;
               }
           }
       }
       if(indeksDrugogOmotaca < drugiPodniz.size() - 2){
           if(indeksPrvogOmotaca > 0){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca], prviPodniz[indeksPrvogOmotaca-1],
                           drugiPodniz[indeksDrugogOmotaca+1], drugiPodniz[indeksDrugogOmotaca+2],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca - 1;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca + 1;
               }
           }
           if(indeksPrvogOmotaca < prviPodniz.size() - 1){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca], prviPodniz[indeksPrvogOmotaca+1],
                           drugiPodniz[indeksDrugogOmotaca+1], drugiPodniz[indeksDrugogOmotaca+2],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca + 1;
               }
           }
           if(indeksPrvogOmotaca < prviPodniz.size() - 2){
               rastojanjeDveTacke = rastojanjeDveDuzi(
                           prviPodniz[indeksPrvogOmotaca+1], prviPodniz[indeksPrvogOmotaca+2],
                           drugiPodniz[indeksDrugogOmotaca+1], drugiPodniz[indeksDrugogOmotaca+2],
                           trazenaTacka1, trazenaTacka2);
               if(rastojanjeDveTacke < minRastojanje){
                   minRastojanje = rastojanjeDveTacke;
                   tackaPrvogPodniza = trazenaTacka1;
                   tackaDrugogPodniza = trazenaTacka2;
                   noviIndeksPrvogOmotaca = indeksPrvogOmotaca + 1;
                   noviIndeksDrugogOmotaca = indeksDrugogOmotaca + 1;
               }
           }
       }
       if(indeksPrvogOmotaca == noviIndeksPrvogOmotaca && indeksDrugogOmotaca == noviIndeksDrugogOmotaca){
           break;
       }
       indeksPrvogOmotaca = noviIndeksPrvogOmotaca;
       indeksDrugogOmotaca = noviIndeksDrugogOmotaca;
   }

   prviKonveksniOmotac.push_back(prviKonveksniOmotac[0]);
   drugiKonveksniOmotac.push_back(drugiKonveksniOmotac[0]);

   _trenutniKlasteri = Klasteri(prvaTacka, drugaTacka, tackaPrvogPodniza, tackaDrugogPodniza, prviNiz, drugiNiz, prviKonveksniOmotac, drugiKonveksniOmotac);

   return minRastojanje;
}

//Za date dve tacke razdvajamo tacke na one levo i desno od prave koju oni cine
//Zatim pravimo konveksne omotace tih skupova tacaka
//Onda racunamo sva moguca rastojanja izmedju tacke jednog i druzi drugog omotaca
//Na kraju auzirramo trenutne klastere, crtamo ih i vracamo rastojanje ta dva omotaca
double Klasterovanje::izracunajDistancuSkupovaNaivni(unsigned long i, unsigned long j)
{
   QPoint prvaTacka = _tacke[i];
   QPoint drugaTacka = _tacke[j];

   std::vector<QPoint> prviNiz = {};
   std::vector<QPoint> drugiNiz = {};

   for (unsigned long l = 0; l < _k; l++) {

       QPoint trenutnaTacka = _tacke[l];
       int P = pomocneFunkcije:: povrsinaTrougla(prvaTacka, drugaTacka, trenutnaTacka);
       if (P <= 0) {
           prviNiz.push_back(trenutnaTacka);
       } else {
           drugiNiz.push_back(trenutnaTacka);
       }
   }

   if (prviNiz.size() <= 1 || drugiNiz.size() <= 1)
       return 0;

   std::vector<QPoint> prviKonveksniOmotac = nadjiKonveksniOmotacNaivni(prviNiz);
   std::vector<QPoint> drugiKonveksniOmotac = nadjiKonveksniOmotacNaivni(drugiNiz);

   double rastojanje = pomocneFunkcije::distanceKvadrat(prviKonveksniOmotac[0], drugiKonveksniOmotac[0]);
   QPoint tackaPrvogOmotaca = prviKonveksniOmotac[0];
   QPoint tackaDrugogOmotaca = drugiKonveksniOmotac[0];
   unsigned long velicinaPrvogOmotaca = prviKonveksniOmotac.size() - 1;
   unsigned long velicinaDrugogOmotaca = drugiKonveksniOmotac.size() - 1;
   for (unsigned long k = 0; k < velicinaPrvogOmotaca; k++) {
       for (unsigned long l = 0; l < velicinaDrugogOmotaca; l++) {

           QPoint trazenaTacka;
           double rastojanjeDveTacke = rastojanjeTackeOdDuzi(prviKonveksniOmotac[k], drugiKonveksniOmotac[l], drugiKonveksniOmotac[(l+1)%velicinaDrugogOmotaca], trazenaTacka);
           if (rastojanjeDveTacke < rastojanje){

               rastojanje = rastojanjeDveTacke;
               tackaPrvogOmotaca = prviKonveksniOmotac[k];
               tackaDrugogOmotaca = trazenaTacka;
           }
           rastojanjeDveTacke = rastojanjeTackeOdDuzi(drugiKonveksniOmotac[l], prviKonveksniOmotac[k], prviKonveksniOmotac[(k+1)%velicinaPrvogOmotaca], trazenaTacka);
           if (rastojanjeDveTacke < rastojanje){

               rastojanje = rastojanjeDveTacke;
               tackaPrvogOmotaca = trazenaTacka;
               tackaDrugogOmotaca = drugiKonveksniOmotac[l];
           }

       }

   }

   _trenutniKlasteriNaivnog = Klasteri(prvaTacka, drugaTacka, tackaPrvogOmotaca, tackaDrugogOmotaca, prviNiz, drugiNiz, prviKonveksniOmotac, drugiKonveksniOmotac);

   return rastojanje;
}

//Druga faza Gremovog algoritma
//Uzimamo onaj sortirani niz iz mape koji nam odgovara po maksimalnoj tacki skupa
//Takodje izbacujemo tacke koje se ponavljaju i tacke koje su kolinearne sa maksimalnom tackom (nisu nam uopste potrebne za racunanja)
std::vector<QPoint> Klasterovanje::nadjiKonveksniOmotac(std::vector<QPoint> niz, const QPoint& prvaTacka, const QPoint& drugaTacka, bool daLiJePrviNiz)
{

   QPoint maksTacka = niz[0];

   for (auto l = 1ul; l < niz.size(); l++) {
       if (niz[l].x() > maksTacka.x() || (niz[l].x() == maksTacka.x() && niz[l].y() < maksTacka.y()))
           maksTacka = niz[l];
   }

   auto iter = std::find(_tacke.cbegin(), _tacke.cend(), maksTacka);
   std::vector<QPoint> sortiraneZaPrviNiz = _sortiraniNizovi[static_cast<unsigned long>(std::distance(_tacke.cbegin(), iter))];

   auto o = 0ul;
   for(auto l = 0ul; l < sortiraneZaPrviNiz.size(); l++){

       QPoint trenutnaTacka = sortiraneZaPrviNiz[l];
       int P = pomocneFunkcije:: povrsinaTrougla(prvaTacka, drugaTacka, trenutnaTacka);
       if ((P <= 0 && daLiJePrviNiz) || (P > 0 && !daLiJePrviNiz)) {
           niz[o] = trenutnaTacka;
           o++;
       }
   }

   unsigned p = 1;
   while(p < niz.size()){
       if(niz[p] == niz[p-1]){
           niz.erase(niz.begin()+p-1);
       } else {
           p++;
       }
   }

   std::vector<QPoint> konveksniOmotac;
   konveksniOmotac.push_back(maksTacka);
   konveksniOmotac.push_back(niz[1]);
   unsigned pom = 2;
   p = 2;

   while(p < niz.size()) {
       if(pomocneFunkcije::povrsinaTrougla(konveksniOmotac[pom-2],
                                           konveksniOmotac[pom-1],
                                           niz[p]) <= 0)
       {
           konveksniOmotac.push_back(niz[p]);
           ++pom;
           ++p;
       }
       else {
           konveksniOmotac.pop_back();
           --pom;
       }
   }

   p = 3;
   while(p < konveksniOmotac.size()){
       if(pomocneFunkcije::povrsinaTrougla(konveksniOmotac[0], konveksniOmotac[p-1], konveksniOmotac[p-2]) == 0){
           if(pomocneFunkcije::distanceKvadrat(konveksniOmotac[0], konveksniOmotac[p-1]) < pomocneFunkcije::distanceKvadrat(konveksniOmotac[0], konveksniOmotac[p-2])){
               konveksniOmotac.erase(konveksniOmotac.begin()+p-1);
           } else {
               konveksniOmotac.erase(konveksniOmotac.begin()+p-2);
           }
       } else {
           p++;
       }
   }

   return konveksniOmotac;
}

//Obicni Gremov algoritam samo sto na kraju izbacujemo tacke koje su kolinearne sa maksimalnom tackom
std::vector<QPoint> Klasterovanje::nadjiKonveksniOmotacNaivni(std::vector<QPoint> niz)
{

   QPoint maksTacka = niz[0];

   for (auto l = 1ul; l < niz.size(); l++) {
       if (niz[l].x() > maksTacka.x() || (niz[l].x() == maksTacka.x() && niz[l].y() < maksTacka.y()))
           maksTacka = niz[l];
   }

   std::sort(niz.begin(), niz.end(), [&](const auto& lhs, const auto& rhs) {
       int P = pomocneFunkcije::povrsinaTrougla(maksTacka, lhs, rhs);
       return  (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadrat(maksTacka, lhs)
                            < pomocneFunkcije::distanceKvadrat(maksTacka, rhs));
   });

   std::vector<QPoint> konveksniOmotac;
   konveksniOmotac.push_back(maksTacka);
   konveksniOmotac.push_back(niz[1]);
   unsigned pom = 2;
   unsigned p = 2;

   while(p < niz.size()) {

       if(pomocneFunkcije::povrsinaTrougla(konveksniOmotac[pom-2],
                                           konveksniOmotac[pom-1],
                                           niz[p]) <= 0)
       {
           konveksniOmotac.push_back(niz[p]);
           ++pom;
           ++p;
       }
       else {
           konveksniOmotac.pop_back();
           --pom;
       }
   }

   p = 3;
   while(p < konveksniOmotac.size()){


       if(pomocneFunkcije::povrsinaTrougla(konveksniOmotac[0], konveksniOmotac[p-1], konveksniOmotac[p-2]) == 0){
           if(pomocneFunkcije::distanceKvadrat(konveksniOmotac[0], konveksniOmotac[p-1]) < pomocneFunkcije::distanceKvadrat(konveksniOmotac[0], konveksniOmotac[p-2])){
               konveksniOmotac.erase(konveksniOmotac.begin()+p-1);
           } else {
               konveksniOmotac.erase(konveksniOmotac.begin()+p-2);
           }
       } else {
           p++;
       }
   }

   konveksniOmotac.push_back(maksTacka);
   return konveksniOmotac;

}

//Nalazimo tangente iz maksimalne tacke jednog omotaca na drugom omotacu i pamtimo tacke preseka
void Klasterovanje::nadjiTangente(QPoint tacka, std::vector<QPoint> konveksniOmotac, unsigned long& vl, unsigned long& vr)
{
   unsigned long levaTacka = 0;
   unsigned long desnaTacka = 0;

   unsigned long velicinaOmotaca = konveksniOmotac.size();

   for (auto i = 0ul; i < velicinaOmotaca; i++) {

       double eprev = pomocneFunkcije:: povrsinaTrougla(tacka, konveksniOmotac[(i-1)%velicinaOmotaca], konveksniOmotac[i]);
       double enext = pomocneFunkcije:: povrsinaTrougla(tacka, konveksniOmotac[i], konveksniOmotac[(i+1)%velicinaOmotaca]);
       if(eprev < 0  && enext >= 0){
           desnaTacka = i;
       } else if(eprev >= 0 && enext < 0){
           levaTacka = i;
       }
   }

   vl = levaTacka;
   vr = desnaTacka;

}

//Pravimo podniz konveksnog omotaca na osnovu tacaka koje su preseci omotaca i tangenti
std::vector<QPoint> Klasterovanje::podniz(std::vector<QPoint> niz, unsigned long pocetak, unsigned long kraj)
{

   std::vector<QPoint> noviNiz;
   for(auto i = pocetak; i != kraj; i = (i+1)%niz.size()){
       noviNiz.push_back(niz[i]);
   }
   noviNiz.push_back(niz[kraj]);
   return noviNiz;

}

//Racunanje rastojanja jedne tacke od duzi koja je data kao dve tacke
double Klasterovanje::rastojanjeTackeOdDuzi(const QPoint &tacka, const QPoint &tackaDuzi1, const QPoint &tackaDuzi2, QPoint& trazenaTacka)
{

   double AB_b = tackaDuzi1.x() - tackaDuzi2.x();
   double AB_a = tackaDuzi2.y() - tackaDuzi1.y();

   double AC_b = tackaDuzi1.x() - tacka.x();
   double AC_a = tacka.y() - tackaDuzi1.y();

   double BC_b = tackaDuzi2.x() - tacka.x();
   double BC_a = tacka.y() - tackaDuzi2.y();

   double BA_b = -AB_b;
   double BA_a = -AB_a;



   if (AB_a*AC_a + AB_b*AC_b <= 0){
       trazenaTacka = tackaDuzi1;
       return pomocneFunkcije::distanceKvadrat(tacka, tackaDuzi1);

   } else if (BC_a*BA_a + BC_b*BA_b <= 0) {
       trazenaTacka = tackaDuzi2;
       return pomocneFunkcije::distanceKvadrat(tacka, tackaDuzi2);

   } else {

       if(abs(AB_b) < 0.00001){
           trazenaTacka = QPoint(tackaDuzi1.x(), tacka.y());
           return (abs(tackaDuzi1.x() - tacka.x()))*(abs(tackaDuzi1.x() - tacka.x()));
       }

       if(abs(AB_a) < 0.00001){
           trazenaTacka = QPoint(tacka.x(), tackaDuzi1.y());
           return (abs(tackaDuzi1.y() - tacka.y()))*(abs(tackaDuzi1.y() - tacka.y()));
       }

       double AB_c = -AB_a*tackaDuzi1.x() -AB_b*tackaDuzi1.y();

       double a = -AB_b;
       double b = AB_a;
       double c = -a*tacka.x() - b*tacka.y();


       double y = (AB_a*c-a*AB_c)/(a*AB_b-AB_a*b);
       double x = (-y*b-c)/a;

       trazenaTacka = QPoint(int(round(x)), int(round(y)));
       return pomocneFunkcije::distanceKvadrat(tacka, trazenaTacka);


   }
}

//Racunanje rastojanja dve duzi tako sto racunamo sve kombinacije rastojanja jedne tacke sa jednom duzi
double Klasterovanje::rastojanjeDveDuzi(const QPoint &tackaPrveDuzi1, const QPoint &tackaPrveDuzi2, const QPoint &tackaDrugeDuzi1, const QPoint &tackaDrugeDuzi2, QPoint &trazenaTacka1, QPoint &trazenaTacka2)
{

   QPoint trazenaTacka;
   double rastojanje = rastojanjeTackeOdDuzi(tackaPrveDuzi1, tackaDrugeDuzi1, tackaDrugeDuzi2, trazenaTacka);
   trazenaTacka1 = tackaPrveDuzi1;
   trazenaTacka2 = trazenaTacka;
   double trenutnoRastojanje = rastojanjeTackeOdDuzi(tackaPrveDuzi2, tackaDrugeDuzi1, tackaDrugeDuzi2, trazenaTacka);
   if (trenutnoRastojanje < rastojanje){
       rastojanje = trenutnoRastojanje;
       trazenaTacka1 = tackaPrveDuzi2;
       trazenaTacka2 = trazenaTacka;
   }
   trenutnoRastojanje = rastojanjeTackeOdDuzi(tackaDrugeDuzi1, tackaPrveDuzi1, tackaPrveDuzi2, trazenaTacka);
   if (trenutnoRastojanje < rastojanje){
       rastojanje = trenutnoRastojanje;
       trazenaTacka1 = trazenaTacka;
       trazenaTacka2 = tackaDrugeDuzi1;
   }
   trenutnoRastojanje = rastojanjeTackeOdDuzi(tackaDrugeDuzi2, tackaPrveDuzi1, tackaPrveDuzi2, trazenaTacka);
   if (trenutnoRastojanje < rastojanje){
       rastojanje = trenutnoRastojanje;
       trazenaTacka1 = trazenaTacka;
       trazenaTacka2 = tackaDrugeDuzi2;
   }
   return rastojanje;

}

double Klasterovanje::maksimalnoRastojanjeNaivnog() const
{
   return _maksimalnoRastojanjeNaivnog;
}

double Klasterovanje::maksimalnoRastojanje() const
{
   return _maksimalnoRastojanje;
}
