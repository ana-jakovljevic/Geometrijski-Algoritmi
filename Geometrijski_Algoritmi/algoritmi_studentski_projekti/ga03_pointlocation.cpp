#include "ga03_pointlocation.h"

ga03_pointlocation::ga03_pointlocation(QWidget *pCrtanje,
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
          velicina = _tacke.size();
      }
void ga03_pointlocation::pokreniAlgoritam(){

}
void ga03_pointlocation::crtajAlgoritam(QPainter *) const{


}
void ga03_pointlocation::pokreniNaivniAlgoritam(){

}

void ga03_pointlocation::crtajNaivniAlgoritam(QPainter *) const {

}
