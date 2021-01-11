#include "ga18_shortestpath.h"
#include <fstream>
ShortestPath::ShortestPath(QWidget *pCrtanje, int pauzaKorak, const bool &naivni, std::string imeDatoteke, int brojPoligona)
    :AlgoritamBaza(pCrtanje, pauzaKorak, naivni){
    if(imeDatoteke==""){
        generisiNasumicnePoligone(brojPoligona);
    }else
        ucitajIzDatoteke(imeDatoteke);

   _prava_za_poredjene = new PoluIvica();
    PoredjenjeZaStabloPretrage pzsp(&_prava_za_poredjene);
    _stablo_pretrage = new std::set<PoluIvica*, PoredjenjeZaStabloPretrage>(pzsp);

}
ShortestPath::~ShortestPath(){

    for(auto p: _poligoni)
        delete p;

}

void ShortestPath::pokreniAlgoritam(){

    _cvorovi_za_graf.push_back(_pocetak);

    for(unsigned i=0; i<_poligoni.size(); i++)
    {
        for(auto& c: _poligoni[i]->cvorovi()){
            _cvorovi_za_graf.push_back(c);
        }
    }

    _cvorovi_za_graf.push_back(_kraj);


    for(unsigned i= 0; i<_cvorovi_za_graf.size(); i++){

        std::vector<Cvor*> vidljivi_susedi_za_cvor = pronadjiVidljiveSusede(_cvorovi_za_graf[i]);

        for(unsigned j=0; j<vidljivi_susedi_za_cvor.size(); j++){
            PoluIvica* pwi = new PoluIvica(_cvorovi_za_graf[i]);
            PoluIvica* wip = new PoluIvica(vidljivi_susedi_za_cvor[j], pwi);
            pwi->postaviTvin(wip);
            _ivice_za_graf.push_back(pwi);
            _ivice_za_graf.push_back(wip);
            AlgoritamBaza_updateCanvasAndBlock();

        }
    }
    /* TODO:
     *
     *  Preostaje da se ivicama grafa dodele tezine
     *  Napravi dijkstrin algoritam uz pomoc neke strukture
     *  Ako nema dijkstrin onda podji od prve tacke i uzimaj onaj put koji je najmanji
     *  Problem sa tim algoritmom je sto ne nalazi globalno najkraci put
     */
    AlgoritamBaza_updateCanvasAndBlock();
    emit animacijaZavrsila();
}

void ShortestPath::crtajAlgoritam(QPainter *painter) const {
    if(!painter)
        return;

    QPen pen = painter->pen();

    pen.setColor(Qt::darkGreen);

    painter->setPen(pen);

    for(unsigned i = 0; i<_poligoni.size(); i++){
        for(auto i: _poligoni[i]->ivice()){
            QLineF linija(i->origin()->koordinate(), i->sledeca()->origin()->koordinate());
            painter->drawLine(linija);
        }

    }


    pen.setColor(Qt::green);
    painter->setPen(pen);

    for(unsigned i = 0; i<_poligoni.size(); i++){
        for(auto c: _poligoni[i]->cvorovi())
            painter->drawPoint(c->koordinate());
    }
    pen.setColor(Qt::red);
    painter->setPen(pen);

    painter->drawPoint(_pocetak->koordinate());
    painter->drawPoint(_kraj->koordinate());

    pen.setColor(Qt::gray);
    painter->setPen(pen);

    for(unsigned i = 0; i<_ivice_za_graf.size(); i++){
            QLineF linija(_ivice_za_graf[i]->origin()->koordinate(), _ivice_za_graf[i]->tvin()->origin()->koordinate());
            painter->drawLine(linija);


    }


    for(unsigned i=0; i<_cvorovi_za_graf.size(); i++){
        painter->drawPoint(_cvorovi_za_graf[i]->koordinate());
    }

  /*   for(auto it = _stablo_pretrage->begin(); it!=_stablo_pretrage->end(); it++){
               QLineF linija((*it)->origin()->koordinate(), (*it)->tvin()->origin()->koordinate());
               painter->drawLine(linija);

       }

    if(_prava_za_poredjene->origin()!=nullptr){
      QLineF linijaPoredjenja(_prava_za_poredjene->origin()->koordinate(), _prava_za_poredjene->tvin()->origin()->koordinate());
      painter->drawLine(linijaPoredjenja);
    }*/
}

