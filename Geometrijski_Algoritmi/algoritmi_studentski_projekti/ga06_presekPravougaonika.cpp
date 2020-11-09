#include "ga06_presekPravougaonika.h"
#include <fstream>

/* Konstrukcija pravougaonika od dve tacke */
Pravougaonik::Pravougaonik(QPoint &topLeft, QPoint &bottomRight)
    : QRect(topLeft, bottomRight), xLevo(topLeft.x()), xDesno(bottomRight.x()),
                                   yGore(topLeft.y()), yDole(bottomRight.y())
{ }


/* Konstrukcija pravougaonika od parametara */
Pravougaonik::Pravougaonik(int x, int y, int w, int h)
    : QRect(x, y, w, h), xLevo(x), xDesno(x+w),
                         yGore(y+h), yDole(y)
{ }

/* Struktura za poredjenje pravougaonika */
bool PravougaonikComp::operator()(const Pravougaonik *l, const Pravougaonik *d) const
{
    /* Odustajanje u slucaju greske */
    if (!l && !d) {
        return false;
    /* Manji je levlji pravougaonik */
    } else if (l->xLevo != d->xLevo) {
        return l->xLevo < d->xLevo;
    /* Manji je levlji pravougaonik */
    } else if (l->xDesno != d->xDesno) {
        return l->xDesno < d->xDesno;
    /* Inace se porede pokazivaci */
    } else {
        return l < d;
    }
}

/* Struktura za poredjenje preseka */
bool PresekComp::operator()(const Presek &l, const Presek &d) const
{
    /* Komparator pravougaonika */
    PravougaonikComp pc;

    /* Jednakost nezavisno od redosleda */
    if ((l.first == d.first && l.second == d.second) ||
        (l.first == d.second && l.second == d.first)) {
        return false;
    /* Inace poredjenje pravougaonika */
    } else if (l.first != d.first) {
        return pc(l.first, d.first);
    } else {
        return pc(l.second, d.second);
    }
}

/* Konstrukcija dogadjaja */
Dogadjaj::Dogadjaj(Pravougaonik *p, TipDogadjaja t)
    : pravougaonik(p), tipDogadjaja(t),
      y(t == TipDogadjaja::GORNJA ? p->yGore : p->yDole)
{ }

/* Dohvatanje levog intervala pravougaonika */
int Dogadjaj::getXLevo() const
{
    return pravougaonik->xLevo;
}

/* Dohvatanje desnog intervala pravougaonika */
int Dogadjaj::getXDesno() const
{
    return pravougaonik->xDesno;
}

/* Struktura za poredjenje dogadjaja */
bool DogadjajComp::operator()(const Dogadjaj &l, const Dogadjaj &d) const
{
    /* Manji je dogadjaj na visoj poziciji */
    if (l.y != d.y) {
        return l.y > d.y;
    /* Manji je dogadjaj na levljoj poziciji */
    } else if (l.getXLevo() != d.getXLevo()) {
        return l.getXLevo() < d.getXLevo();
    /* Manji je dogadjaj na levljoj poziciji */
    } else if (l.getXDesno() != d.getXDesno()) {
        return l.getXDesno() < d.getXDesno();
    /* Manji je dogadjaj dodavanja od izbacivanja */
    } else {
        return l.tipDogadjaja < d.tipDogadjaja;
    }
}

/* Konstrukcija algoritma */
PresekPravougaonika::PresekPravougaonika(QWidget *pCrtanje,
                                         int pauzaKoraka,
                                         std::string imeDatoteke,
                                         int brojPravougaonika)
    :AlgoritamBaza(pCrtanje, pauzaKoraka)
{
    /* Inicijalizacija niza pravougaonika */
    if (imeDatoteke == "")
        generisiNasumicnePravougaonike(brojPravougaonika);
    else
        ucitajPodatkeIzDatoteke(imeDatoteke);
}

/* Deinicijalizacija algoritma */
PresekPravougaonika::~PresekPravougaonika()
{
    for (auto i = 0ul; i < _n; i++) {
        delete _pravougaonici[i];
    }
    delete [] _pravougaonici;
    delete [] _V;
    delete [] _H;
    delete [] _Hh;
}

/* Algoritam zasnovan na strategiji podeli pa vladaj, pogledati
 * https://www.widelands.org/~sirver/wl/141229_devail_rects.pdf */
