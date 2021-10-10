#ifndef GA03_STRUKTURE_H
#define GA03_STRUKTURE_H

#include <bits/stdc++.h>
#include "algoritambaza.h"

class Duz {
public:
    QPointF leviKraj;
    QPointF desniKraj;

    Duz(QPointF p, QPointF q): leviKraj(p), desniKraj(q) {
        //postavljamo krajeve duzi u pravi redosled
        //ukoliko se levi kraj nalazi desno od desnog vrsimo zamenu
        //smatramo da nema vertikalnih duzi
        if (leviKraj.x() > desniKraj.x())
            std::swap(leviKraj, desniKraj);
    }

    //pomocna funkcija za odredjivanje orijentacije trougla
    double pozicija(QPointF T) {
        return (desniKraj.x() - leviKraj.x()) * (T.y() - leviKraj.y()) - (T.x() - leviKraj.x()) * (desniKraj.y() - leviKraj.y());
    }

    //funkcija koja odredjuje poziciju tacke u odnosu na duz
    //ukoliko smo iznad duzi vratice 1, ispod -1, a inace vraca 0
    int iznadDuzi(QPointF T) {
        double ori = pozicija(T);
        if (ori > 0)
            return 1;
        else if (ori < 0)
            return -1;
        else
            return 0;
    }

    bool sekuSe(Duz *duz) {
        QPointF A = leviKraj;
        QPointF B = desniKraj;
        QPointF C = duz->leviKraj;
        QPointF D = duz->desniKraj;

        int a = pomocneFunkcije::orijentacija(A, B, C);
        int b = pomocneFunkcije::orijentacija(A, B, D);
        int c = pomocneFunkcije::orijentacija(C, D, A);
        int d = pomocneFunkcije::orijentacija(C, D, B);

        // ako su orijentacije razlicite po parovima, onda se sigurno seku
        if (a != b && c != d)
            return true;
        // ako su A,B i C kolinearne i C pripada duzi AB imamo presek
        if (a == 0 && pomocneFunkcije::pripada(A, C, B)) return true;

        // ako su A,B i D kolinearne i D pripada duzi AB imamo presek
        if (b == 0 && pomocneFunkcije::pripada(A, D, B)) return true;

        // ako su C,D i A kolinearne i A pripada duzi CD imamo presek
        if (c == 0 && pomocneFunkcije::pripada(C, A, D)) return true;

         // ako su C,D i B kolinearne i B pripada duzi CD imamo presek
        if (d == 0 && pomocneFunkcije::pripada(C, B, D)) return true;

        return false;
    }
    std::string toString(){
        return "(("+std::to_string(leviKraj.x())+","+std::to_string(leviKraj.y())+")"+","+"("+std::to_string(desniKraj.x())+","+std::to_string(desniKraj.y())+"))";
    }
};

class Cvor;

class Trapez {
public:
    std::string label;
    //susedi trapezoidi
    Trapez *gLevi;
    Trapez *gDesni;
    Trapez *dLevi;
    Trapez *dDesni;

    Duz *gDuz;
    Duz *dDuz;

    Cvor *cvorT;

    QPointF tleviKraj;
    QPointF tdesniKraj;