void ShortestPath::pokreniNaivniAlgoritam() {

    _cvorovi_za_graf.push_back(_pocetak);
    for(unsigned i=0; i<_poligoni.size(); i++)
    {
        for(auto& c: _poligoni[i]->cvorovi()){
            _cvorovi_za_graf.push_back(c);
        }
    }

    _cvorovi_za_graf.push_back(_kraj);

    for(unsigned i=0; i<_cvorovi_za_graf.size(); i++){
        for(unsigned j=i+1; j<_cvorovi_za_graf.size(); j++){
            PoluIvica *ivica = new PoluIvica(_cvorovi_za_graf[i]);
            PoluIvica *ivica_tvin = new PoluIvica(_cvorovi_za_graf[j], ivica);
            ivica->postaviTvin(ivica_tvin);
            bool ima_presek=false;
            for(unsigned k = 0; k< _poligoni.size(); k++){

                ima_presek = ima_presek || _poligoni[k]->pronadjiPresekSaLinijom(ivica);

            }

            if(ima_presek==false){
                    _ivice_za_graf.push_back(ivica);
                  //  _ivice_za_graf.push_back(ivica_tvin);
            }

        }
    }
    /* TODO:
     *
     *  Preostaje da se ivicama grafa dodele tezine
     *  Napravi dijkstrin algoritam uz pomoc neke strukture
     *  Ako nema dijkstrin onda podji od prve tacke i uzimaj onaj put koji je najmanji
     *  Problem sa tim algoritmom je sto ne nalazi globalno najkraci put
     */

    Graf* novi_graf = new Graf(_cvorovi_za_graf, _ivice_za_graf);

    _najkraci_put = novi_graf->najkraciPut(_pocetak, _kraj);
    AlgoritamBaza_updateCanvasAndBlock();

    emit animacijaZavrsila();
}

void ShortestPath::crtajNaivniAlgoritam(QPainter *painter) const{
    if(!painter)
        return;

    QPen pen = painter->pen();

    pen.setColor(Qt::darkYellow);
    painter->setPen(pen);

    for(unsigned i = 0; i<_poligoni.size(); i++){
        for(auto i: _poligoni[i]->ivice()){
            QLineF linija(i->origin()->koordinate(), i->tvin()->origin()->koordinate());
            painter->drawLine(linija);
        }

    }


    pen.setColor(Qt::yellow);
    painter->setPen(pen);

    for(unsigned i = 0; i<_poligoni.size(); i++){
        for(auto c: _poligoni[i]->cvorovi())
            painter->drawPoint(c->koordinate());
    }

    pen.setColor(Qt::magenta);
    painter->setPen(pen);

    painter->drawPoint(_pocetak->koordinate());
    painter->drawPoint(_kraj->koordinate());


/*
    for(unsigned i = 0; i<_ivice_za_graf.size(); i++){
            QLineF linija(_ivice_za_graf[i]->origin()->koordinate(), _ivice_za_graf[i]->tvin()->origin()->koordinate());
            painter->drawLine(linija);
    }*/


    pen.setColor(Qt::darkGray);
    painter->setPen(pen);
/*
   for(unsigned i = 0; i<_najkraci_put.size(); i++){
         QLineF linija(_najkraci_put[i]->origin()->koordinate(), _najkraci_put[i]->tvin()->origin()->koordinate());
         painter->drawLine(linija);
    }*/

    for(unsigned i=1; i<_najkraci_put.size(); i++){
        QLineF linija(_najkraci_put[i-1]->koordinate(), _najkraci_put[i]->koordinate());
        painter->drawLine(linija);
    }
}

