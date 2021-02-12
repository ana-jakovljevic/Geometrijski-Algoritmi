#include "ga17_convexhulllineintersections.h"

#define GREMOV_NAIVNI (1)

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

ConvexHullLineIntersections::ConvexHullLineIntersections(QWidget *pCrtanje,
                                                         int pauzaKoraka,
                                                         const bool &naivni,
                                                         std::string imeDatoteke,
                                                         int brojDuzi)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    if (imeDatoteke != "")
        _duzi = ConvexHullLineIntersections::ucitajPodatkeIzDatoteke(imeDatoteke);
    else {
        if(brojDuzi == 0)
            return;
        _duzi = ConvexHullLineIntersections::generisiNasumicneLinije(brojDuzi);
    }

    /* Stampanje koordinata u fajl za potrebe provere */
//    std::ofstream myfile;
//    myfile.open ("andja.txt");
//    for (auto &d: _duzi) {
//        myfile << d.x1() << " " << d.y1() << " " << d.x2() << " " << d.y2() << std::endl;
//    }
//    myfile.close();

    pretvoriDuziUMapu();
}

void ConvexHullLineIntersections::pokreniAlgoritam()
{
    /* Algoritam (Computing the Convex Hull of Line Intersections):
     *
     * 1. Sortirati sve linije prema nagibu koji zaklapaju sa x-osom (vec sortirano u mapi)
     * 2. Pronaci presek izmedju svake dve susedne (gledano po prvom sledecem uglu koji je razlicit)
     *    i smestiti u preseke
     * 3. Napraviti konveksni omotac od presecnih tacaka
     *    - Gremov algoritam
    */


    /* Ako je datoteka prazna ili ima 0 linija */
    if (_mapaUgaoDuzi.size() == 0) {
        emit animacijaZavrsila();
        return;
    }

    /* 2. korak - Preseci O(nlogn) */
    for (auto it = std::begin(_mapaUgaoDuzi); it != std::end(_mapaUgaoDuzi); ++it){
        if (_mapaUgaoDuzi.size() == 1) {
            emit animacijaZavrsila();
            return;
        }
        QLineF prvaDuz = it->second[0];
        QLineF poslednjaDuz;
        /* Zbog provere da li postoji */
        poslednjaDuz.setLength(0);

        // Poslednja ako ima
        if (it->second.size() > 1)
            poslednjaDuz = it->second[it->second.size()-1];


        QLineF prvaDuzZaPresek;
        QLineF poslednjaDuzZaPresek;
        /* Zbog provere da li postoji */
        poslednjaDuzZaPresek.setLength(0);

        /* Ako smo na kraju uzmemo onda prvu */
        if (std::next(it) == _mapaUgaoDuzi.end()) {
            prvaDuzZaPresek = _mapaUgaoDuzi.begin()->second.at(0);
            // Poslednja ako ima
            if (_mapaUgaoDuzi[0].size() > 1){
                poslednjaDuzZaPresek = _mapaUgaoDuzi.begin()->second.at(_mapaUgaoDuzi.begin()->second.size()-1);
            }
        /* Ako nismo na kraju uzimamo sledecu */
        } else {
            prvaDuzZaPresek = std::next(it)->second.at(0);
            // Poslednja ako ima
            if (std::next(it)->second.size()>1)
                poslednjaDuzZaPresek = std::next(it)->second[std::next(it)->second.size()-1];
        }

        QPointF presek;
        if (ConvexHullLineIntersections::presekLinija(prvaDuz,
                                                      prvaDuzZaPresek,
                                                      presek)) {
            if (_preseciSet.find(presek) == _preseciSet.end()) {
                _preseci.push_back(presek);
                _preseciSet.insert(presek);
            }
            AlgoritamBaza_updateCanvasAndBlock()
        }
        if (poslednjaDuzZaPresek.length() > static_cast<double>(EPSf)) {
            if (ConvexHullLineIntersections::presekLinija(prvaDuz,
                                                          poslednjaDuzZaPresek,
                                                          presek)) {
                if (_preseciSet.find(presek) == _preseciSet.end()) {
                    _preseci.push_back(presek);
                    _preseciSet.insert(presek);
                }
                AlgoritamBaza_updateCanvasAndBlock()
            }
            if (ConvexHullLineIntersections::presekLinija(poslednjaDuz,
                                                          poslednjaDuzZaPresek,
                                                          presek)) {
                if (_preseciSet.find(presek) == _preseciSet.end()) {
                    _preseci.push_back(presek);
                    _preseciSet.insert(presek);
                }
                AlgoritamBaza_updateCanvasAndBlock()
            }

        }

        if (ConvexHullLineIntersections::presekLinija(poslednjaDuz,
                                                      prvaDuzZaPresek,
                                                      presek)) {
            if (_preseciSet.find(presek) == _preseciSet.end()) {
                _preseci.push_back(presek);
                _preseciSet.insert(presek);
            }
            AlgoritamBaza_updateCanvasAndBlock()
        }
    }
    /* Koristimo samo zbog provere da li presek vec postoji */
    _preseciSet.clear();

    /* 3. korak - Gremov algoritam O(nlogn) */
    gremovAlgoritam(false);
}

