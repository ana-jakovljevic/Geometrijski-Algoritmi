#include "ga07_konturaPravougaonika.h"
#include <iostream>
#include <fstream>
#include <QPainterPath>

KonturaPravougaonika::KonturaPravougaonika(QWidget *pCrtanje,
                         int pauzaKoraka,
                         const bool &naivni,
                         std::string imeDatoteke,
                         int brojPravougaonika)
   : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni),
     _brisucaPravaY(0)
{
    if (imeDatoteke != "")
        _pravougaonici = ucitajPodatkeIzDatoteke(imeDatoteke);
    else
        _pravougaonici = generisiNasumicnePravougaonike(brojPravougaonika);
}

void KonturaPravougaonika::pokreniAlgoritam()
{
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void KonturaPravougaonika::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;

}

void KonturaPravougaonika::dodajVertikalnuIvicu(ivica* iv) {
    if (iv->duz->y1() != iv->duz->y2())
        const auto tmp = ph1_vertikalneIvice.insert(iv);
}
void KonturaPravougaonika::pocetakPravougaonika(ivica* iv) {
    std::cout << "Od " << iv->duz->y1() << " do " << iv->duz->y2() << std::endl;
    if (ph1_tackeUKonturi.size() == 0) {
        dodajVertikalnuIvicu(iv);
        ph1_tackeUKonturi.emplace(new tacka1d(_tip::ULAZ, iv->duz->y1()));
        ph1_tackeUKonturi.emplace(new tacka1d(_tip::IZLAZ, iv->duz->y2()));
        return;
    }
    if (iv->duz->y1() < (*ph1_tackeUKonturi.begin())->vr && iv->duz->y2() < (*ph1_tackeUKonturi.begin())->vr) {
        // ivica je levo od postojećeg J i nema preseka
        dodajVertikalnuIvicu(iv);
    }
    else if (iv->duz->y1() > (*ph1_tackeUKonturi.rbegin())->vr && iv->duz->y2() > (*ph1_tackeUKonturi.rbegin())->vr) {
        // ivica je desno od postojećeg J i nema preseka
        dodajVertikalnuIvicu(iv);
    }
    else {
        auto i = ph1_tackeUKonturi.begin();
        int _y = 0;
        float x = iv->duz->x1();
        int counter = 0;
        float startFrom = iv->duz->y1();
        while (i != ph1_tackeUKonturi.end() && (*i)->vr <= iv->duz->y2()) {
            _y = (*i)->vr;
            if ((*i)->tip == _tip::ULAZ) {
                counter++;
                if (_y > iv->duz->y1() && counter == 1) {
                    // "ulazimo" u pravougaonik
                    const auto novaIvica = new ivica(_tip::ULAZ, new QLineF(x, startFrom, x, _y));
                    dodajVertikalnuIvicu(novaIvica);
                }
            }
            else {
                counter--;
                if (counter == 0 && _y > iv->duz->y1()) {
                    // "izlazimo" iz pravougaonika
                    // ako nova ivica postoji, počeće odavde
                    startFrom = _y;
                }
            }
            i++;
        }
        if (counter == 0 && _y < iv->duz->y2()) {
            // ako nova ivica produzuje J po y osi
            const auto novaIvica = new ivica(_tip::ULAZ, new QLineF(x, _y > iv->duz->y1() ? _y : iv->duz->y1(), x, iv->duz->y2()));
            dodajVertikalnuIvicu(novaIvica);
        }
    }
    ph1_tackeUKonturi.emplace(new tacka1d(_tip::ULAZ, iv->duz->y1()));
    ph1_tackeUKonturi.emplace(new tacka1d(_tip::IZLAZ, iv->duz->y2()));

}

void KonturaPravougaonika::krajPravougaonika(ivica* iv) {
    auto i = ph1_tackeUKonturi.begin();
    float _y = 0;
    float x = iv->duz->x1();
    float counter = 0;
    float startFrom = iv->duz->y1();
    while (i != ph1_tackeUKonturi.end() && (*i)->vr <= iv->duz->y2()) {
        _y = (*i)->vr;
        if ((*i)->tip == _tip::ULAZ) {
            counter++;
            if (_y >= iv->duz->y1() && counter == 2) {
                // vec smo ušli u pravougaonik čiju krajnju ivicu sada brišemo
                // ako je counter == 2, ulazimo u drugi pravougaonik,
                // i ovde će kontura da se završi
                dodajVertikalnuIvicu(new ivica(_tip::IZLAZ, new QLineF(x, _y, x, startFrom)));
            }
        }
        else {
            if (_y >= iv->duz->y1() && counter == 1) {
                // pravougaonik nema preseka sa drugim
                dodajVertikalnuIvicu(new ivica(_tip::IZLAZ, new QLineF(x, _y, x, startFrom)));
            }
            counter--;
            if (_y >= iv->duz->y1() && counter == 1) {
                // izašli smo iz pravougaonika i ostao je samo onaj čiju ivicu brišemo
                startFrom = _y;
            }
        }
        i++;
    }
    auto found = std::find_if(ph1_tackeUKonturi.begin(), ph1_tackeUKonturi.end(), [&](const auto &val) {
        return val->vr == iv->duz->y1();
    });
    if (found != ph1_tackeUKonturi.end()) ph1_tackeUKonturi.erase(found);
    found = std::find_if(ph1_tackeUKonturi.begin(), ph1_tackeUKonturi.end(), [&](const auto &val) {
        return val->vr == iv->duz->y2();
    });
    if (found != ph1_tackeUKonturi.end()) ph1_tackeUKonturi.erase(found);
}