void ShortestPath::generisiNasumicnePoligone(int brojPoligona){


    srand(static_cast<unsigned>(time(nullptr)));
   // double poluprecnik = brojPoligona <= 6? 50: 425*(1.0/brojPoligona);
    double poluprecnik = brojPoligona>0? 460.0/brojPoligona: 0;
    _pocetak = new Cvor(20+rand()%70, 20 + rand()%950);

    Poligon* p1 = new Poligon();
    Poligon* p2 = new Poligon();
    PoluIvica* pocetak = new PoluIvica(_pocetak);
    _pocetak->postaviSusednuIvicu(pocetak);
    pocetak->postaviPrethodnu(pocetak);
    pocetak->postaviSledecu(pocetak);
    pocetak->postaviTvin(pocetak);
    _pocetak->postaviIdPoligona(0);

    p1->dodajCvor(_pocetak);
    p1->dodajPoluIvicu(pocetak);
    _kraj = new Cvor(1400+rand()%50, 20+ rand()%950);

    PoluIvica* kraj = new PoluIvica(_kraj);
    _kraj->postaviSusednuIvicu(kraj);
    _kraj->postaviIdPoligona(brojPoligona+1);

    kraj->postaviPrethodnu(kraj);
    kraj->postaviSledecu(kraj);
    kraj->postaviTvin(kraj);

    p2->dodajCvor(_kraj);
    p2->dodajPoluIvicu(kraj);

    std::vector<QPolygonF> niz_poligona;
    std::vector<Cvor*> niz_cvorova;
    std::vector<PoluIvica*> niz_ivica;
    bool ima_presek;
    _poligoni.push_back(p1);
    for(int i= 0; i<brojPoligona; i++){

        QPolygonF poligon = QPolygonF();
        QPointF p(100+rand()%1400, 20+rand()%930);
        Poligon* poseban_poligon = new Poligon();
        for(double j=0; j<2.0*M_PI;){

            double dx = poluprecnik*cos(j);
            double dy = poluprecnik*sin(j);

            QPointF r(p.x()+dx, p.y()+dy);

            poligon.append(r);

            j+=(20.0+(rand()%40))*M_PI/180.0;

        }
        ima_presek=false;

        for(auto p: niz_poligona){
            if(p.intersects(poligon) || p.containsPoint(_pocetak->koordinate(), Qt::OddEvenFill) || p.containsPoint(_kraj->koordinate(), Qt::OddEvenFill)){
                ima_presek = true;
                break;
            }
        }
        if(ima_presek){
            i= i-1;
            continue;
        }
        niz_poligona.push_back(poligon);

        for(int p = 0; p<poligon.size(); p++){
            Cvor* c = new Cvor(poligon.at(p));

            poseban_poligon->dodajCvor(c);
            c->postaviIdPoligona(i+1);
            PoluIvica* i = new PoluIvica(c);
         //   c->postaviSusednuIvicu(i);

            poseban_poligon->dodajPoluIvicu(i);
        }

        for(unsigned p = 0; p<poseban_poligon->cvorovi().size(); p++){
            poseban_poligon->cvorovi()[p]->postaviSusednuIvicu( poseban_poligon->ivice()[p]);

            if(poseban_poligon->cvorovi().size()-1!=p)
                poseban_poligon->ivice()[p]->postaviSledecu(poseban_poligon->ivice()[p+1]);
            else
                poseban_poligon->ivice()[p]->postaviSledecu(poseban_poligon->ivice()[0]);

            if(0!=p)
                poseban_poligon->ivice()[p]->postaviPrethodnu(poseban_poligon->ivice()[p-1]);
            else
                poseban_poligon->ivice()[p]->postaviPrethodnu(poseban_poligon->ivice()[poseban_poligon->cvorovi().size()-1]);

            PoluIvica* tvin = new PoluIvica(poseban_poligon->cvorovi()[(p+1)%poseban_poligon->cvorovi().size()], poseban_poligon->ivice()[p]);
            poseban_poligon->ivice()[p]->postaviTvin(tvin);
            poseban_poligon->dodajPoluIvicu(tvin);
        }

        for(unsigned p = poseban_poligon->cvorovi().size(); p<poseban_poligon->ivice().size(); p++){
            if( poseban_poligon->ivice().size()-1!=p)
                 poseban_poligon->ivice()[p]->postaviSledecu( poseban_poligon->ivice()[p+1]);
            else
                 poseban_poligon->ivice()[p]->postaviSledecu( poseban_poligon->ivice()[ poseban_poligon->cvorovi().size()]);

            if(0!=p)
                 poseban_poligon->ivice()[p]->postaviPrethodnu( poseban_poligon->ivice()[p-1]);
            else
                 poseban_poligon->ivice()[p]->postaviPrethodnu( poseban_poligon->ivice()[ poseban_poligon->ivice().size()-1]);

        }

        /*for(auto& c: niz_cvorova){
            poseban_poligon->dodajCvor(c);
        }


        for(auto& i: niz_ivica){
            poseban_poligon->dodajPoluIvicu(i);
        }
*/
        _poligoni.push_back(poseban_poligon);

     //   niz_cvorova.clear();
       // niz_ivica.clear();
    }
//    _kraj->postaviIdPoligona();

    _poligoni.push_back(p2);
}