void ConvexHullLineIntersections::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;    

    auto olovka = painter->pen();
    unsigned i = 1;
    for(auto &kv : _mapaUgaoDuzi) {
       for (auto &d: kv.second) {
           /* Podesavanje stila olovke */
           olovka.setColor(Qt::black);
           olovka.setWidth(4);
           painter->setPen(olovka);
           /* Iscrtavanje duzi */
           painter->drawLine(d);

            /* Podesavanje stila olovke */
            olovka.setColor(Qt::magenta);
            olovka.setWidth(15);
            painter->setPen(olovka);

            /* Oznacavanje trenutne linije */
            std::stringstream s;
            s << "L" << i;
            naglasiTrenutno(painter, &d, i, s.str().c_str());
            i++;
        }
    }

    /* Podesavanje stila olovke */
    auto pen = painter->pen();
    pen.setColor(Qt::yellow);
    pen.setWidth(20);
    painter->setPen(pen);

    /* Iscrtavanje svih preseka */
    for(auto &presek: _preseci) {
        painter->drawPoint(presek);
    }

    /* Podesavanje stila olovke */
    pen.setColor(Qt::cyan);
    pen.setWidth(5);
    painter->setPen(pen);

    /* Iscrtavanje konveksnog omotaca */
    for(i = 1; i < _konveksniOmotac.size(); i++) {
       painter->drawLine(_konveksniOmotac[i-1],
                         _konveksniOmotac[i]);
    }
}