void PresekPravougaonika::pokreniAlgoritam()
{
    /* Ciscenje skupa preseka */
    _preseci.clear();

    /* Prijavljivanje svih preseka u paru */
    report();

    /* Obavestavanje pozivaoca o finalizovanoj animaciji */
    emit animacijaZavrsila();
}

/* Iscrtavanje tekuceg stanja algoritma */
void PresekPravougaonika::crtajAlgoritam(QPainter *painter) const
{
    /* Odustajanje u slucaju greske */
    if (!painter) return;

    /* Iscrtavanje svakog pravougaonika */
    for (auto i = 0ul; i < _n; i++) {
        painter->drawRect(*_pravougaonici[i]);
    }
}

/* Algoritam zasnovan na brisucoj (pokretnoj) pravoj */
void PresekPravougaonika::pokreniNaivniAlgoritam()
{
    /* Ciscenje skupa preseka */
    _preseci.clear();

    /* Popunjavanje niza dogadjaja */
    for (auto i = 0ul; i < _n; i++) {
        _dogadjaji.emplace(_pravougaonici[i], TipDogadjaja::GORNJA);
        _dogadjaji.emplace(_pravougaonici[i], TipDogadjaja::DONJA);
    }

    /* Prolazak redom kroz dogadjaje */
    for (const auto &dogadjaj : _dogadjaji) {
        auto pravougaonik = const_cast<Pravougaonik *>(dogadjaj.pravougaonik);

        /* Obrada nailaska na gornju stranicu */
        if (dogadjaj.tipDogadjaja == TipDogadjaja::GORNJA) {
            /* Nalazenje svih trenutnih preseka */
            const auto preseci = _status.nadjiPreseke(pravougaonik);
            _preseci.insert(std::cbegin(preseci), std::cend(preseci));

            /* Ubacivanje novog pravougaonika u status */
            _status.insert(pravougaonik);
        /* Obrada nailaska na donju stranicu */
        } else {
            /* Izbacivanje starog pravougaonika iz statusa */
            _status.erase(pravougaonik);
        }
    }

    /* Ciscenje reda dogadjaja */
    _dogadjaji.clear();
}

/* Provera da li postoji presek para pravougaonika */
bool PresekPravougaonika::sekuSe(const Pravougaonik *p1,
                                 const Pravougaonik *p2) const
{
    return p1 && p2 && /* nisu null */
           p1->xLevo < p2->xDesno &&
           p1->xDesno > p2->xLevo &&
           p1->yDole < p2->yGore &&
           p1->yGore > p2->yDole;
}

void PresekPravougaonika::ubaciPresek(Pravougaonik *p1, Pravougaonik *p2)
{
    /* Instanciranje komparatora pravougaonika */
    PravougaonikComp pc;

    /* Dodavanje u dobrom poretku */
    if (pc(p1, p2)) {
        _preseci.emplace(p1, p2);
    } else {
        _preseci.emplace(p2, p1);
    }
}

/* Kvadratni algoritam grube sile */
void PresekPravougaonika::pokreniAlgoritamGrubeSile()
{
    /* Ciscenje skupa preseka */
    _preseci.clear();

    /* Uporedjivanje pravougaonika u parovima */
    for (auto i = 0ul; i < _n; i++) {
        auto *p1 = _pravougaonici[i];
        for (auto j = i+1; j < _n; j++) {
            auto *p2 = _pravougaonici[j];

            /* Odredjivanje preseka i dodavanje
             * u skup ako postoji (nije prazan) */
            if (sekuSe(p1, p2)) {
                ubaciPresek(p1, p2);
            }
        }
    }
}

/* Provera da li postoji dodirna tacka para pravougaonika */
bool PresekPravougaonika::dodirujuSe(const Pravougaonik *p1, const Pravougaonik *p2) const
{
    return p1 && p2 && /* nisu null */
        (((p1->xLevo == p2->xDesno || p1->xDesno == p2->xLevo) &&
           p1->yDole <= p2->yGore && p1->yGore >= p2->yDole) ||
         ((p1->yDole == p2->yGore || p1->yGore == p2->yDole) &&
           p1->xLevo <= p2->xDesno && p1->xDesno >= p2->xLevo));
}