void KonturaPravougaonika::pokreniNaivniAlgoritam()
{
    std::vector<ivica*> ivicePragougaonika = {};
    for (QRectF &pravou : _pravougaonici) {
        QLineF *leva = new QLineF(pravou.left(), pravou.top(), pravou.left(), pravou.bottom());
        QLineF *desna = new QLineF(pravou.right(), pravou.top(), pravou.right(), pravou.bottom());
        ivicePragougaonika.push_back(new ivica(_tip::ULAZ, leva));
        ivicePragougaonika.push_back(new ivica(_tip::IZLAZ, desna));
    }
    std::sort(ivicePragougaonika.begin(), ivicePragougaonika.end(), [](ivica* a, ivica* b) {
        return b->duz->x1() > a->duz->x1();
    });
    for (ivica* iv : ivicePragougaonika) {
        if (iv->tip == _tip::ULAZ) {
            pocetakPravougaonika(iv);
        } else {
            krajPravougaonika(iv);
        }
        AlgoritamBaza_updateCanvasAndBlock();
    }
    /*
    std::cout << "ivice duzine " << _ivice  .size() << std::endl;
    QPointF presek;
    for (ivica ivica : _ivice) {
        auto levaTacka = new QPointF(ivica.duz->x1(), ivica.duz->y1());
//        std::cout << "levaTacka " << levaTacka->x() << " " << levaTacka->y() << std::endl;
        auto desnaTacka = new QPointF(ivica.duz->x2(), ivica.duz->y2());
        if (!_kontura.size()) {
            std::cout << "prva " << std::endl;
            _kontura.push_back(levaTacka);
            _kontura.push_back(desnaTacka);
            _najlevlje.emplace(levaTacka);
            _najdesnije.emplace(desnaTacka);
            AlgoritamBaza_updateCanvasAndBlock();
            continue;
        }
        auto trenutnaNajlevlja = *_najlevlje.begin();
//        std::cout << "trenutnaNajlevlja " << trenutnaNajlevlja->x() << " " << trenutnaNajlevlja->y() << std::endl;
        auto trenutnaNajdesnija = *_najdesnije.rbegin();
//        std::cout << "trenutnaNajdesnija " << trenutnaNajdesnija->x() << " " << trenutnaNajdesnija->y() << std::endl;
        if (ivica.tip == _tip::ULAZ) {
            std::cout << "gornja" << std::endl;
            _najlevlje.emplace(levaTacka);
            _najdesnije.emplace(desnaTacka);
            if (levaTacka == *_najlevlje.begin()) {
                _kontura.emplace(_kontura.begin(), new QPointF(trenutnaNajlevlja->x(), levaTacka->y()));
                _kontura.emplace(_kontura.begin(), new QPointF(levaTacka->x(), levaTacka->y()));
            }
            if (desnaTacka == *_najdesnije.rbegin()) {
                _kontura.push_back(new QPointF(trenutnaNajdesnija->x(), desnaTacka->y()));
                _kontura.push_back(new QPointF(desnaTacka->x(), desnaTacka->y()));
            }
        }
        if (ivica.tip == _tip::IZLAZ) {
            std::cout << "donja " << std::endl;
            auto found = std::find_if(_najlevlje.begin(), _najlevlje.end(), [&](const auto &val) {
                return val->x() == levaTacka->x();
            });
            if (found != _najlevlje.end()) _najlevlje.erase(found);
            found = std::find_if(_najdesnije.begin(), _najdesnije.end(), [&](const auto &val) {
                return val->x() == desnaTacka->x();
            });
            if (found != _najdesnije.end()) _najdesnije.erase(found);
            if (trenutnaNajlevlja->x() == levaTacka->x()) {
                _kontura.emplace(_kontura.begin(), new QPointF(levaTacka->x(), levaTacka->y()));
                if (_najlevlje.size()) _kontura.emplace(_kontura.begin(), new QPointF((*_najlevlje.begin())->x(), levaTacka->y()));
            }
            if (trenutnaNajdesnija->x() == desnaTacka->x()) {
                _kontura.push_back(new QPointF(desnaTacka->x(), desnaTacka->y()));
                if (_najdesnije.size()) _kontura.push_back(new QPointF((*_najdesnije.rbegin())->x(), desnaTacka->y()));
            }
        }
        AlgoritamBaza_updateCanvasAndBlock();
    }
    */
    emit animacijaZavrsila();
}