void ConvexHullLineIntersections::pokreniNaivniAlgoritam()
{
    /*
     * Ideja je da pronadjem sve preseke linija i da pozovem
     * neki konveksni (naivni ili Gremov) algoritam
    */

    /* Slozenost naivnog algoritma: O(n^2). Ona je
     * asimptotski optimalna za najgori slucaj. */
    QPointF presek;
    for (unsigned _i = 0; _i < _duzi.size(); _i++) {
        for (unsigned _j = _i+1; _j < _duzi.size(); _j++) {
            if (ConvexHullLineIntersections::presekLinija(_duzi[_i],
                                                          _duzi[_j],
                                                          presek)){
                _sviPreseci.push_back(presek);
            }
            AlgoritamBaza_updateCanvasAndBlock()
        }
    }
    AlgoritamBaza_updateCanvasAndBlock()

#ifdef GREMOV_NAIVNI
    ConvexHullLineIntersections::gremovAlgoritam(true);
#else
    /* Slozenost naivnog algoritma: O(n^3).
     * Prolazi se kroz svaki par tacaka. */
    for (_naivnoI = 0; _naivnoI < _sviPreseci.size(); _naivnoI++) {
        for (_naivnoJ = 0; _naivnoJ < _sviPreseci.size(); _naivnoJ++) {
            if (_naivnoI == _naivnoJ) continue;

            /* Proverava se da li su sve povrsine sa
             * trecom tackom negativne, sto znaci da
             * je trojka negativne orijentacije */
            bool svePovrsineNegativne = true;
            for (_naivnoK = 0; _naivnoK < _sviPreseci.size(); _naivnoK++) {
                if (_naivnoK == _naivnoI || _naivnoK == _naivnoJ) continue;

                _naivnoPovrsina = pomocneFunkcije::povrsinaTrouglaF(_sviPreseci[_naivnoI],
                                                                    _sviPreseci[_naivnoJ],
                                                                    _sviPreseci[_naivnoK]);
                AlgoritamBaza_updateCanvasAndBlock()
                if (_naivnoPovrsina >= 0) {
                    svePovrsineNegativne = false;
                    break;
                }
            }

            /* Ako jesu, dodaje se usmerena duz */
            _naivnoK = _sviPreseci.size();
            if (svePovrsineNegativne) {
                _naivniOmotac.emplace_back(_sviPreseci[_naivnoI], _sviPreseci[_naivnoJ]);
                AlgoritamBaza_updateCanvasAndBlock()
            }
        }
    }

    /* Ovde fix naivniOmotac */
//    std::ofstream myfile;
//    std::set<QPointF, ConvexHullLineIntersections::poredjenjeTacaka> naivniOmotacTacke;
//    for (auto &l: _naivniOmotac) {
//        naivniOmotacTacke.emplace(l.x1(), l.y1());
//    }

//    /* Proveravamo kolinearnost tacaka, izbacujemo sredisnje od kolinearnih */
//    for (unsigned i=0; i < naivniOmotacTacke.size()-2; i++) {
//        while (fabs(pomocneFunkcije::povrsinaTrouglaF(naivniOmotacTacke.at(i),
//                                                      naivniOmotacTacke.at(i+1),
//                                                      naivniOmotacTacke.at(i+2))) <= static_cast<double>(EPSf)) {
//            naivniOmotacTacke.erase(std::next(naivniOmotacTacke.begin(), i+1));
//        }
//    }
//    std::ofstream myfile;
//    myfile.open ("andja_naivninaivni.txt");
//    for (auto &p: _naivniOmotac) {
//        myfile << p.x1() << " " << p.y1() << " " << p.x2() << " " << p.y2() << std::endl;
//    }
//    myfile.close();

    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
#endif
}

void ConvexHullLineIntersections::crtajNaivniAlgoritam(QPainter *painter) const
{
    /* Odustajanje u slucaju greske */
    if (!painter) return;

    auto olovka = painter->pen();

    unsigned i = 1;
    for(const QLineF& d : _duzi) {
        /* Podesavanje stila olovke */
        olovka.setColor(Qt::black);
        olovka.setWidth(5);
        painter->setPen(olovka);

        /* Iscrtavanje duzi */
        painter->drawLine(d);

        /* Podesavanje stila olovke */
        olovka.setColor(Qt::green);
        olovka.setWidth(15);
        painter->setPen(olovka);

        /* Oznacavanje trenutne linije */
        std::stringstream s;
        s << "L" << i;
        naglasiTrenutno(painter, &d, i, s.str().c_str());
        i++;
    }

    /* Ako je algoritam u toku */
#ifndef GREMOV_NAIVNI
    if (_naivnoK < _sviPreseci.size()) {
        /* Podesavanje stila olovke */
        if (_naivnoPovrsina < 0) {
            olovka.setColor(Qt::green);
        } else {
            olovka.setColor(Qt::darkRed);
        }
        olovka.setWidth(10);
        painter->setPen(olovka);

        /* Crtanje tekuceg trougla */
        QPainterPath put(_sviPreseci[_naivnoI]);
        put.lineTo(_sviPreseci[_naivnoJ]);
        put.lineTo(_sviPreseci[_naivnoK]);
        put.lineTo(_sviPreseci[_naivnoI]);
        painter->fillPath(put, olovka.color());

        /* Crtanje tekuce ivice */
        olovka.setColor(Qt::red);
        painter->setPen(olovka);
        painter->drawLine(_sviPreseci[_naivnoI], _sviPreseci[_naivnoJ]);
    }
#endif
    /* Podesavanje stila olovke */
    olovka.setColor(Qt::yellow);
    painter->setPen(olovka);

    /* Crtanje svih preseka */
    for(auto &presek: _sviPreseci) {
        painter->drawPoint(presek);
    }

    /* Podesavanje stila olovke */
    olovka.setColor(Qt::magenta);
    olovka.setWidth(5);
    painter->setPen(olovka);

    /* Iscrtavanje konveksnog omotaca */
    if (!_naivniOmotac.empty()) {

        for(i = 0; i < _naivniOmotac.size(); i++) {
           painter->drawLine(_naivniOmotac[i]);
        }
    } else if (!_naivniOmotacGrem.empty()) {
        for(i = 1; i < _naivniOmotacGrem.size(); i++) {
           painter->drawLine(_naivniOmotacGrem[i-1],
                             _naivniOmotacGrem[i]);
        }
    }
}