void ShortestPath::ucitajIzDatoteke(const std::string imeDatoteke){

    std::fstream datoteka(imeDatoteke);


    int broj_cvorova, broj_poligona;
    double px, py, kx, ky;

    datoteka >> broj_cvorova >>broj_poligona;

    datoteka>>px>>py>>kx>>ky;

    _pocetak = new Cvor(px, py);
    _kraj = new Cvor(kx, ky);

    Poligon* p1 = new Poligon();
    Poligon* p2 = new Poligon();
    PoluIvica* pocetak = new PoluIvica(_pocetak);
    _pocetak->postaviSusednuIvicu(pocetak);
    pocetak->postaviPrethodnu(pocetak);
    pocetak->postaviSledecu(pocetak);
    pocetak->postaviTvin(pocetak);
    _pocetak->postaviIdPoligona(0);

    p1->dodajCvor(_pocetak);
    p1->dodajPoluIvicu(pocetak);

    PoluIvica* kraj = new PoluIvica(_kraj);
    _kraj->postaviSusednuIvicu(kraj);
    _kraj->postaviIdPoligona(broj_poligona+1);

    kraj->postaviPrethodnu(kraj);
    kraj->postaviSledecu(kraj);
    kraj->postaviTvin(kraj);

    p2->dodajCvor(_kraj);
    p2->dodajPoluIvicu(kraj);

    std::vector<Cvor*> temp_niz_cv;
    std::vector<PoluIvica*> temp_niz_pi;
    double x, y;
    for(int i=0; i<broj_cvorova; i++){
        datoteka>>x>>y;
        Cvor* c = new Cvor(x, y);
        temp_niz_cv.push_back(c);

        PoluIvica* iv = new PoluIvica(c);

        temp_niz_pi.push_back(iv);
    }

    _poligoni.push_back(p1);
    for(int i=0; i<broj_poligona; i++){

        Poligon* novi_poligon = new Poligon();
        int n_cvorova, index;
        datoteka>>n_cvorova;
        for(int j=0; j<n_cvorova; j++){
            datoteka>>index;
            temp_niz_cv[index]->postaviIdPoligona(i+1);
            novi_poligon->dodajCvor(temp_niz_cv[index]);
            novi_poligon->dodajPoluIvicu(temp_niz_pi[index]);
        }

        for(unsigned p = 0; p<novi_poligon->cvorovi().size(); p++){
            novi_poligon->cvorovi()[p]->postaviSusednuIvicu(novi_poligon->ivice()[p]);

            if(novi_poligon->cvorovi().size()-1!=p)
                novi_poligon->ivice()[p]->postaviSledecu(novi_poligon->ivice()[p+1]);
            else
                novi_poligon->ivice()[p]->postaviSledecu(novi_poligon->ivice()[0]);

            if(0!=p)
                novi_poligon->ivice()[p]->postaviPrethodnu(novi_poligon->ivice()[p-1]);
            else
                novi_poligon->ivice()[p]->postaviPrethodnu(novi_poligon->ivice()[novi_poligon->cvorovi().size()-1]);

            PoluIvica* tvin = new PoluIvica(novi_poligon->cvorovi()[(p+1)%novi_poligon->cvorovi().size()], novi_poligon->ivice()[p]);
            novi_poligon->ivice()[p]->postaviTvin(tvin);
            novi_poligon->dodajPoluIvicu(tvin);
        }

        for(unsigned p = novi_poligon->cvorovi().size(); p<novi_poligon->ivice().size(); p++){
            if( novi_poligon->ivice().size()-1!=p)
                 novi_poligon->ivice()[p]->postaviSledecu( novi_poligon->ivice()[p+1]);
            else
                 novi_poligon->ivice()[p]->postaviSledecu( novi_poligon->ivice()[ novi_poligon->cvorovi().size()]);

            if(0!=p)
                 novi_poligon->ivice()[p]->postaviPrethodnu( novi_poligon->ivice()[p-1]);
            else
                 novi_poligon->ivice()[p]->postaviPrethodnu( novi_poligon->ivice()[ novi_poligon->ivice().size()-1]);

        }



        _poligoni.push_back(novi_poligon);
    }

    _poligoni.push_back(p2);



}

