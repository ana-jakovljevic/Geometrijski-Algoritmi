#include "ga03_konveksniomotac.h"
#include "pomocnefunkcije.h"

#include <algorithm>

konveksniomotac::konveksniomotac(QWidget *pCrtanje,
                                 int pauzaKoraka,
                                 QCheckBox *const naivni,
                                 std::string imeDatoteke,
                                 int brojTacaka)
    :AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    if (imeDatoteke == "")
        _tacke = generisiNasumicneTacke(brojTacaka);
    else
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
}

void konveksniomotac::pokreniAlgoritam() {
    /* Slozenost ovakvog (Gremovog) algoritma: O(nlogn).
     * Dominira sortiranje, dok su ostali koraci linearni. */
    _maxTacka = _tacke[0];

    for (auto i = 1ul; i < _tacke.size(); i++) {
        if (_tacke[i].x() > _maxTacka.x() || (_tacke[i].x() == _maxTacka.x() && _tacke[i].y() < _maxTacka.y()))
            _maxTacka = _tacke[i];
    }
    AlgoritamBaza_updateCanvasAndBlock()

    std::sort(_tacke.begin(), _tacke.end(), [&](const auto& lhs, const auto& rhs) {
        int P = pomocneFunkcije::povrsinaTrougla(_maxTacka, lhs, rhs);
        return  (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadrat(_maxTacka, lhs)
                             < pomocneFunkcije::distanceKvadrat(_maxTacka, rhs));
    });

    _konveksniOmotac.push_back(_maxTacka);
    _konveksniOmotac.push_back(_tacke[1]);
    unsigned pom = 2;
    unsigned j = 2;

    while(j < _tacke.size()) {
        if(pomocneFunkcije::povrsinaTrougla(_konveksniOmotac[pom-2],
                                            _konveksniOmotac[pom-1],
                                            _tacke[j]) < 0)
        {
            _konveksniOmotac.push_back(_tacke[j]);
            ++pom;
            ++j;
        }
        else {
            _konveksniOmotac.pop_back();
            --pom;
            // Ne smemo da povecamo j u ovom slucaju, jer nismo zavrsili sa ovom tackom
        }
        AlgoritamBaza_updateCanvasAndBlock()
    }

    _konveksniOmotac.push_back(_maxTacka);
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void konveksniomotac::crtajAlgoritam(QPainter *painter) const {
    if (!painter) return;

    QPen pen = painter->pen();
    pen.setColor(Qt::red);
    painter->setPen(pen);
    for(auto tacka: _tacke) {
        painter->drawPoint(tacka);
    }

    pen.setColor(Qt::blue);
    painter->setPen(pen);
    for(auto i = 1ul; i < _konveksniOmotac.size(); i++) {
       painter->drawLine(_konveksniOmotac[i-1], _konveksniOmotac[i]);
    }
}

void konveksniomotac::pokreniNaivniAlgoritam() {
    /* Slozenost naivnog algoritma: O(n^3) */
    for (auto i = 0ul;i < _tacke.size(); i++) {
        for (auto j = 0ul; j < _tacke.size(); j++) {
            if (i == j){
                continue;
            }
            bool a = true;
            for (auto k = 0ul; k < _tacke.size(); k++){
                if (k == i || k == j){
                    continue;
                }
                if (pomocneFunkcije::povrsinaTrougla(_tacke[i], _tacke[j], _tacke[k]) > 0){
                    a = false;
                    break;
                }
            }
            if (a){
                if(std::find(_naivniOmotac.begin(), _naivniOmotac.end(),
                             _tacke[i]) == _naivniOmotac.end()){
                   _naivniOmotac.push_back(_tacke[i]);
                }
                if(std::find(_naivniOmotac.begin(), _naivniOmotac.end(),
                             _tacke[j]) == _naivniOmotac.end()){
                   _naivniOmotac.push_back(_tacke[j]);
                }
            }
        }
    }

    std::sort(_naivniOmotac.begin(), _naivniOmotac.end(),
              [&](const auto& lhs, const auto& rhs) {
        int P = pomocneFunkcije::povrsinaTrougla(_maxTacka, lhs, rhs);
        return  (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadrat(_maxTacka, lhs)
                             < pomocneFunkcije::distanceKvadrat(_maxTacka, rhs));
    });

    emit animacijaZavrsila();
}

void konveksniomotac::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;
}

std::vector<QPoint> konveksniomotac::getNaivniOmotac() const
{
    return _naivniOmotac;
}

std::vector<QPoint> konveksniomotac::getKonveksniOmotac() const
{
    return _konveksniOmotac;
}