void ConvexHullLineIntersections::gremovAlgoritam(bool naivni) {
    /* Slozenost ovakvog (Gremovog) algoritma: O(nlogn).
     * Dominira sortiranje, dok su ostali koraci linearni. */

    /* Ako radimo za naivni algoritam Grema */
    if (naivni){
        if (_sviPreseci.size() == 0) {
            emit animacijaZavrsila();
            return;
        }

        auto _maxTacka = _sviPreseci[0];
        for (unsigned i = 1; i < _sviPreseci.size(); i++) {
            if ( _sviPreseci[i].x() > _maxTacka.x() ||
                (pomocneFunkcije::bliski(_sviPreseci[i].x(), _maxTacka.x()) && _sviPreseci[i].y() < _maxTacka.y()) )
                _maxTacka = _sviPreseci[i];
        }

        AlgoritamBaza_updateCanvasAndBlock()

        std::sort(_sviPreseci.begin(),
                  _sviPreseci.end(),
                  [&](const auto& lhs, const auto& rhs) {
            int P = pomocneFunkcije::povrsinaTrouglaF(_maxTacka, lhs, rhs);
            return (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadratF(_maxTacka, lhs)
                                       < pomocneFunkcije::distanceKvadratF(_maxTacka, rhs));
        });

        /* Sredjuju se tacke koje su kolinearne sa prvom tackom u omotacu, zbog gresaka */
        unsigned p = 2;
        while (p < _sviPreseci.size()) {
            double P = pomocneFunkcije::povrsinaTrouglaF(_sviPreseci.at(p),
                                                       _sviPreseci.at(p-1),
                                                       _sviPreseci.at(0));
            P = static_cast<int>(P);
            if (pomocneFunkcije::bliski(P, 0)) {
                _sviPreseci.erase(std::next(_sviPreseci.begin(), (p-1)));
            }
            else {
                p += 1;
            }
        }

        _naivniOmotacGrem.push_back(_maxTacka);
        _naivniOmotacGrem.push_back(_sviPreseci[1]);

        unsigned tmp = 2;
        unsigned i = 2;

        while(i < _sviPreseci.size()) {
            if(pomocneFunkcije::povrsinaTrouglaF(_naivniOmotacGrem[tmp-2],
                                                 _naivniOmotacGrem[tmp-1],
                                                 _sviPreseci[i]) < 0)
            {
                _naivniOmotacGrem.push_back(_sviPreseci[i]);
                ++tmp;
                ++i;
            }
            else {
                _naivniOmotacGrem.pop_back();
                --tmp;
                // Ne smemo da povecamo i, jer nismo zavrsili sa ovom tackom
            }
            AlgoritamBaza_updateCanvasAndBlock()
        }

        /* Dodajemo prvu tacku opet, da zatvorimo omotac */
        _naivniOmotacGrem.push_back(_maxTacka); 

//        std::ofstream myfile;
//        myfile.open ("andja_naivni.txt");
//        for (auto &d: _naivniOmotacGrem) {
//            myfile << d.x() << " " << d.y() << std::endl;
//        }
//        myfile.close();
    }
    /* Ako radimo za optimalni algoritam Grema */
    else {
        if (_preseci.size() == 0) {
            emit animacijaZavrsila();
            return;
        }

        auto _maxTacka = _preseci[0];
        for (unsigned i = 1; i < _preseci.size(); i++) {
            if ( _preseci[i].x() > _maxTacka.x() || (pomocneFunkcije::bliski(_preseci[i].x(), _maxTacka.x()) && _preseci[i].y() < _maxTacka.y()) )
                _maxTacka = _preseci[i];
        }
        AlgoritamBaza_updateCanvasAndBlock()

        std::sort(_preseci.begin(),
                  _preseci.end(),
                  [&](const auto& lhs, const auto& rhs) {
            int P = pomocneFunkcije::povrsinaTrouglaF(_maxTacka, lhs, rhs);
            return (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadratF(_maxTacka, lhs)
                                        < pomocneFunkcije::distanceKvadratF(_maxTacka, rhs));
        });

        _konveksniOmotac.push_back(_maxTacka);
        _konveksniOmotac.push_back(_preseci[1]);

        unsigned tmp = 2;
        unsigned i = 2;

        while(i < _preseci.size()) {
            if(pomocneFunkcije::povrsinaTrouglaF(_konveksniOmotac[tmp-2],
                                                 _konveksniOmotac[tmp-1],
                                                 _preseci[i]) < 0)
            {
                _konveksniOmotac.push_back(_preseci[i]);
                ++tmp;
                ++i;
            }
            else {
                _konveksniOmotac.pop_back();
                --tmp;
                // Ne smemo da povecamo i, jer nismo zavrsili sa ovom tackom
            }
            AlgoritamBaza_updateCanvasAndBlock()
        }

        /* Dodajemo prvu tacku opet, da zatvorimo omotac */
        _konveksniOmotac.push_back(_maxTacka);

//        std::ofstream myfile;
//        myfile.open ("andja_optimalni.txt");
//        for (auto &d: _konveksniOmotac) {
//            myfile << d.x() << " " << d.y() << std::endl;
//        }
//        myfile.close();
    }
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}


