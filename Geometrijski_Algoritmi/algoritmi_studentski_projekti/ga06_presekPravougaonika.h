#ifndef GA06_PRESEKPRAVOUGAONIKA_H
#define GA06_PRESEKPRAVOUGAONIKA_H

#include "algoritambaza.h"

#include <ext/pb_ds/assoc_container.hpp>

/* Struktura koja predstavlja pravougaonik */
struct Pravougaonik : QRect {
    /* Konstruktor i destruktor klase */
    Pravougaonik(QPoint &, QPoint &);
    Pravougaonik(int, int, int, int);
    virtual ~Pravougaonik() = default;

    /* Cuvanje intervala za brisucu pravu */
    const int xLevo, xDesno,
              yGore, yDole;
};

/* Struktura za poredjenje pravougaonika */
struct PravougaonikComp {
    bool operator()(const Pravougaonik *, const Pravougaonik *) const;
};

/* Definisanje preseka */
using Presek = std::pair<Pravougaonik *, Pravougaonik *>;

/* Struktura za poredjenje preseka */
struct PresekComp {
    bool operator()(const Presek &, const Presek &) const;
};

/* Definisanje skupa preseka */
using IntersecSet = std::set<Presek, PresekComp>;

/* Politika azuriranja cvorova stabla; sabloni
 * su iteratori, kao i dva koja zanemarujemo:
 * funkcija za poredjenje i politika alokacije */
template <class CIterator,
          class Iterator,
          class, class>
struct IntervalUpdatePolicy {

    /* Definisanje metapodatka kao celobrojne
     * vrednosti, posto intervalno drvo cuva
     * maksimalnu desnu tacku poddrveta */
    using metadata_type = int;

    /* Neophodan virtuelni destruktor */
    virtual ~IntervalUpdatePolicy() = default;

    /* Virtuelne metode iteracije */
    virtual CIterator node_begin() const = 0;
    virtual CIterator node_end() const = 0;


    /* Provera presecanja dva intervala */
    bool imaPreseka(const Pravougaonik *i1,
                    CIterator i2)
    {
        return i1->xLevo <= (**i2)->xDesno
               && (**i2)->xLevo <= i1->xDesno;
    }

    /* Trazenje intervala u podstablu */
    void pretrazi(CIterator it,
                  Pravougaonik *i,
                  IntersecSet &preseci)
    {
        /* Baza indukcije je prazno podstablo */
        if (it == node_end())
            return;

        /* Hvatanje preseka sa korenom */
        if (imaPreseka(i, it)) {
            /* Izdvajanje pravougaonika */
            auto* p = **it;

            /* Komparator pravougaonika */
            PravougaonikComp pc;

            /* Dodavanje u dobrom poretku */
            if (pc(i, p)) {
                preseci.emplace(i, p);
            } else {
                preseci.emplace(p, i);
            }
        }

        /* Uzimanje dostupne dece */
        const auto levo = it.get_l_child();
        const auto desno = it.get_r_child();

        /* Indikator obostranog obilaska */
        auto idiDesno = true;

        /* Provera levog sina ako ima nade */
        if (levo != node_end() &&
            levo.get_metadata() >= i->xLevo){
            const auto velicina = preseci.size();
            pretrazi(levo, i, preseci);

            /* Odluka za obostrani obilazak */
            idiDesno = velicina < preseci.size();
        }

        /* Provera desnog sina ako ima nade */
        if (idiDesno) {
            pretrazi(desno, i, preseci);
        }
    }

    /* Pronalazak svih preklapajucih intervala,
     * pocevsi od korena intervalnog stabla */
    IntersecSet nadjiPreseke(Pravougaonik *i)
    {
        IntersecSet preseci;
        pretrazi(node_begin(), i, preseci);
        return preseci;
    }

    /* Popravka stabla nakon izmene, kako
     * bi se odrzala invarijanta drveta */
    void operator()(Iterator it,
                    CIterator end_it)
    {
        /* Podrazumevano je to desna
         * granica tekuceg intervala */
        auto metadata = (**it)->xDesno;

        /* Azuriranje nalevo */
        const auto levo = it.get_l_child();
        if (levo != end_it) {
            metadata = std::max(
                metadata,
                levo.get_metadata());
        }

        /* Azuriranje nadesno */
        const auto desno = it.get_r_child();
        if (desno != end_it) {
            metadata = std::max(
                metadata,
                desno.get_metadata());
        }

        /* Azuriranje metapodatka, uz neophodno
         * uveravanje kompilatora const castom */
        const_cast<int&>(
            it.get_metadata())
            = metadata;
    }
};

/* Definisanje intervalnog drveta */
using IntervalTree = __gnu_pbds::tree<Pravougaonik *, /* nas interval */
                     __gnu_pbds::null_type, /* pomocni tip nas ne zanima */
                     PravougaonikComp, /* podrazumevano uredjenje */
                     __gnu_pbds::rb_tree_tag, /* crveno-crno stablo */
                     IntervalUpdatePolicy>; /* nasa politika */

/* Enumeracija tipa dogadjaja */
enum TipDogadjaja {GORNJA, DONJA};

/* Struktura koja predstavlja dogadjaj */
struct Dogadjaj {
    /* Konstruktor i destruktor klase */
    Dogadjaj(Pravougaonik *, TipDogadjaja);

    /* Cuvanje pravougaonika i tipa */
    const Pravougaonik *pravougaonik;
    const TipDogadjaja tipDogadjaja;

    /* Cuvanje visine dogadjaja */
    const int y;

    /* Dohvataci koordinata */
    int getXLevo() const;
    int getXDesno() const;
};

/* Struktura za poredjenje dogadjaja */
struct DogadjajComp {
    bool operator()(const Dogadjaj &, const Dogadjaj &) const;
};

/* Definisanje reda dogadjaja */
using EventQueue = std::set<Dogadjaj, DogadjajComp>;

/* Definisanje vertikalne stranice */
using VertIvica = std::pair<int, Pravougaonik *>;

/* Klasa koja predstavlja algoritam */
class PresekPravougaonika : public AlgoritamBaza {
public:
    /* Konstruktor i destruktor klase */
    PresekPravougaonika(QWidget *, int, std::string = "",
                        int = BROJ_NASUMICNIH_TACAKA);
    virtual ~PresekPravougaonika();

    /* Virtuelni metodi iz natklase */
    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *) const final;
    void pokreniNaivniAlgoritam() final;

private:
    /* Rad sa podacima, inicijalizacija */
    void generisiNasumicnePravougaonike(int);
    void ucitajPodatkeIzDatoteke(std::string);

    /* Niz pravougaonika, pokazivaca */
    Pravougaonik **_pravougaonici;
    unsigned int _n;

    /* Funkcije za strategiju podeli pa vladaj */
    void stab(unsigned int, unsigned int,
              unsigned int, unsigned int);
    void detect(unsigned int, unsigned int);
    void report();

    /* Strukture za metod brisuce prave */
    EventQueue _dogadjaji;
    IntervalTree _status;
    IntersecSet _preseci;

    /* Nizovi za strategiju podeli pa vladaj */
    Pravougaonik **_V;
    Pravougaonik **_H;
    Pravougaonik **_Hh;
};

#endif // GA06_PRESEKPRAVOUGAONIKA_H
