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
    /* Manji je levlji pravougaonik */
    if (l->xLevo != d->xLevo) {
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
    : pravougaonik(p), tipDogadjaja(t), y(t == GORNJA ? p->yGore : p->yDole)
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
    /* Popunjavanje niza dogadjaja */
    for (auto i = 0ul; i < _n; i++) {
        _dogadjaji.emplace(_pravougaonici[i], GORNJA);
        _dogadjaji.emplace(_pravougaonici[i], DONJA);
    }

    /* Prolazak redom kroz dogadjaje */
    for (const auto &dogadjaj : _dogadjaji) {
        auto pravougaonik = const_cast<Pravougaonik *>(dogadjaj.pravougaonik);

        /* Obrada nailaska na gornju stranicu */
        if (dogadjaj.tipDogadjaja == GORNJA) {
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
}

/* Pravljenje slucajnih pravougaonika */
void PresekPravougaonika::generisiNasumicnePravougaonike(int brojPravougaonika)
{
    /* Alokacija potrebnog prostora */
    _n = brojPravougaonika;
    _pravougaonici = new Pravougaonik *[_n];

    /* Generisanje duplo veceg broja tacaka */
    const auto tacke = generisiNasumicneTacke(2*brojPravougaonika);

    /* Za svaki par pokusaj pravljenja pravougaonika */
    for (auto i = 0ul; i < tacke.size(); i+=2) {
        auto tacka1 = tacke[i];
        auto tacka2 = tacke[i+1];

        /* Preskakanje losih parova */
        if (tacka1.x() == tacka2.x() &&
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
        _pravougaonici[i/2] = new Pravougaonik(tacka1, tacka2);
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

/* Odredjivanje preseka izmedju najuzih kandidata */
void PresekPravougaonika::stab(unsigned int, unsigned int, unsigned int, unsigned int)
{

}

/* Pronalazenje preseka u potprostoru [l, d) */
void PresekPravougaonika::detect(unsigned int l, unsigned int d)
{
    /* Nema preseka ako je samo jedan pravougaonik */
    if (d-l < 2) return;
}

/* Prijavljivanje svih preseka u paru */
void PresekPravougaonika::report()
{
    /* Pravljenje i sortiranje niza vertikalnih ivica */
    VertIvica V[2*_n];
    for (auto i = 0ul; i < _n; i++) {
        V[2*i] = std::make_pair(_pravougaonici[i]->xLevo, _pravougaonici[i]);
        V[2*i+1] = std::make_pair(_pravougaonici[i]->xDesno, _pravougaonici[i]);
    }
    std::sort(V, V+2*_n);

    /* Uproscavanje prethodnog rezultata na pravougaonike */
    _V = new Pravougaonik *[2*_n];
    for (auto i = 0ul; i < 2*_n; i++) {
        _V[i] = V[i].second;
    }

    /* Pravljenje i sortiranje niza pravougaonika
     * posmatranih kao vertikalnih intervala */
    _H = new Pravougaonik *[2*_n];
    for (auto i = 0ul; i < _n; i++) {
        _H[i] = _pravougaonici[i];
    }
    std::sort(_H, _H+_n, [](const auto a, const auto b)
                           { return a->yDole < b->yDole; });

    /* Pravljenje pomocnog sortiranog niza */
    _Hh = new Pravougaonik *[2*_n];

    /* Pronalazenje preseka u potprostoru */
    detect(0, 2*_n);
}
