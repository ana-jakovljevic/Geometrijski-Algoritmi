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
    std::cout << "konstruktor" << std::endl;
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

void KonturaPravougaonika::pokreniNaivniAlgoritam()
{
    for (QRectF &pravou : _pravougaonici) {
        QLineF *gornja = new QLineF(pravou.left(), pravou.top(), pravou.right(), pravou.top());
        QLineF *donja = new QLineF(pravou.left(), pravou.bottom(), pravou.right(), pravou.bottom());
        _ivice.emplace(tipIvice::GORNJA_IVICA, gornja);
        _ivice.emplace(tipIvice::DONJA_IVICA, donja);
    }
//    std::cout << "ivice duzine " << _ivice  .size() << std::endl;
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
        if (ivica.tip == tipIvice::GORNJA_IVICA) {
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
        if (ivica.tip == tipIvice::DONJA_IVICA) {
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
    std::cout << "crtalje" << std::endl;
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

    std::cout << "kontura duzine " << _kontura.size() << std::endl;
    for (const auto presek: _kontura) {
        std::cout << "Tacka " << presek->x() << " " << presek->y() << std::endl;
        painter->drawPoint(*presek);
    }
    if (_kontura.size()) {
        std::cout << "kontura linija " << std::endl;
        QPainterPath path;
        path.moveTo((*_kontura.begin())->x(), (*_kontura.begin())->y());
        for (const auto presek: _kontura) {
            path.lineTo(presek->x(), presek->y());
        }
        path.closeSubpath();
        QBrush cetka(Qt::GlobalColor::red, Qt::BrushStyle::Dense6Pattern);
        painter->fillPath(path, cetka);
    }
    std::cout << "kontura nacrtana " << std::endl;
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