std::vector<Cvor*> ShortestPath::pronadjiVidljiveSusede(Cvor *p){

    std::vector<Cvor*> novi_niz_tacaka;
    for(auto& c: _cvorovi_za_graf){
        c->postaviTipCvora(TipCvora::NEPOSECEN);
        novi_niz_tacaka.push_back(c);
    }

    std::sort(novi_niz_tacaka.begin(), novi_niz_tacaka.end(), [&](Cvor* c1, Cvor* c2){
        QLineF l1(p->koordinate(), c1->koordinate());
        QLineF l2(p->koordinate(), c2->koordinate());
        return l1.angle()<l2.angle() || (l1.angle()==l2.angle() &&  l1.length()>l2.length());
    });


    _prava_za_poredjene = new PoluIvica(p);
    Cvor* cvor_paralelna_sa_x_osom = new Cvor(p->x()+1500, p->y());
    PoluIvica *tvin_za_poredjenje = new PoluIvica(cvor_paralelna_sa_x_osom, _prava_za_poredjene);
    _prava_za_poredjene->postaviTvin(tvin_za_poredjenje);

    QLineF duz(_prava_za_poredjene->origin()->koordinate(), _prava_za_poredjene->tvin()->origin()->koordinate());

    for(unsigned i=0; i<_poligoni.size(); i++){

        for(unsigned j=0; j<_poligoni[i]->ivice().size(); j++){
            QLineF ivica(_poligoni[i]->ivice()[j]->origin()->koordinate(), _poligoni[i]->ivice()[j]->tvin()->origin()->koordinate());
            QPointF tacka;
            if(pomocneFunkcije::presekDuzi(duz, ivica, tacka)){
                if(tacka!=duz.p1() && tacka!=duz.p2())
                  _stablo_pretrage->insert(_poligoni[i]->ivice()[j]);
            }
        }
    }
    std::vector<Cvor*> susedi;

    for(unsigned i=0; i<novi_niz_tacaka.size(); i++){
        if(novi_niz_tacaka[i] == p)
            continue;

        _prava_za_poredjene = new PoluIvica(p);
        PoluIvica *wi_ivica = new PoluIvica(novi_niz_tacaka[i], _prava_za_poredjene);
        _prava_za_poredjene->postaviTvin(wi_ivica);


        if(vidljivCvor(p, novi_niz_tacaka[i], i, novi_niz_tacaka)){

            susedi.push_back(novi_niz_tacaka[i]);



            _stablo_pretrage->insert(novi_niz_tacaka[i]->susednaIvica()->prethodna());
            _stablo_pretrage->erase(novi_niz_tacaka[i]->susednaIvica()->sledeca());

        }


    }

    return susedi;
}

