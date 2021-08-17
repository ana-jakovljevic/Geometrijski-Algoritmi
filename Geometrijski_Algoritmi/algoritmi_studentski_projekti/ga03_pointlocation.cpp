#include "ga03_pointlocation.h"

pointlocation::pointlocation(QWidget *pCrtanje,
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
          _velicina = _tacke.size();
      }
void pointlocation::pokreniAlgoritam(){
    //odredjujemo prvo segmente

}
void pointlocation::crtajAlgoritam(QPainter *) const{


}
void pointlocation::pokreniNaivniAlgoritam(){

}

void pointlocation::crtajNaivniAlgoritam(QPainter *) const {

}