void ConvexHullLineIntersections::naglasiTrenutno(QPainter *painter, const QLineF *d, unsigned long i, const char *s) const
{
    /* Transformacija cetkice */
    painter->save();
    painter->scale(1, -1);
    QLineF* duz = nullptr;
    if (_naivni) {
        duz = new QLineF(*std::next(_duzi.begin(), static_cast<int>(i-1)));
    } else {
        duz = new QLineF(*d);
    }
    painter->translate(0, -2*duz->p1().y()-40);

    /* Oznacavanje prvog temena */
    painter->drawText(duz->p1(), s);

    /* Ponistavanje transformacija */
    painter->restore();
    delete duz;
}

bool ConvexHullLineIntersections::presekLinija(const QLineF& l1, const QLineF& l2, QPointF& presek) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return l1.intersects(l2, &presek) == QLineF::BoundedIntersection ||
           l1.intersects(l2, &presek) == QLineF::UnboundedIntersection;
#else
    return l1.intersect(l2, &presek) == QLineF::BoundedIntersection ||
           l1.intersect(l2, &presek) == QLineF::UnboundedIntersection;
#endif
}

std::vector<QPointF> ConvexHullLineIntersections::vratiRazapinjuceTacke(double angle, double n) const {
    std::vector<QPointF> twoPoints;
    double scaleFactor = 1;
    /* Nadji maksimalne razapinjuce tacke prave na kanvasu */
    auto leftX = 0;
    auto bottomY = 0;
    auto rightX = 3200; // ako nemam _pcrtanje
    auto upY = 1500; // ako nemam _pcrtanje
    if (_pCrtanje) {
        rightX = _pCrtanje->width();
        upY = _pCrtanje->height();
    }

    auto intersectLeftY = angle*leftX + n;
    auto intersectRightY = angle*rightX + n;
    auto intersectBottomX = (bottomY-n)/angle;
    auto intersectUpX = (upY-n)/angle;

    if (intersectLeftY >= 0 && intersectLeftY <= (_pCrtanje ? _pCrtanje->height() : 1500)){
        twoPoints.emplace_back(scaleFactor*leftX, scaleFactor*intersectLeftY);
    }
    if (intersectRightY >= 0 && intersectRightY <= (_pCrtanje ? _pCrtanje->height() : 1500)){
        twoPoints.emplace_back(scaleFactor*rightX, scaleFactor*intersectRightY);
    }
    if (intersectBottomX >= 0 && intersectBottomX <= (_pCrtanje ? _pCrtanje->width() : 3200)){
        twoPoints.emplace_back(scaleFactor*intersectBottomX, scaleFactor*bottomY);
    }
    if (intersectUpX >= 0 && intersectUpX <= (_pCrtanje ? _pCrtanje->width() : 3200)){
        twoPoints.emplace_back(scaleFactor*intersectUpX, scaleFactor*upY);
    }

    if(twoPoints.size() < 2){
        std::cout << "Funkcija vratiRazapinjuceTacke je nasla manje od 2 :(" << std::endl;
    }
    return twoPoints;
}

