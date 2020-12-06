#include "ga05_preseciduzi.h"

#include <fstream>

PreseciDuzi::PreseciDuzi(QWidget *pCrtanje,
                         int pauzaKoraka,
                         QCheckBox *const naivni,
                         std::string imeDatoteke,
                         int brojDuzi)
   :AlgoritamBaza(pCrtanje, pauzaKoraka, naivni), _redDuzi(poredjenjeDuzi(&y_brisuce_prave))
{
    if (imeDatoteke != "")
        _duzi = ucitajPodatkeIzDatoteke(imeDatoteke);
    else
        _duzi = generisiNasumicneDuzi(brojDuzi);
}

void PreseciDuzi::pokreniAlgoritam()
{
    /* Slozenost ovakvog algoritma: O(nlogn + klogn).
     * Izlazni parametar k je broj preseka. */
    for(auto duz : _duzi) {
        _redDogadjaja.emplace(duz.p1(), tipDogadjaja::POCETAK_DUZI, &duz, nullptr);
        _redDogadjaja.emplace(duz.p2(), tipDogadjaja::KRAJ_DUZI, &duz, nullptr);
    }

    while(!_redDogadjaja.empty()){
         auto td = *_redDogadjaja.begin();
         _redDogadjaja.erase(_redDogadjaja.begin());

         if(td.tip == tipDogadjaja::POCETAK_DUZI){
             y_brisuce_prave = td.tacka.y();

             auto trenutna=_redDuzi.emplace(td.duz1).first;
             if(trenutna!=_redDuzi.begin()){
                 auto prethodna= std::prev(trenutna);
                 QPointF presek;
                 if(pomocneFunkcije::presekDuzi(**trenutna, **prethodna, &presek))
                    _redDogadjaja.emplace(presek, tipDogadjaja::PRESEK,*trenutna,*prethodna);
             }

             auto sledeca = std::next(trenutna);
             if(sledeca != _redDuzi.end()) {
                 QPointF presek;
                 if(pomocneFunkcije::presekDuzi(**trenutna, **sledeca, &presek))
                    _redDogadjaja.emplace(presek, tipDogadjaja::PRESEK, *trenutna, *sledeca);
             }
          }
         else if (td.tip == tipDogadjaja::KRAJ_DUZI) {
            y_brisuce_prave = td.tacka.y();
            auto tr_duz = _redDuzi.find(td.duz1);
            auto sledeca = std::next(tr_duz);

            if (tr_duz != _redDuzi.begin() && sledeca != _redDuzi.end()) {
                auto prethodna = std::prev(tr_duz);
                QPointF presek;
                if (pomocneFunkcije::presekDuzi(**prethodna, **sledeca, &presek) && presek.y() <= y_brisuce_prave)
                   _redDogadjaja.emplace(presek, tipDogadjaja::PRESEK, *prethodna, *sledeca);
            }
            _redDuzi.erase(tr_duz);
         }
         else /*if (td.tip == tipDogadjaja::PRESEK)*/ {
             _preseci.push_back(td.tacka);
             y_brisuce_prave = td.tacka.y();

             _redDuzi.erase(td.duz1);
             _redDuzi.erase(td.duz2);

             auto duz1 = _redDuzi.insert(td.duz1).first;
             auto duz2 = _redDuzi.insert(td.duz2).first;
         }
    }
}

void PreseciDuzi::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;
}

void PreseciDuzi::pokreniNaivniAlgoritam()
{
    /* Slozenost naivnog algoritma: O(n^2). Ona je
     * asimptotski optimalna za najgori slucaj. */
    QPointF presek;
    for (auto i = 0ul; i < _duzi.size(); i++) {
        for (auto j = i+1; j < _duzi.size(); j++) {
            if (pomocneFunkcije::presekDuzi(_duzi[i], _duzi[j], &presek))
                _naivniPreseci.push_back(presek);
        }
    }
    emit animacijaZavrsila();
}

void PreseciDuzi::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;
}

std::vector<QLineF> PreseciDuzi::generisiNasumicneDuzi(int brojDuzi) const
{
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<QLineF> randomDuzi;

    std::vector<QPoint> tacke_za_duzi = generisiNasumicneTacke(2*brojDuzi);

    for(auto i=0ul; i < tacke_za_duzi.size()-1; i+=2){
        auto x1 = tacke_za_duzi[i].x();
        auto x2 = tacke_za_duzi[i+1].x();
        auto y1 = tacke_za_duzi[i].y();
        auto y2 = tacke_za_duzi[i+1].y();

        if (y1 < y2 || (y1 == y2 && x2 < x1)) {
            auto tmp_y = y1;
            auto tmp_x = x1;
            y1 = y2;
            x1 = x2;
            y2 = tmp_y;
            x2 = tmp_x;
        }
        randomDuzi.emplace_back(x1, y1, x2, y2);
    }

    return randomDuzi;
}

std::vector<QLineF> PreseciDuzi::ucitajPodatkeIzDatoteke(std::string imeDatoteke) const
{
    std::ifstream inputFile(imeDatoteke);
    std::vector<QLineF> duzi;

    double x1, y1, x2, y2;

    while(inputFile >> x1 >> y1 >> x2 >> y2)
    {
        if (y1 < y2 || (fabs(y1-y2) < EPS && x2 < x1)) {
            auto tmp_y = y1;
            auto tmp_x = x1;
            y1 = y2;
            x1 = x2;
            y2 = tmp_y;
            x2 = tmp_x;
        }

        duzi.emplace_back(x1, y1, x2, y2);
    }

    return duzi;
}