bool ShortestPath::vidljivCvor(Cvor *p, Cvor *wi, int i, std::vector<Cvor*>& niz){


    PoluIvica *pwi = new PoluIvica(p);
    PoluIvica* wip = new PoluIvica(wi, pwi);
    pwi->postaviTvin(wip);

   /* if(_poligoni[wi->id_poligona()]->pronadjiPresekSaLinijom(pwi)==true){
        wi->postaviTipCvora(TipCvora::NEVIDLJIV);
        return false;
    }*/


    QLineF linija_ps(wi->susednaIvica()->prethodna()->origin()->koordinate(),wi->susednaIvica()->sledeca()->origin()->koordinate());
    QLineF linija_za_presek(pwi->origin()->koordinate(), pwi->tvin()->origin()->koordinate());
    QPointF tacka;

   if(pomocneFunkcije::presekDuzi(linija_za_presek, linija_ps, tacka)){
       if(tacka!=p->koordinate() && tacka!=wi->koordinate()){
             wi->postaviTipCvora(TipCvora::NEVIDLJIV);
             return false;
        }
    }

    int k = i-1;
    if(i==0){
        k = niz.size()-1;
    }

    if(i==0 || povrsinaTrougla(p->koordinate(), wi->koordinate(), niz[k]->koordinate())!=0){

        auto e = _stablo_pretrage->lower_bound(pwi);

        if(e!=_stablo_pretrage->end()){
            QLineF e_linija = QLineF((*e)->origin()->koordinate(), (*e)->tvin()->origin()->koordinate());
            if(pomocneFunkcije::presekDuzi(linija_za_presek, e_linija, tacka)){
                wi->postaviTipCvora(TipCvora::NEVIDLJIV);
                return false;
            }
         }else{
               wi->postaviTipCvora(TipCvora::VIDLJIV);
               return true;
         }


    }else if(niz[k]->tip()==TipCvora::NEVIDLJIV){

        wi->postaviTipCvora(TipCvora::NEVIDLJIV);
        return false;

    }else{

        PoluIvica* wiwi = new PoluIvica(wi);
        PoluIvica* wiwi_tvin = new PoluIvica(niz[k], wiwi);
        wiwi->postaviTvin(wiwi_tvin);

        auto e = _stablo_pretrage->begin();
        for(; e!=_stablo_pretrage->end(); e++){
            QLineF e_ivica((*e)->origin()->koordinate(), (*e)->tvin()->origin()->koordinate());
            QLineF wiwi_ivica(wiwi->origin()->koordinate(), wiwi->tvin()->origin()->koordinate());
            bool ind= false;
            if(pomocneFunkcije::presekDuzi(wiwi_ivica, e_ivica, tacka)){
                if(tacka!=wi->koordinate() && tacka!=niz[k]->koordinate()){
                    ind = true;
                }
            }
            if(ind) {
                break;
            }
        }

        if(e!=_stablo_pretrage->end())
          {
            wi->postaviTipCvora(TipCvora::NEVIDLJIV);
            return false;

         }else{
            wi->postaviTipCvora(TipCvora::VIDLJIV);
            return true;

        }
    }
/*        if(i==0 || povrsinaTrougla(p->koordinate(), wi->koordinate(), niz[k]->koordinate())!=0){
            auto e = _stablo_pretrage->lower_bound(pwi);

/*            auto ee = _stablo_pretrage->begin();

            while (ee != _stablo_pretrage->end()){

                QLineF e_ivica = QLineF((*ee)->origin()->koordinate(), (*ee)->tvin()->origin()->koordinate());

                QPointF tacka;

                if(pomocneFunkcije::presekDuzi(linija_za_presek, e_ivica, tacka)/* && tacka!=p->koordinate() && tacka!=wi->koordinate() && *ee < pwi){
                    break;
                }

                ee++;
            }

            if(e!=_stablo_pretrage->end()){
                QLineF e_linija((*e)->origin()->koordinate(), (*e)->tvin()->origin()->koordinate());
                if(pomocneFunkcije::presekDuzi(e_linija, linija_za_presek, tacka)){
                   // if(tacka!= p->koordinate() && tacka!=wi->koordinate()){
                        wi->postaviTipCvora(TipCvora::NEVIDLJIV);
                        return false;
                   // }
                }
            }else {
                wi->postaviTipCvora(TipCvora::VIDLJIV);
                return true;
            }
/*            if(e!=_stablo_pretrage->end()){
                QLineF e_linija((*e)->origin()->koordinate(), (*e)->tvin()->origin()->koordinate());
                if(pomocneFunkcije::presekDuzi(e_linija, linija_za_presek, tacka)){
                    if(tacka!= p->koordinate() && tacka!=wi->koordinate()){
                        wi->postaviTipCvora(TipCvora::NEVIDLJIV);
                        return false;
                    }
                }

            //}

        }else if(niz[k]->tip()==TipCvora::NEVIDLJIV){

            wi->postaviTipCvora(TipCvora::NEVIDLJIV);
            return false;

        }else{

            PoluIvica* wiwi = new PoluIvica(wi);
            PoluIvica* wiwi_tvin = new PoluIvica(niz[k], wiwi);
            wiwi->postaviTvin(wiwi_tvin);

            auto e = _stablo_pretrage->begin();
            for(; e!=_stablo_pretrage->end(); e++){
                QLineF e_ivica((*e)->origin()->koordinate(), (*e)->tvin()->origin()->koordinate());
                QLineF wiwi_ivica(wiwi->origin()->koordinate(), wiwi->tvin()->origin()->koordinate());
                bool ind= false;
                if(pomocneFunkcije::presekDuzi(wiwi_ivica, e_ivica, tacka)){
                  //  if(tacka!=wi->koordinate() && tacka!=niz[k]->koordinate()){
                        ind = true;
                   // }
                }
                if(ind) {
                    break;
                }
            }

            if(e!=_stablo_pretrage->end())
              {
                wi->postaviTipCvora(TipCvora::NEVIDLJIV);
                return false;

             }else{
                wi->postaviTipCvora(TipCvora::VIDLJIV);
                return true;

            }
        }
*/
        wi->postaviTipCvora(TipCvora::NEVIDLJIV);
        return false;

}

void ShortestPath::InicijalizujDuzinuGranama()
{
    for(auto& ivica: _ivice_za_graf){
        ivica->izracunajDuzinu();
    }
}

double ShortestPath::povrsinaTrougla(QPointF A, QPointF B, QPointF C){

    return (B.x() - A.x())*(C.y() - A.y()) - (C.x() - A.x())*(B.y() - A.y());

}

/////////////////////////////////////////////
/// \brief Cvor::Cvor
/// \param koordinate
///
Cvor::Cvor(){
    _tip = TipCvora::NEPOSECEN;
}

Cvor::Cvor(const QPointF& koordinate, PoluIvica* susednaIvica):_koordinate(koordinate),_susednaIvica(susednaIvica),
    _tip(TipCvora::NEPOSECEN){

}

Cvor::Cvor(const float x, const float y){
    _koordinate = QPointF(x, y);
    _susednaIvica = nullptr;
    _tip = TipCvora::NEPOSECEN;
}

float Cvor::x(){return _koordinate.x();}

float Cvor::y(){return _koordinate.y();}

const QPointF& Cvor::koordinate() const { return _koordinate;}

void Cvor::postaviKoordinate(const QPointF& koordinate){
    _koordinate = koordinate;
}