/* Odbacivanje novih pravougaonika koji kvare stanje */
void PresekPravougaonika::popraviNasumicnoGenerisane()
{
    for (auto i = 0ul; i < _n-1; i++) {
        if (dodirujuSe(_pravougaonici[i], _pravougaonici[_n-1])) {
            delete _pravougaonici[-1+_n--]; return;
        }
    }
}

/* Pravljenje slucajnih pravougaonika */
void PresekPravougaonika::generisiNasumicnePravougaonike(int brojPravougaonika)
{
    /* Alokacija potrebnog prostora */
    _n = 0;
    _pravougaonici = new Pravougaonik *[brojPravougaonika];

    /* Generisanje duplo veceg broja tacaka */
    const auto tacke = generisiNasumicneTacke(2*brojPravougaonika);

    /* Za svaki par pokusaj pravljenja pravougaonika */
    for (auto i = 0ul; i < tacke.size(); i+=2) {
        auto tacka1 = tacke[i];
        auto tacka2 = tacke[i+1];

        /* Preskakanje losih parova */
        if (tacka1.x() == tacka2.x() ||
            tacka1.y() == tacka2.y()) {
            continue;
        }

        /* Prva treba da bude levo */
        if (tacka1.x() > tacka2.x()) {
            std::swap(tacka1.rx(), tacka2.rx());
        }

        /* Prva treba da bude gore */
        if (tacka1.y() < tacka2.y()) {
            std::swap(tacka1.ry(), tacka2.ry());
        }

        /* Pravljenje pomocu sredjenih tacaka */
        _pravougaonici[_n++] = new Pravougaonik(tacka1, tacka2);
        popraviNasumicnoGenerisane();
    }
}

/* Citanje pravougaonika iz datoteke */
void PresekPravougaonika::ucitajPodatkeIzDatoteke(std::string imeDatoteke)
{
    /* Otvaranje zadatog fajla */
    std::ifstream datoteka(imeDatoteke);

    /* Alokacija potrebnog prostora */
    datoteka >> _n;
    _pravougaonici = new Pravougaonik *[_n];

    /* Ucitavanje svakog pravougaonika pojedinacno */
    for (auto i = 0ul; i < _n; i++) {
        int x, y, w, h;
        datoteka >> x >> y >> w >> h;
        _pravougaonici[i] = new Pravougaonik(x, y, w, h);
    }
}

/* Pomocna funkcija za proveru indeksa */
bool PresekPravougaonika::proveriIndeks(unsigned int i,
                                        unsigned int d) const
{
    return i < d && _H[i];
}

/* Pomocna funkcija za azuriranje indeksa */
void PresekPravougaonika::azurirajIndeks(unsigned int &i,
                                         unsigned int d,
                                         KandidatS k) const
{
    do i++; while (proveriIndeks(i, d) && _H[i]->kS != k);
}

/* Odredjivanje preseka najuzih kandidata; skupovi
 * A i B iz rada su apstrahovani enumeracijom */
void PresekPravougaonika::stab(unsigned int l, unsigned int d,
                               KandidatS A, KandidatS B)
{
    /* Odredjivanje pocetnih indeksa */
    auto i = l-1; azurirajIndeks(i, d, A);
    auto j = l-1; azurirajIndeks(j, d, B);

    /* Prolazak kroz sve kandidate */
    while (proveriIndeks(i, d) && proveriIndeks(j, d)) {
        /* Ako je prvi skup ispod drugog */
        if (_H[i]->yDole < _H[j]->yDole) {
            auto k = j;
            /* Registrovanje svih susednih */
            while (proveriIndeks(k, d) &&
                   _H[k]->yDole < _H[i]->yGore) {
                ubaciPresek(_H[i], _H[k]);
                azurirajIndeks(k, d, B);
            }
            azurirajIndeks(i, d, A);
        /* Ako je prvi skup iznad drugog */
        } else {
            auto k = i;
            /* Registrovanje svih susednih */
            while (proveriIndeks(k, d) &&
                   _H[k]->yDole < _H[j]->yGore) {
                ubaciPresek(_H[j], _H[k]);
                azurirajIndeks(k, d, A);
            }
            azurirajIndeks(j, d, B);
        }
    }
}

/* Uzimanje odgovarajuce ivice pravougaonika */
int PresekPravougaonika::uzmiIvicu(const VertIvica &ivica) const
{
    return ivica.first == TipIvice::LEVA ?
                ivica.second->xLevo : ivica.second->xDesno;
}