    Trapez(): gLevi(nullptr), gDesni(nullptr), dLevi(nullptr), dDesni(nullptr), cvorT(nullptr) {}
    std::string toString(){
        std::string rez="";
        rez+="gd "+gDuz->toString()+"\n";
        rez+="dd "+dDuz->toString()+"\n";
        rez+="lt "+std::to_string(tleviKraj.x())+","+std::to_string(tleviKraj.y())+"\n";
        rez+="dt "+std::to_string(tdesniKraj.x())+","+std::to_string(tdesniKraj.y())+"\n";
        /*ISPIS POMOCU TACAKA
         * std::vector<QPointF> v = niztacaka();
        for(QPointF q:v)
            rez+="("+ std::to_string(q.x())+","+std::to_string(q.y())+")\n";*/
        return rez;
    }
    std::vector<QPointF> niztacaka(){
        std::vector<QPointF> rez;
        if(gDuz->iznadDuzi(tleviKraj)==0&&dDuz->iznadDuzi(tleviKraj)==0){
            //imamo trougao sa temenom u tleviKraj
            rez.push_back(tleviKraj);
            rez.push_back(gDuz->desniKraj);
            rez.push_back(dDuz->desniKraj);
        } else if(gDuz->iznadDuzi(tdesniKraj)==0&&dDuz->iznadDuzi(tdesniKraj)==0){
            //imamo trougao sa temenom u tdesniKraj
            rez.push_back(tdesniKraj);
            rez.push_back(gDuz->leviKraj);
            rez.push_back(dDuz->leviKraj);
        } else{
            //imamo trapez odredimo jednacinu prave koja sadrzi gornju duz

            double k=(gDuz->leviKraj.y()-gDuz->desniKraj.y())/(gDuz->leviKraj.x()-gDuz->desniKraj.x());
            double n= gDuz->leviKraj.y()-k*gDuz->leviKraj.x();
            QPointF a(tleviKraj.x(), k*(tleviKraj.x())+n);
            rez.push_back(a);
            QPointF b(tdesniKraj.x(), k*(tdesniKraj.x())+n);
            rez.push_back(b);

            k=(dDuz->leviKraj.y()-dDuz->desniKraj.y())/(dDuz->leviKraj.x()-dDuz->desniKraj.x());
            n= dDuz->leviKraj.y()-k*dDuz->leviKraj.x();
            QPointF c(tleviKraj.x(), k*(tleviKraj.x())+n);
            rez.push_back(c);
            QPointF d(tdesniKraj.x(), k*(tdesniKraj.x())+n);
            rez.push_back(d);


        }
        return rez;
    }
};
//klasa koja opisuje cvor pretrazivacke strukture
class Cvor {
public:
    virtual ~Cvor() {}
    //funkcija koja cita ime cvora
    virtual std::string procitajImeCvora() { return ""; }
    //funkcija koja vraca pokazivac na trapez koji odgovara cvoru, ako je cvor list, a inace vraca NULL
    virtual Trapez* trapezCvora() { return nullptr; }
    //funkcija koja prolazi kroz pretrazivacku strukturu
    virtual Cvor* sledeciCvor(QPointF) { return nullptr; }

    Cvor *levi;
    Cvor *desni;
    std::vector <Cvor*> roditelj;
};
//u pretrazivackoj strukturi koristicemo tri vrste cvorova
class TCvor: public Cvor {
public:
    QPointF A;

    TCvor(QPointF Ac): A(Ac) {}

    Cvor* sledeciCvor(QPointF T) override {
        if (T.x() < A.x())
            return levi;
        else
            return desni;
    }

    std::string procitajImeCvora() override { return "Tacka"; }
};

class DCvor: public Cvor {
public:
    Duz *d;

    DCvor(Duz *dc): d(dc) {}
    std::string procitajImeCvora() override { return "Duz"; }
    Cvor* sledeciCvor(QPointF T) override {
        int br = d->iznadDuzi(T);
        if (br > 0)
            return levi;
        else if (br < 0)
            return desni;
        else
            // tacka pripada bas duzi
            return nullptr;
    }
};

class List: public Cvor {
public:
    Trapez *t;

    List(Trapez *tc): t(tc) {}
    std::string procitajImeCvora() override { return "List"; }
    Trapez* trapezCvora() override { return t; }
};

class TrapeznaMapa {
public:
    Cvor *koren;

    void novaMapa(std::vector <Duz*> duzi) {
        double minX = 5, maxX = 670;
        double minY = 0, maxY = 580;
        // Ogranicavajuci trapez bice trapez koji se sastoji od spoljasnjih ivica
        Trapez *t = new Trapez();
        Duz *gd = new Duz(QPointF(minX, maxY), QPointF(maxX, maxY));
        Duz *dd = new Duz(QPointF(minX, minY), QPointF(maxX, minY));
        QPointF levaT = QPointF(minX, maxY);
        QPointF desnaT = QPointF(maxX, maxY);
        t->label = "granicni trapez";

        t->gDuz = gd;
        t->dDuz = dd;
        t->tleviKraj = levaT;
        t->tdesniKraj = desnaT;

        Cvor *pocetak = new List(t);
        koren = pocetak;
        t->cvorT = koren;
        //unosimo jednu po jednu duz u trapeznu mapu i gradimo pretrazivacku strukturu
        for (unsigned long i = 0; i < duzi.size(); ++i){
            dodavanjeduzi(duzi[i]);
            //std::cout<<"Dodata je duz"<< i+1<<std::endl;
        }
    }