PoluIvica* Cvor::susednaIvica() const {
    return _susednaIvica;
}

void Cvor::postaviSusednuIvicu(PoluIvica* susednaIvica){
    _susednaIvica =  susednaIvica;
}

TipCvora Cvor::tip() const {
    return _tip;
}

void Cvor::postaviTipCvora(TipCvora tip){
    _tip = tip;
}

void Cvor::postaviIdPoligona(int id){
    _id_poligona = id;
}

void Cvor::postaviSledeciCvor(Cvor *c){
    _sledeci = c;
}
////////////////////////////////////////
/// \brief PoluIvica
///
PoluIvica::PoluIvica():_origin(nullptr), _tvin(nullptr), _sledeca(nullptr), _prethodna(nullptr){}

PoluIvica::PoluIvica(Cvor* origin, PoluIvica* tvin, PoluIvica* sledeca, PoluIvica* prethodna):_origin(origin), _tvin(tvin), _sledeca(sledeca), _prethodna(prethodna){}

Cvor* PoluIvica::origin() const {return _origin;}

void PoluIvica::postaviOrigin(Cvor* origin) {
    _origin = origin;
}

PoluIvica* PoluIvica::tvin() const {
    return _tvin;
}

void PoluIvica::postaviTvin(PoluIvica* tvin) {
    _tvin = tvin;

}
PoluIvica* PoluIvica::sledeca() const {
    return _sledeca;
}

void PoluIvica::postaviSledecu(PoluIvica* sledeca) {
    _sledeca = sledeca;
}

PoluIvica* PoluIvica::prethodna() const {
    return _prethodna;
}

void PoluIvica::postaviPrethodnu(PoluIvica* prethodna) {
    _prethodna = prethodna;
}

double PoluIvica::duzinaPoluIvice(){
    if(_tvin==nullptr)
       return 0;

    QLineF ivica(_origin->koordinate(), _tvin->origin()->koordinate());
    return ivica.length();

}
void PoluIvica::izracunajDuzinu(){

    QLineF l(_origin->koordinate(), _tvin->origin()->koordinate());
    _duzina = l.length();

}
////////////////////////////////////
/// \brief Poligon
///

Poligon::~Poligon() {

    for(auto c: _cvorovi)
        delete c;

    for(auto i: _ivice)
        delete i;

}
Cvor* Poligon::cvor(int i) const {
    return _cvorovi[i];
}

const std::vector<Cvor*> &Poligon::cvorovi() const {
    return _cvorovi;
}

void Poligon::dodajCvor(Cvor* c) {
    _cvorovi.push_back(c);
}

PoluIvica* Poligon::poluIvica(int i) const {
    return _ivice[i];
}

const std::vector<PoluIvica*> &Poligon::ivice() const {
    return _ivice;
}

void Poligon::dodajPoluIvicu(PoluIvica* c) {
    _ivice.push_back(c);
}

PoluIvica* Poligon::pronadjiIvicu(Cvor* pocetak, Cvor* kraj) {

    for(auto& i: _ivice)
        if(i->origin() == pocetak && i->tvin()->origin() == kraj)
            return i;
    return nullptr;
}

PoluIvica* Poligon::pronadjiIvicuPoCvoru(Cvor* cvor) {

    for(auto& i: _ivice)
        if(i->origin() == cvor)
            return i;
    return nullptr;
}

bool Poligon::pronadjiPresekSaLinijom(PoluIvica *ivica){
    bool ima_presek_sa_poligonom=false;

    for(auto& i: _ivice)
        if(i->origin() == ivica->origin() && i->tvin()->origin() == ivica->tvin()->origin()){
            ima_presek_sa_poligonom =true;
            break;
    }
    if(ima_presek_sa_poligonom)
        return false;
    ima_presek_sa_poligonom = false;
    QLineF linija_ivice(ivica->origin()->koordinate(), ivica->tvin()->origin()->koordinate());

    for(unsigned i=0; i<_ivice.size(); i++){
        QLineF linija_ps(_ivice[i]->prethodna()->origin()->koordinate(), _ivice[i]->sledeca()->origin()->koordinate());
        QLineF linija_ivica(_ivice[i]->origin()->koordinate(), _ivice[i]->tvin()->origin()->koordinate());
        QPointF tacka, tacka1;
        ima_presek_sa_poligonom = ima_presek_sa_poligonom || (pomocneFunkcije::presekDuzi(linija_ps, linija_ivice, tacka) ||  pomocneFunkcije::presekDuzi(linija_ivica, linija_ivice, tacka1));
        ima_presek_sa_poligonom = ima_presek_sa_poligonom && ((tacka!=ivica->origin()->koordinate() && tacka!=ivica->tvin()->origin()->koordinate()) && (tacka1!=ivica->origin()->koordinate() && tacka1!=ivica->tvin()->origin()->koordinate()));

    }

    return ima_presek_sa_poligonom;

}