/* Pronalazenje preseka u potprostoru [l, d) */
void PresekPravougaonika::detect(unsigned int l, unsigned int d)
{
    /* Nema preseka ako je samo jedan pravougaonik */
    if (d-l < 2) return;

    /* Podrazumevano nijedan pravougaonik nije zanimljiv */
    for (auto i = l; i < d; i++) {
        _V[i].second->kS = KandidatS::Sxx;
    }

    /* Odredjivanje sredine intervala za podelu; ovo
     * imitira skupove V1 i V2 iz rada bez kopiranja */
    const auto s = l+(d-l)/2;

    /* Odredjivanje skupova S11 i S12 iz rada */
    for (auto i = l; i < s; i++) {
        /* Pravougaonik je skroz levo od podele */
        if (_V[i].first == TipIvice::DESNA) {
            _V[i].second->kS = KandidatS::S11;
        /* Pravougaonik obuhvata desnu polovinu */
        } else if (_V[i].second->xDesno >= uzmiIvicu(_V[d-1])) {
            _V[i].second->kS = KandidatS::S12;
        }
    }

    /* Odredjivanje skupova S22 i S21 iz rada */
    for (auto i = s; i < d; i++) {
        /* Pravougaonik je skroz desno od podele */
        if (_V[i].first == TipIvice::LEVA) {
            _V[i].second->kS = KandidatS::S22;
        /* Pravougaonik obuhvata levu polovinu */
        } else if (_V[i].second->xLevo <= uzmiIvicu(_V[l])) {
            _V[i].second->kS = KandidatS::S21;
        }
    }

    /* Odredjivanje preseka najuzih kandidata */
    stab(l, d, KandidatS::S12, KandidatS::S22);
    stab(l, d, KandidatS::S21, KandidatS::S11);
    stab(l, d, KandidatS::S12, KandidatS::S21);

    /* Particionisanje niza H na H1 i H2 iz rada */
    auto H1 = l, H2 = s;
    for (auto i = l; proveriIndeks(i, d); i++) {
        switch (_H[i]->kS) {
        /* Neodredjeni idu u obe polovine */
        case KandidatS::Sxx:
            _Hh[H1++] = _H[i];
            _Hh[H2++] = _H[i];
            break;
        /* Levo idu oni koju su u toj polovini */
        case KandidatS::S11:
        case KandidatS::S12:
            _Hh[H1++] = _H[i];
            break;
        /* Desno idu oni koji su u toj polovini */
        case KandidatS::S21:
        case KandidatS::S22:
            _Hh[H2++] = _H[i];
            break;
        }
    }

    /* Popunjavanje preostalih rupa */
    for (auto i = H1; i < s; i++) {
        _Hh[i] = nullptr;
    }
    for (auto j = H2; j < d; j++) {
        _Hh[j] = nullptr;
    }

    /* Prepisivanje particionisanog niza nazad */
    for (auto i = l; i < d; i++) {
        _H[i] = _Hh[i];
    }

    /* Pronalazenje preseka u potprostorima */
    detect(l, s); detect(s, d);
}

/* Prijavljivanje svih preseka u paru */
void PresekPravougaonika::report()
{
    /* Pravljenje i sortiranje niza vertikalnih ivica */
    _V = new VertIvica[2*_n];
    for (auto i = 0ul; i < _n; i++) {
        _V[2*i] = std::make_pair(TipIvice::LEVA, _pravougaonici[i]);
        _V[2*i+1] = std::make_pair(TipIvice::DESNA, _pravougaonici[i]);
    }
    std::sort(_V, _V+2*_n, [this](const VertIvica &a, const VertIvica &b)
                                 /* Poredjenje odgovarajucih ivica */
                                 { return uzmiIvicu(a) < uzmiIvicu(b); });

    /* Pravljenje i sortiranje niza pravougaonika
     * posmatranih kao vertikalnih intervala */
    _H = new Pravougaonik *[2*_n];
    for (auto i = 0ul; i < _n; i++) {
        _H[i] = _pravougaonici[i];
        _H[_n+i] = nullptr;
    }
    std::sort(_H, _H+_n, [](const Pravougaonik *a, const Pravougaonik *b)
                           { return a->yDole < b->yDole; });

    /* Pravljenje pomocnog sortiranog niza */
    _Hh = new Pravougaonik *[2*_n];

    /* Pronalazenje preseka u (celom) potprostoru */
    detect(0, 2*_n);
}