void ConvexHullLineIntersections::generateAngles(const int N, const double minAngleDegree, const double maxAngleDegree, std::set<double> &angles) {
    for (int i = 0; i < N; i++) {
        bool angleOkay = true;
        while (angleOkay) {
            double angle = minAngleDegree + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX/(maxAngleDegree-minAngleDegree)));
            if (abs(angle - 90) <= 1) {
                continue;
            }
            angle = angle * M_PI / 180;
            auto n = angles.size();
            angles.insert(angle);
            if (angles.size() == (n + 1)) {
                angleOkay = false;
            }
        }
    }
}

std::vector<QLineF> ConvexHullLineIntersections::generisiNasumicneLinije(int brojDuzi)
{
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<QLineF> randomDuzi;
    if(brojDuzi == 0){
        return randomDuzi;
    }
    std::set<double> angles;

    /* Generisanje uglova */
    ConvexHullLineIntersections::generateAngles(brojDuzi, 1, 179, angles);

    /* Fiksiram onaj prvi presek */
    double firstIntersectX;
    double firstIntersectY;
    if (_pCrtanje){
        firstIntersectX = _pCrtanje->width()/2;
        firstIntersectY = _pCrtanje->height()/2;
    } else {
        firstIntersectX = 1600;
        firstIntersectY = 1000;
    }
    double intersectX=0, intersectY=0, n1=0, n2=0, lowX=0, highX=0, x1, x2;

    double firstAngle = tan(*angles.begin());
    double secondAngle;
    std::vector<QPointF> twoPoints;
    for (unsigned m=0; m < static_cast<unsigned>(brojDuzi)-1; m++) {
        twoPoints.clear();
        secondAngle = tan(*std::next(angles.begin(), m+1));

        /* Prva grana je da postavim prve dve linije */
        if (m==0) {
            intersectX = firstIntersectX;
            intersectY = firstIntersectY;
            n1 = intersectY - firstAngle*intersectX;
            n2 = intersectY - secondAngle*intersectX;

            twoPoints = ConvexHullLineIntersections::vratiRazapinjuceTacke(firstAngle, n1);

            if (twoPoints.size() < 2){
                std::cout << "[i==1]: Nije vratila funkcija vratiRazapinjuceTacke dovoljno :(" << std::endl;
                twoPoints = ConvexHullLineIntersections::vratiRazapinjuceTacke(firstAngle, 1000);
            }
            if (twoPoints[0].y() < twoPoints[1].y() ||
                    (pomocneFunkcije::bliski(twoPoints[0].y(), twoPoints[1].y())
                     && twoPoints[1].x() < twoPoints[0].x())) {
                randomDuzi.emplace_back(twoPoints[1].x(), twoPoints[1].y(),
                        twoPoints[0].x(), twoPoints[0].y());
            } else {
                randomDuzi.emplace_back(twoPoints[0].x(), twoPoints[0].y(), twoPoints[1].x(), twoPoints[1].y());
            }

            twoPoints.clear();
            twoPoints = ConvexHullLineIntersections::vratiRazapinjuceTacke(secondAngle, n2);

            if (twoPoints.size() < 2){
                std::cout << "[i==1]: Nije vratila funkcija vratiRazapinjuceTacke dovoljno :(" << std::endl;
                twoPoints = ConvexHullLineIntersections::vratiRazapinjuceTacke(firstAngle, 1000);
            }
            if (twoPoints[0].y() < twoPoints[1].y() ||
                    (pomocneFunkcije::bliski(twoPoints[0].y(), twoPoints[1].y())
                     && twoPoints[1].x() < twoPoints[0].x())) {
                randomDuzi.emplace_back(twoPoints[1].x(), twoPoints[1].y(),
                        twoPoints[0].x(), twoPoints[0].y());
            } else {
                randomDuzi.emplace_back(twoPoints[0].x(), twoPoints[0].y(), twoPoints[1].x(), twoPoints[1].y());
            }

            x1 = twoPoints[0].x();
            x2 = twoPoints[1].x();

            if (x1 < x2) {
                lowX = x1;
                highX = x2;
            } else {
                lowX = x2;
                highX = x1;
            }
        }
        /* Druga grana je da idem dalje kroz linije */
        else {
            auto randX = lowX + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX/(highX-lowX)));
            intersectY = firstAngle*randX + n1;
            intersectX = (intersectY - n1) / firstAngle;
            n2 = intersectY - secondAngle*intersectX;

            twoPoints.clear();
            twoPoints = ConvexHullLineIntersections::vratiRazapinjuceTacke(secondAngle, n2);

            if (twoPoints.size() < 2){
                std::cout << "[i>0]: Nije vratila funkcija vratiRazapinjuceTacke dovoljno :(" << std::endl;
                twoPoints = ConvexHullLineIntersections::vratiRazapinjuceTacke(firstAngle, 1000);
            }

            if (twoPoints[0].y() < twoPoints[1].y() ||
                    (pomocneFunkcije::bliski(twoPoints[0].y(), twoPoints[1].y())
                     && twoPoints[1].x() < twoPoints[0].x())) {
                randomDuzi.emplace_back(twoPoints[1].x(), twoPoints[1].y(),
                        twoPoints[0].x(), twoPoints[0].y());
            } else {
                randomDuzi.emplace_back(twoPoints[0].x(), twoPoints[0].y(), twoPoints[1].x(), twoPoints[1].y());
            }

            x1 = twoPoints[0].x();
            x2 = twoPoints[1].x();

            if (x1 < x2) {
                lowX = x1;
                highX = x2;
            } else {
                lowX = x2;
                highX = x1;
            }
        }

        /* Pripremim se da gledam drugu sa sledecom */
        firstAngle = secondAngle;
        n1 = n2;
    }

    std::vector<QLineF> duzi;
    for (auto &d: randomDuzi) {
        if (d.y1() < d.y2() || (pomocneFunkcije::bliski(d.y1(), d.y2()) && d.x2() < d.x1())) {
           duzi.emplace_back(d.x2(),
                             d.y2(),
                             d.x1(),
                             d.y1());
        }
        else {
           duzi.emplace_back(d.x1(),
                             d.y1(),
                             d.x2(),
                             d.y2());
        }
    }

    return duzi;
}