//void KonturaPravougaonika::naglasiTrenutnu(QPainter *painter, unsigned long i) const
//{
//    /* Ako je trenutna duz u redu */
//    if (i < _kontura.size()) {
//        /* Transformacija cetkice */
//        painter->save();
//        painter->scale(1, -1);
//        painter->translate(0, _kontura[i].y());

//        /* Podesavanje stila olovke */
//        auto olovka = painter->pen();
//        olovka.setColor(Qt::darkGreen);
//        painter->setPen(olovka);

//        /* Ponistavanje transformacija */
//        painter->restore();

//        /* Podesavanje stila olovke */
//        olovka.setColor(Qt::red);
//        painter->setPen(olovka);

//        /* Iscrtavanje duzi */
////        painter->draw(_duzi[i]);
//    }
//}

void KonturaPravougaonika::crtajNaivniAlgoritam(QPainter *painter) const
{
    /* Odustajanje u slucaju greske */
    if (!painter) return;

    /* Iscrtavanje svakog pravougaonika */
    for (auto pr : _pravougaonici) {
        painter->drawRect(pr);
    }

    /* Podesavanje stila fonta */
    auto font = painter->font();
    font.setWeight(font.Bold);
    font.setPointSizeF(1.3*font.pointSizeF());
    painter->setFont(font);

    /* Naglasavanje trenutnih duzi */
//    naglasiTrenutnu(painter, _i);
//    naglasiTrenutnu(painter, _j);

    /* Podesavanje stila olovke */
    auto olovka = painter->pen();
    olovka.setColor(Qt::yellow);
    olovka.setWidth(2*olovka.width());
    painter->setPen(olovka);

    for (const auto iv: ph1_vertikalneIvice) {
        painter->drawLine(*iv->duz);
    }

//    std::cout << "kontura duzine " << _kontura.size() << std::endl;
    for (const auto presek: _kontura) {
//        std::cout << "Tacka " << presek->x() << " " << presek->y() << std::endl;
        painter->drawPoint(*presek);
    }
    if (_kontura.size()) {
//        std::cout << "kontura linija " << std::endl;
        QPainterPath path;
        path.moveTo((*_kontura.begin())->x(), (*_kontura.begin())->y());
        for (const auto presek: _kontura) {
            path.lineTo(presek->x(), presek->y());
        }
        path.closeSubpath();
        QBrush cetka(Qt::GlobalColor::red, Qt::BrushStyle::Dense6Pattern);
        painter->fillPath(path, cetka);
    }
//    std::cout << "kontura nacrtana " << std::endl;
}

std::vector<QRectF> KonturaPravougaonika::generisiNasumicnePravougaonike(int brojPravougaonika) const
{
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<QRectF> pravougaonici;

    const auto tacke = generisiNasumicneTacke(2*brojPravougaonika);

    for(auto i=0ul; i < tacke.size()-1; i+=2){
        auto xA = tacke[i].x();
        auto xB = tacke[i+1].x();
        auto yA = tacke[i].y();
        auto yB = tacke[i+1].y();
        auto x1 = xA < xB ? xA : xB;
        auto x2 = xA < xB ? xB : xA;
        auto y1 = yA < yB ? yA : yB;
        auto y2 = yA < yB ? yB : yA;
        pravougaonici.emplace_back(x1, y1, x2 - x1, y2 - y1);
    }

    return pravougaonici;
}

std::vector<QRectF> KonturaPravougaonika::ucitajPodatkeIzDatoteke(std::string imeDatoteke) const
{
    std::ifstream inputFile(imeDatoteke);
    std::vector<QRectF> pravougaonici;

    double xA, yA, xB, yB;

    while(inputFile >> xA >> yA >> xB >> yB)
    {
        auto x1 = xA < xB ? xA : xB;
        auto x2 = xA < xB ? xB : xA;
        auto y1 = yA < yB ? yA : yB;
        auto y2 = yA < yB ? yB : yA;
        pravougaonici.emplace_back(x1, y1, x2, y2);
    }

    return pravougaonici;
}