    Trapez* pronadjiTrapez(QPointF t) {
        Cvor* k = koren;
        while (k->trapezCvora()==nullptr) {
            k = k->sledeciCvor(t);
            if(k==nullptr) return nullptr;
        }
        return k->trapezCvora();
    }

    // funkcija koja unosi novu duz u trapeznu mapu
    void dodavanjeduzi(Duz *novaduz) {
        Trapez* pocetniTrapez = pronadjiTrapez(novaduz->leviKraj);
        Trapez* poslednjiTrapez = pronadjiTrapez(novaduz->desniKraj);
        std::vector <Trapez*> preseceni;

        //ukoliko je i levi i desni kraj duzi unutar jednog trapeza
        if (pocetniTrapez == poslednjiTrapez) {
            // pravimo cetiri nova trapeza koja se dobijaju unosom ove duzi
            Trapez* gornjit = new Trapez();
            Trapez* donjit = new Trapez();
            Trapez* levit = new Trapez();
            Trapez* desnit = new Trapez();

            // podesavamo novi levi trapez
            //gornja duz ostaje duz polaznog trapeza isto i donja duz
            levit->gDuz = pocetniTrapez->gDuz;
            levit->dDuz = pocetniTrapez->dDuz;
            //podesavamo susede levog trapeza
            levit->gLevi = pocetniTrapez->gLevi;
            levit->dLevi = pocetniTrapez->dLevi;
            levit->gDesni = gornjit;
            levit->dDesni = donjit;
            //podesavamo krajnje tacke levog trapeza
            levit->tleviKraj = pocetniTrapez->tleviKraj;
            levit->tdesniKraj = novaduz->leviKraj;


            // podesavamo novi gornji trapez
            //gornja duz ostaje duz polaznog trapeza, a donja je ubacena duz
            gornjit->gDuz = pocetniTrapez->gDuz;
            gornjit->dDuz = novaduz;
            //novi gornji trapez ima samo dva suseda koja cemo oznaciti kao gornja
            gornjit->gLevi = levit;
            gornjit->gDesni = desnit;
            //krajnje tacke gornjeg trapeza su krajnje tacke novaduza
            gornjit->tleviKraj = novaduz->leviKraj;
            gornjit->tdesniKraj = novaduz->desniKraj;

            // podesavamo novi gornji trapez
            //gornja duz ostaje duz polaznog trapeza, a donja je ubacena duz
            donjit->gDuz = novaduz;
            donjit->dDuz = pocetniTrapez->dDuz;
            //levi sused donjeg trapeza je levi trapez koji smo napravili, analogno i desni
            //imamo u ovom slucaju ponovo dva suseda
            donjit->gLevi = levit;
            donjit->gDesni = desnit;
            //krajnje tacke donjeg trapeza su krajnje tacke novaduza
            donjit->tleviKraj = novaduz->leviKraj;
            donjit->tdesniKraj = novaduz->desniKraj;

            // podesavamo novi desni trapez
            //gornja duz, a i donja preuzece se od polaznog trapeza
            desnit->gDuz = pocetniTrapez->gDuz;
            desnit->dDuz = pocetniTrapez->dDuz;
            //podesavamo susede novog desnog trapeza
            desnit->gLevi = gornjit;
            desnit->dLevi = donjit;
            desnit->gDesni = pocetniTrapez->gDesni;
            desnit->dDesni = pocetniTrapez->dDesni;
            //novi levi kraj bice desni kraj duzi, a desni se nasledjuje od polaznog trapeza
            desnit->tleviKraj = novaduz->desniKraj;
            desnit->tdesniKraj = pocetniTrapez->tdesniKraj;

            //potrebno je azurirati pokazivace susednih trapeza pocetnog trapeza
            if (pocetniTrapez->gLevi != nullptr) {
                if (pocetniTrapez->gLevi->gDesni == pocetniTrapez)
                    pocetniTrapez->gLevi->gDesni = levit;
                else
                    pocetniTrapez->gLevi->dDesni = levit;
            }

            if (pocetniTrapez->dLevi != nullptr) {
                if (pocetniTrapez->dLevi->gDesni == pocetniTrapez)
                    pocetniTrapez->dLevi->gDesni = levit;
                else
                    pocetniTrapez->dLevi->dDesni = levit;
            }

            if (pocetniTrapez->gDesni != nullptr) {
                if (pocetniTrapez->gDesni->gLevi == pocetniTrapez)
                    pocetniTrapez->gDesni->gLevi = desnit;
                else
                    pocetniTrapez->gDesni->dLevi = desnit;
            }

            if (pocetniTrapez->dDesni != nullptr) {
                if (pocetniTrapez->dDesni->gLevi == pocetniTrapez)
                    pocetniTrapez->dDesni->gLevi = desnit;
                else
                    pocetniTrapez->dDesni->dLevi = desnit;
            }

            // Azuriramo i pretrazivacku strukturu
            Cvor* levicvorT = new TCvor(novaduz->leviKraj);
            Cvor* desnicvorT = new TCvor(novaduz->desniKraj);
            Cvor* duzCvor = new DCvor(novaduz);

            Cvor* cvorLista = pocetniTrapez->cvorT;
            //prolazimo kroz sve roditelje tekuceg lista
            for (unsigned long i = 0; i < cvorLista->roditelj.size(); ++i) {
                Cvor* rod = cvorLista->roditelj[i];
                if(rod==nullptr) continue;
                //ako je polazni trapez bio levi cvor svog roditelja onda to postaje levicvorT
                //inace levicvorT postavljamo kao desni cvor roditeljskih cvorova originalnog cvora koji je pamtio polazni trapez
                if (cvorLista == rod->levi) {
                    rod->levi = levicvorT;
                    levicvorT->roditelj.push_back(rod);
                } else {
                    rod->desni = levicvorT;
                    levicvorT->roditelj.push_back(rod);
                }
            }

            //ako je u pitanju prva duz koja se dodaje onda levi cvor postaje pocetak azurirane pretrazivacke strukture
            if (koren == cvorLista) {
                koren = levicvorT;
            }

            delete cvorLista;

            //za svaki od dobijena cetiri trapeza napravicemo novi cvor Lista koji odmah unosimo u napravljene trapeze
            Cvor* levitC = new List(levit);
            levit->cvorT = levitC;
            Cvor* gornjitC = new List(gornjit);
            gornjit->cvorT = gornjitC;
            Cvor* donjitC = new List(donjit);
            donjit->cvorT = donjitC;
            Cvor* desnitC = new List(desnit);
            desnit->cvorT = desnitC;

            //azurirajmo najpre levicvorTacke u pretrazivackoj strukturi
            //sa leve strane leve tacke je levi trapez u cije roditelje dodajemo cvor leve tacke
            levicvorT->levi = levitC;
            levitC->roditelj.push_back(levicvorT);
            //sa desne strane dodajemo vezu ka desnom cvoru tacke i azuriramo roditelje desnog cvora
            levicvorT->desni = desnicvorT;
            desnicvorT->roditelj.push_back(levicvorT);
            //kao desni sused desnog cvora dodajemo desni trapez i azuriramo njegove roditelje
            desnicvorT->desni = desnitC;
            desnitC->roditelj.push_back(desnicvorT);
            //kao levi sused desnog cvora dodajemo cvor duzi i azuriramo njene roditelje
            desnicvorT->levi = duzCvor;
            duzCvor->roditelj.push_back(desnicvorT);
            //levi cvor cvora duzi je gornji trapez
            duzCvor->levi = gornjitC;
            gornjitC->roditelj.push_back(duzCvor);
            //desni cvor cvora duzi je donji trapez
            duzCvor->desni = donjitC;
            donjitC->roditelj.push_back(duzCvor);
        }
        else {
            // U slucaju da duz koju dodajemo ne pripada samo jednom trapezu
            // dobijamo novi trapez kao levi deo pocetnog trapeza
            //pravimo skup presecenih trapeza
            preseceni.push_back(pocetniTrapez);
            //prolazimo dalje kroz trapeze koje sece nova duz i smestamo ih u skup presecenih trapeza
            Trapez* tekuci=pocetniTrapez;
            while(tekuci!=poslednjiTrapez){
                //gradimo listu presecenih trapeza

                if(tekuci->gDesni!=nullptr&& novaduz->iznadDuzi(tekuci->gDesni->dDuz->leviKraj)==-1){
                        preseceni.push_back(tekuci->gDesni);
                        tekuci=tekuci->gDesni;
                }else if(tekuci->dDesni!=nullptr){
                        preseceni.push_back(tekuci->dDesni);
                        tekuci=tekuci->dDesni;
                }
            }

            //ispisujemo sve presecene trapeze
            /*std::cout<<"PRESECENI"<<std::endl;
            int i=1;
            for(Trapez * t:preseceni)
                std::cout<<i++<<t->toString()<<std::endl;*/

            Trapez* leviT = new Trapez(*pocetniTrapez);
            Trapez* desniT = new Trapez(*poslednjiTrapez);
            Cvor* cvorLevogT = new List(leviT);
            Cvor* cvorDesnogT = new List(desniT);
            leviT->cvorT=cvorLevogT;
            desniT->cvorT=cvorDesnogT;
            leviT->tdesniKraj = novaduz->leviKraj;
            desniT->tleviKraj = novaduz->desniKraj;

            // azuriramo leve susede pocetnog levog trapeza da pokazuju na novi levi trapez
            if (pocetniTrapez->gLevi != nullptr) {
                if (pocetniTrapez->gLevi->gDesni == pocetniTrapez)
                    pocetniTrapez->gLevi->gDesni = leviT;
                else
                    pocetniTrapez->gLevi->dDesni = leviT;
            }

            if (pocetniTrapez->dLevi != nullptr) {
                if (pocetniTrapez->dLevi->gDesni == pocetniTrapez)
                    pocetniTrapez->dLevi->gDesni = leviT;
                else
                    pocetniTrapez->dLevi->dDesni = leviT;
            }

            //azurirati pretrazivacku strukturu
            Cvor* levicvorT = new TCvor(novaduz->leviKraj);
            Cvor* duzCvor = new DCvor(novaduz);
            Cvor* desnicvorT = new TCvor(novaduz->desniKraj);

            Cvor* cvorLista = pocetniTrapez->cvorT;
            //prolazimo kroz sve roditelje tekuceg lista
            for (unsigned long i = 0; i < cvorLista->roditelj.size(); ++i) {
                Cvor* rod = cvorLista->roditelj[i];
                if(rod==nullptr) continue;
                //ako je polazni trapez bio levi cvor svog roditelja onda to postaje levicvorT
                //inace levicvorT postavljamo kao desni cvor roditeljskih cvorova originalnog cvora koji je pamtio polazni trapez
                if (cvorLista == rod->levi) {
                    rod->levi = levicvorT;
                    levicvorT->roditelj.push_back(rod);
                } else {
                    rod->desni = levicvorT;
                    levicvorT->roditelj.push_back(rod);
                }
            }
            delete cvorLista;
            cvorLista = poslednjiTrapez->cvorT;
            //prolazimo kroz sve roditelje tekuceg lista
            for (unsigned long i = 0; i < cvorLista->roditelj.size(); ++i) {
                Cvor* rod = cvorLista->roditelj[i];
                if(rod==nullptr) continue;
                if (cvorLista == rod->levi) {
                    rod->levi = desnicvorT;
                    desnicvorT->roditelj.push_back(rod);
                } else {
                    rod->desni = desnicvorT;
                    desnicvorT->roditelj.push_back(rod);
                }
            }
            delete cvorLista;

            //za cvor tacke azuriramo nove leve i desne susede
            levicvorT->levi=cvorLevogT;
            cvorLevogT->roditelj.push_back(levicvorT);
            levicvorT->desni=duzCvor;
            duzCvor->roditelj.push_back(levicvorT);

            //strukture za pamcenje novih trapeza iznad i ispod duzi
            std::vector<Trapez *> novigornjiT;
            std::vector<Trapez *> novidonjiT;

            // pripremamo prvi gornji trapez koji ce biti dobijen
            //potencijalnim spajanjem gornjih trapeza
            //i azuriramo mu poznate atribute
            Trapez* gornjiT = new Trapez(*pocetniTrapez);
            gornjiT->tleviKraj = novaduz->leviKraj;
            gornjiT->gDuz = pocetniTrapez->gDuz;
            gornjiT->dDuz = novaduz;
            Cvor* cvorGornjegT = new List(gornjiT);
            gornjiT->cvorT = cvorGornjegT;
            novigornjiT.push_back(gornjiT);

            //pripremamo prvi donji trapez koji ce biti dobijen
            //potencijalnim spajanjem gornjih trapeza
            //i azuriramo mu poznate atribute
            Trapez* donjiT = new Trapez(*pocetniTrapez);
            donjiT->tleviKraj = novaduz->leviKraj;
            donjiT->gDuz = novaduz;
            donjiT->dDuz = pocetniTrapez->dDuz;
            Cvor* cvorDonjegT = new List(donjiT);
            donjiT->cvorT = cvorDonjegT;
            novidonjiT.push_back(donjiT);

            // novi trapezi postaju desni susedi novog levog trapeza
            leviT->gDesni = gornjiT;
            leviT->dDesni = donjiT;

            duzCvor->levi=cvorGornjegT;
            cvorGornjegT->roditelj.push_back(duzCvor);
            duzCvor->desni = cvorDonjegT;
            cvorDonjegT->roditelj.push_back(duzCvor);

            Trapez* prethodniGornjiT=gornjiT;
            Trapez* prethodniDonjiT=donjiT;
            //prolazimo kroz presecene trapeze i pravimo nove trapeze
            for (unsigned i=1;i<preseceni.size();i++){
                gornjiT = new Trapez(*preseceni[i]);
                donjiT = new Trapez(*preseceni[i]);
                novigornjiT.push_back(gornjiT);
                novidonjiT.push_back(donjiT);
                gornjiT->dDuz=novaduz;
                donjiT->gDuz=novaduz;
                //srediti prethodne trapeze i desne susede novih trapeza
                if(preseceni[i-1]->gDesni!=nullptr&&preseceni[i-1]->gDesni!=preseceni[i]){
                    prethodniGornjiT->dDesni=gornjiT;
                    gornjiT->gLevi=prethodniGornjiT;
                    gornjiT->dLevi=nullptr;
                }else if(preseceni[i-1]->gDesni!=nullptr&&preseceni[i-1]->gDesni==preseceni[i]){
                    prethodniGornjiT->dDesni=nullptr;
                    prethodniGornjiT->gDesni=gornjiT;
                    gornjiT->gLevi=prethodniGornjiT;
                    gornjiT->dLevi=nullptr;
                }
                if(preseceni[i-1]->dDesni!=nullptr&&preseceni[i-1]->dDesni!=preseceni[i]){
                    prethodniDonjiT->gDesni=donjiT;
                    donjiT->gLevi=prethodniDonjiT;
                    donjiT->dLevi=nullptr;
                }else if(preseceni[i-1]->dDesni!=nullptr&&preseceni[i-1]->dDesni==preseceni[i]){
                    prethodniDonjiT->gDesni=donjiT;
                    prethodniDonjiT->dDesni=nullptr;
                    donjiT->gLevi=prethodniDonjiT;
                    donjiT->dLevi=nullptr;
                }

                //azuriranje pretrazivacke strukture
                cvorGornjegT = new List(gornjiT);
                cvorDonjegT = new List(donjiT);
                gornjiT->cvorT=cvorGornjegT;
                donjiT->cvorT=cvorDonjegT;

                duzCvor = new DCvor(novaduz);
                cvorLista = preseceni[i]->cvorT;
                //prolazimo kroz sve roditelje tekuceg lista
                for (unsigned long j = 0; j < cvorLista->roditelj.size(); ++j) {
                    Cvor* rod = cvorLista->roditelj[j];
                    if(rod==nullptr) continue;
                    //ako je list bio levi cvor svog roditelja onda to postaje levicvorT
                    //inace levicvorT postavljamo kao desni cvor roditeljskih cvorova originalnog cvora koji je pamtio polazni trapez
                    duzCvor->roditelj.push_back(rod);
                    if (cvorLista == rod->levi)
                        rod->levi = duzCvor;
                    else
                        rod->desni = duzCvor;
                }

                duzCvor->levi=cvorGornjegT;
                cvorGornjegT->roditelj.push_back(duzCvor);
                duzCvor->desni=cvorDonjegT;
                cvorDonjegT->roditelj.push_back(duzCvor);
                //sacuvati pokazivace za tekuce trapeze u prethodne
                //i u vektore (zbog kasnijeg merge-a)
                prethodniGornjiT=gornjiT;
                prethodniDonjiT=donjiT;

            }
            prethodniGornjiT->tdesniKraj=novaduz->desniKraj;
            prethodniDonjiT->tdesniKraj=novaduz->desniKraj;
            prethodniGornjiT->gDesni=desniT;
            prethodniGornjiT->dDesni=nullptr;
            prethodniDonjiT->gDesni=desniT;
            prethodniDonjiT->dDesni=nullptr;
            //azuriramo desni trapez
            // novi trapezi postaju desni susedi novog desnog trapeza
            desniT->gLevi = prethodniGornjiT;
            desniT->dLevi = prethodniDonjiT;

            if (poslednjiTrapez->gDesni != nullptr) {
                if (poslednjiTrapez->gDesni->gLevi == poslednjiTrapez)
                    poslednjiTrapez->gDesni->gLevi = desniT;
                else
                    poslednjiTrapez->gDesni->dLevi = desniT;
            }

            if (poslednjiTrapez->dDesni != nullptr) {
                if (poslednjiTrapez->dDesni->gLevi == poslednjiTrapez)
                    poslednjiTrapez->dDesni->gLevi = desniT;
                else
                    poslednjiTrapez->dDesni->dLevi = desniT;
            }

            duzCvor = new DCvor(novaduz);
            desnicvorT->desni=cvorDesnogT;
            cvorDesnogT->roditelj.push_back(desnicvorT);

            desnicvorT->levi=duzCvor;
            duzCvor->roditelj.push_back(desnicvorT);

            duzCvor->levi=prethodniGornjiT->cvorT;
            prethodniGornjiT->cvorT->roditelj.push_back(duzCvor);

            duzCvor->desni=prethodniDonjiT->cvorT;
            prethodniDonjiT->cvorT->roditelj.push_back(duzCvor);

            //spajanje
            for(unsigned i=0;i<novidonjiT.size()-1;i++){
                if(novidonjiT[i]->gDuz==novidonjiT[i+1]->gDuz
                   && novidonjiT[i]->dDuz==novidonjiT[i+1]->dDuz){
                    Trapez * noviTrapez = new Trapez(*novidonjiT[i]);
                    noviTrapez->tdesniKraj=novidonjiT[i+1]->tdesniKraj;
                    noviTrapez->gDesni=novidonjiT[i+1]->gDesni;
                    noviTrapez->dDesni=novidonjiT[i+1]->dDesni;
                    Cvor * cvorNovogTrapeza= new List(noviTrapez);
                    noviTrapez->cvorT=cvorNovogTrapeza;
                    //preusmeravamo pokazivace jednog roditelja na novi trapez
                    cvorLista = novidonjiT[i]->cvorT;
                    //prolazimo kroz sve roditelje tekuceg lista
                    for (unsigned long j = 0; j < cvorLista->roditelj.size(); ++j) {
                        Cvor* rod = cvorLista->roditelj[j];
                        if(rod==nullptr) continue;
                        //ako je list bio levi cvor svog roditelja onda to postaje cvorNovogTrapeza
                        //inace cvorNovogTrapeza postavljamo kao desni cvor roditeljskih cvorova
                        //originalnog cvora koji je pamtio polazni trapez
                        cvorNovogTrapeza->roditelj.push_back(rod);
                        if (cvorLista == rod->levi)
                            rod->levi = cvorNovogTrapeza;
                        else
                            rod->desni = cvorNovogTrapeza;
                    }
                    delete cvorLista;
                    //preusmeravamo pokazivace drugog roditelja
                    cvorLista = novidonjiT[i+1]->cvorT;
                    //prolazimo kroz sve roditelje tekuceg lista
                    for (unsigned long j = 0; j < cvorLista->roditelj.size(); ++j) {
                        Cvor* rod = cvorLista->roditelj[j];
                        if(rod==nullptr) continue;
                        //ako je list bio levi cvor svog roditelja onda to postaje cvorNovogTrapeza
                        //inace cvorNovogTrapeza postavljamo kao desni cvor roditeljskih cvorova
                        //originalnog cvora koji je pamtio polazni trapez
                        cvorNovogTrapeza->roditelj.push_back(rod);
                        if (cvorLista == rod->levi)
                            rod->levi = cvorNovogTrapeza;
                        else
                            rod->desni = cvorNovogTrapeza;
                    }
                    delete cvorLista;
                   }
            }

             for(unsigned i=0;i<novigornjiT.size()-1;i++){

                if(novigornjiT[i]->gDuz==novigornjiT[i+1]->gDuz
                   && novigornjiT[i]->dDuz==novigornjiT[i+1]->dDuz){
                    Trapez * noviTrapez = new Trapez(*novigornjiT[i]);
                    noviTrapez->tdesniKraj=novigornjiT[i+1]->tdesniKraj;
                    noviTrapez->gDesni=novigornjiT[i+1]->gDesni;
                    noviTrapez->dDesni=novigornjiT[i+1]->dDesni;

                    //azuriramo pretrazivacku strukturu
                    Cvor * cvorNovogTrapeza= new List(noviTrapez);
                    noviTrapez->cvorT=cvorNovogTrapeza;

                    //preusmeravamo pokazivace jednog roditelja na novi trapez
                    cvorLista = novigornjiT[i]->cvorT;
                    //prolazimo kroz sve roditelje tekuceg lista
                    for (unsigned long j = 0; j < cvorLista->roditelj.size(); ++j) {
                        Cvor* rod = cvorLista->roditelj[j];
                        if(rod==nullptr) continue;
                        //ako je list bio levi cvor svog roditelja onda to postaje cvorNovogTrapeza
                        //inace cvorNovogTrapeza postavljamo kao desni cvor roditeljskih cvorova
                        //originalnog cvora koji je pamtio polazni trapez
                        cvorNovogTrapeza->roditelj.push_back(rod);
                        if (cvorLista == rod->levi)
                            rod->levi = cvorNovogTrapeza;
                        else
                            rod->desni = cvorNovogTrapeza;
                    }
                    delete cvorLista;
                    //preusmeravamo pokazivace drugog roditelja
                    cvorLista = novigornjiT[i+1]->cvorT;
                    novigornjiT[i+1]=noviTrapez;
                    //prolazimo kroz sve roditelje tekuceg lista
                    for (unsigned long j = 0; j < cvorLista->roditelj.size(); ++j) {
                        Cvor* rod = cvorLista->roditelj[j];
                        if(rod==nullptr) continue;
                        //ako je list bio levi cvor svog roditelja onda to postaje cvorNovogTrapeza
                        //inace cvorNovogTrapeza postavljamo kao desni cvor roditeljskih cvorova
                        //originalnog cvora koji je pamtio polazni trapez
                        cvorNovogTrapeza->roditelj.push_back(rod);
                        if (cvorLista == rod->levi)
                            rod->levi = cvorNovogTrapeza;
                        else
                            rod->desni = cvorNovogTrapeza;
                    }
                    delete cvorLista;
                   }
                }
            }
            //brisemo presecene trapeze (od njih su nastali novi trapezi)
            for(Trapez *t:preseceni)
            {
                delete t;
            }
        }
};
#endif // GA03_STRUKTURE_H