std::vector<QLineF> ConvexHullLineIntersections::ucitajPodatkeIzDatoteke(std::string imeDatoteke) const
{
    std::ifstream inputFile(imeDatoteke);
    std::vector<QLineF> duzi;

    double x1, y1, x2, y2;

    while(inputFile >> x1 >> y1 >> x2 >> y2)
    {
        if (y1 < y2 || (pomocneFunkcije::bliski(y1, y2) && x2 < x1)) {
           duzi.emplace_back(x2, y2, x1, y1);
        }
        else {
           duzi.emplace_back(x1, y1, x2, y2);
        }
    }
    return duzi;
}

const std::vector<QPointF> &ConvexHullLineIntersections::getPreseci() const {
    return _preseci;
}
const std::vector<QPointF> &ConvexHullLineIntersections::getNaivniPreseci() const {
    return _sviPreseci;
}
const std::vector<QPointF> &ConvexHullLineIntersections::getKonveksniOmotac() {
    std::sort(_konveksniOmotac.begin(),
              _konveksniOmotac.end(),
              [&](const auto &lhs, const auto &rhs) {
        return (lhs.x() < rhs.x() ||
                (fabs(lhs.x() - rhs.x()) < 1e-6f && lhs.y() < rhs.y()));
    });
    return _konveksniOmotac;
}
const std::vector<QPointF> &ConvexHullLineIntersections::getNaivniKonveksniOmotacGrem() {
    std::sort(_naivniOmotacGrem.begin(),
              _naivniOmotacGrem.end(),
              [&](const auto &lhs, const auto &rhs) {
        return (lhs.x() < rhs.x() ||
                (fabs(lhs.x() - rhs.x()) < 1e-6f && lhs.y() < rhs.y()));
    });
    return _naivniOmotacGrem;
}
const std::vector<QLineF> &ConvexHullLineIntersections::getNaivniKonveksniOmotac()
{
    return _naivniOmotac;
}

bool ConvexHullLineIntersections::getGREMOV_NAIVNI() const
{
    return _GREMOV_NAIVNI;
}