bool PoredjenjeZaStabloPretrage::operator()(PoluIvica *prva, PoluIvica *druga)
{

        QPointF tacka1;
        QLineF _linija_za_poredjenje((*_prava_za_poredjenje)->origin()->koordinate(), (*_prava_za_poredjenje)->tvin()->origin()->koordinate());
        QLineF linija_prva(prva->origin()->koordinate(), prva->tvin()->origin()->koordinate());
        pomocneFunkcije::presekDuzi(_linija_za_poredjenje, linija_prva, tacka1);
        QPointF tacka2;
        QLineF linija_druga(druga->origin()->koordinate(), druga->tvin()->origin()->koordinate());

        pomocneFunkcije::presekDuzi(_linija_za_poredjenje, linija_druga, tacka2);

        return tacka1.y()>tacka2.y() || (tacka1.y()==tacka2.y() && tacka1.x()<tacka2.y());
 }

PoredjenjeZaCvorove::PoredjenjeZaCvorove(Cvor** cvor){
    _cvor_za_poredjenje = cvor;
}

bool PoredjenjeZaCvorove::operator()(Cvor* c1, Cvor* c2){

    QLineF l1((*_cvor_za_poredjenje)->koordinate(), c1->koordinate());
    QLineF l2((*_cvor_za_poredjenje)->koordinate(), c2->koordinate());

    return l1.angle()<l2.angle() || (l1.angle()==l2.angle() && l1.length()>l2.length());
}
////////////////////////////////////////////
/// \brief Graf::Graf
///
Graf::Graf(std::vector<Cvor*> cvorovi, std::vector<PoluIvica*> ivice){

    std::sort(cvorovi.begin(), cvorovi.end(), [](Cvor *c1, Cvor *c2){
        return c1->x() <c2->x() || (c1->x() == c2->x() && c1->y() > c2->y());
    });

    for(auto& c: cvorovi){
        _svi_cvorovi_grafa.push_back(c);
        _duzine_najkracih_puteva_do_cvora[c]=__FLT_MAX__;
        for(auto& i: ivice){
            if(i->origin()==c)
             {
                double duzina = sqrt(pomocneFunkcije::distanceKvadratF(i->origin()->koordinate(), i->tvin()->origin()->koordinate()));
                std::pair<Cvor*, double> par = std::make_pair<>(i->tvin()->origin(), duzina);
                _susedi[c].push_back(par);
            }
        }
    }



}

std::vector<Cvor*> Graf::najkraciPut(Cvor* pocetak, Cvor* kraj){

    _roditelji[pocetak]= (nullptr);
    _duzine_najkracih_puteva_do_cvora[pocetak] = 0;
    Cvor*  cvor;
    while(_svi_cvorovi_grafa.size()>0){

        cvor = nullptr;

        for(auto it=_svi_cvorovi_grafa.begin(); it!=_svi_cvorovi_grafa.end(); it++){
            if(cvor ==nullptr || ( _duzine_najkracih_puteva_do_cvora[*it]!=__FLT_MAX__ && _duzine_najkracih_puteva_do_cvora[cvor] >_duzine_najkracih_puteva_do_cvora[(*it)])){
                cvor = *it;
            }
        }

        if(cvor == kraj){
            std::vector<Cvor*> putanja;
            while(true){
                if(cvor == nullptr)
                    break;

                putanja.push_back(cvor);
                cvor = _roditelji[cvor];
            }

            return putanja;
        }


        for(unsigned i=0; i<_susedi[cvor].size(); i++){
            auto c = _susedi[cvor][i];
            if(_duzine_najkracih_puteva_do_cvora[c.first] == __FLT_MAX__ ||
                    _duzine_najkracih_puteva_do_cvora[cvor]+c.second < _duzine_najkracih_puteva_do_cvora[c.first]){
                _duzine_najkracih_puteva_do_cvora[c.first] = _duzine_najkracih_puteva_do_cvora[cvor]+c.second;
                _roditelji[c.first] = cvor;
            }
        }

         auto it = std::find_if(_svi_cvorovi_grafa.begin(), _svi_cvorovi_grafa.end(), [&](Cvor* c){ return c->koordinate()==cvor->koordinate();});
         _svi_cvorovi_grafa.erase(it);
    }

    return std::vector<Cvor*>();
}
