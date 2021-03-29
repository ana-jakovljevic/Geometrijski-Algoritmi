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

/**
 * BRUTE FORCE
 */
void KonturaPravougaonika::bf_pocetakPravougaonika(ivica* iv) {
//    std::cout << "Od " << iv->duz->y1() << " do " << iv->duz->y2() << std::endl;
    if (bf_tackeUKonturi.size() == 0) {
        dodajVertikalnuIvicu(iv);
        bf_tackeUKonturi.emplace(new tacka1d(_tip::ULAZ, iv->duz->y1()));
        bf_tackeUKonturi.emplace(new tacka1d(_tip::IZLAZ, iv->duz->y2()));
        return;
    }
    if (iv->duz->y1() < (*bf_tackeUKonturi.begin())->vr && iv->duz->y2() < (*bf_tackeUKonturi.begin())->vr) {
        // ivica je levo od postojećeg J i nema preseka
        dodajVertikalnuIvicu(iv);
    }
    else if (iv->duz->y1() > (*bf_tackeUKonturi.rbegin())->vr && iv->duz->y2() > (*bf_tackeUKonturi.rbegin())->vr) {
        // ivica je desno od postojećeg J i nema preseka
        dodajVertikalnuIvicu(iv);
    }
    else {
        auto i = bf_tackeUKonturi.begin();
        int _y = 0;
        float x = iv->duz->x1();
        int counter = 0;
        float startFrom = iv->duz->y1();
        while (i != bf_tackeUKonturi.end() && (*i)->vr <= iv->duz->y2()) {
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
    bf_tackeUKonturi.emplace(new tacka1d(_tip::ULAZ, iv->duz->y1()));
    bf_tackeUKonturi.emplace(new tacka1d(_tip::IZLAZ, iv->duz->y2()));

}

void KonturaPravougaonika::bf_krajPravougaonika(ivica* iv) {
    auto i = bf_tackeUKonturi.begin();
    float _y = 0;
    float x = iv->duz->x1();
    float counter = 0;
    float startFrom = iv->duz->y1();
    while (i != bf_tackeUKonturi.end() && (*i)->vr <= iv->duz->y2()) {
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
    auto found = std::find_if(bf_tackeUKonturi.begin(), bf_tackeUKonturi.end(), [&](const auto &val) {
        return val->vr == iv->duz->y1() && val->tip == _tip::ULAZ;
    });
    if (found != bf_tackeUKonturi.end()) bf_tackeUKonturi.erase(found);
    else std::cout << "Nije pronadjeno 1" << std::endl;
    found = std::find_if(bf_tackeUKonturi.begin(), bf_tackeUKonturi.end(), [&](const auto &val) {
        return val->vr == iv->duz->y2() && val->tip == _tip::IZLAZ;
    });
    if (found != bf_tackeUKonturi.end()) bf_tackeUKonturi.erase(found);
    else std::cout << "Nije pronadjeno 2" << std::endl;
}

void KonturaPravougaonika::pokreniNaivniAlgoritam() {
    std::vector<ivica*> ivicePragougaonika = {};
    for (QRectF &pravou : _pravougaonici) {
        QLineF *leva = new QLineF(pravou.left(), pravou.top(), pravou.left(), pravou.bottom());
        QLineF *desna = new QLineF(pravou.right(), pravou.top(), pravou.right(), pravou.bottom());
        ivicePragougaonika.push_back(new ivica(_tip::ULAZ, leva));
        ivicePragougaonika.push_back(new ivica(_tip::IZLAZ, desna));
    }
    std::sort(ivicePragougaonika.begin(), ivicePragougaonika.end(), [](ivica* a, ivica* b) {
        if (b->duz->x1() == a->duz->x1()) return b->duz->y1() > a->duz->y1();
        return b->duz->x1() > a->duz->x1();
    });
    for (ivica* iv : ivicePragougaonika) {
        if (iv->tip == _tip::ULAZ) {
            bf_pocetakPravougaonika(iv);
        } else {
            bf_krajPravougaonika(iv);
        }
        AlgoritamBaza_updateCanvasAndBlock();
    }
    AlgoritamBaza_updateCanvasAndBlock();
    faza2();
    emit animacijaZavrsila();
}

void KonturaPravougaonika::crtajNaivniAlgoritam(QPainter *painter) const { _crtajAlgoritam(painter); }

/**
 * SEGMENT TREE
 */

void KonturaPravougaonika::comp(Segment* v, bool inFull) {
    // console.log(`comp ${v.b}, ${v.e}, ${v.status}`, inFull);
    if (inFull) return;
    if (v->status == SegStatus::EMPTY) {
        if (st_stack.size() && v->b == st_stack.back()) {
            st_stack.pop_back();
        } else {
            st_stack.push_back(v->b);
        }
        st_stack.push_back(v->e);
    } else if (v->status == SegStatus::PARTIAL) {
        comp(v->lson, false);
        comp(v->rson, false);
    }
}

void KonturaPravougaonika::updateStatus(Segment* v) {
    if (v->c > 0) {
        v->status = SegStatus::FULL;
        return;
    }
    if (v->lson == 0 || v->rson == 0) {
        v->status = SegStatus::EMPTY; // leaf
        return;
    }
    if (v->lson->status == SegStatus::EMPTY && v->rson->status == SegStatus::EMPTY) {
        v->status = SegStatus::EMPTY;
        return;
    }
    v->status = SegStatus::PARTIAL;
}

/**
 * Insert segment [b, e] at node v of segment tree
 * @param b {number}
 * @param e {number}
 * @param v {Segment}
 **/
void KonturaPravougaonika::lp_insert(float b, float e, Segment* v, bool inFull) {
    if (!v) return;
    if (b <= v->b && v->e <= e) {
        comp(v, inFull || v->status == SegStatus::FULL);
        v->c += 1;
    } else {
        if (b < floor((v->b + v->e) / 2)) {
            lp_insert(b, e, v->lson, inFull || v->status == SegStatus::FULL);
        }
        if (floor((v->b + v->e) / 2) < e) {
            lp_insert(b, e, v->rson, inFull || v->status == SegStatus::FULL);
        }
    }
    updateStatus(v);
}

void KonturaPravougaonika::lp_delete(float b, float e, Segment* v, bool inFull) {
    if (!v) {
        return;
    }
    if (b <= v->b && v->e <= e) {
        v->c -= 1;
        updateStatus(v);
        comp(v, inFull || v->status == SegStatus::FULL);
    } else {
        if (b < floor((v->b + v->e) / 2)) {
            lp_delete(b, e, v->lson, inFull || v->status == SegStatus::FULL);
        }
        if (floor((v->b + v->e) / 2) < e) {
            lp_delete(b, e, v->rson, inFull || v->status == SegStatus::FULL);
        }
        updateStatus(v);
    }
}

std::vector<Tacka*> KonturaPravougaonika::normalizeEdgesForSegmentTree(std::vector<st_Ivica*> inEdges, std::vector<st_Ivica*> outEdges) {
    std::vector<Tacka*> points = {};
    for (auto edge : inEdges) {
        points.push_back(edge->pt1);
        points.push_back(edge->pt2);
    }
    std::sort(points.begin(), points.end(), [](Tacka* a, Tacka* b) {
        return a->y() < b->y();
    });
    int i = 0;
    for (auto p : points) {
        p->normalizovanY = i;
        i++;
    }
    i = 0;
    for (auto edge : inEdges) {
        outEdges[i]->pt1->normalizovanY = edge->pt1->normalizovanY;
        outEdges[i]->pt2->normalizovanY = edge->pt2->normalizovanY;
        i++;
    };
    return points;
}

void KonturaPravougaonika::pokreniAlgoritam() {
    std::vector<st_Ivica*> inEdges = {};
    std::vector<st_Ivica*> outEdges = {};
    for (auto r : _pravougaonici) {
        inEdges.push_back(new st_Ivica(new Tacka(r.left(), r.top()), new Tacka(r.left(), r.bottom()), _tip::ULAZ));
        outEdges.push_back(new st_Ivica(new Tacka(r.right(), r.top()), new Tacka(r.right(), r.bottom()), _tip::IZLAZ));
    }
    Segment* tree = new Segment(0, inEdges.size() * 2 - 1);
    auto sortedPoints = normalizeEdgesForSegmentTree(inEdges, outEdges);
    // data.contourEdges = [];
    std::vector<st_Ivica*> edges = inEdges;
    edges.insert(edges.end(), outEdges.begin(), outEdges.end());
    std::sort(edges.begin(), edges.end(), [](st_Ivica* a, st_Ivica* b) {
        if (a->pt1->x() == b->pt1->x()) {
            return a->tip == _tip::ULAZ;
        }
        return a->pt1->x() < b->pt1->x();
    });
    for (auto edge : edges) {
        if (edge->tip == _tip::ULAZ) { // insert
            lp_insert(edge->pt1->normalizovanY, edge->pt2->normalizovanY, tree, false);
        } else { // remove
            lp_delete(edge->pt1->normalizovanY, edge->pt2->normalizovanY, tree, false);
        }
        for (unsigned long i = 0; i < st_stack.size(); i += 2) {
            auto actualEdgeB = sortedPoints[st_stack[i]];
            auto actualEdgeE = sortedPoints[st_stack[i + 1]];
            // Ako je izlazna ivica, okrecemo je
            auto y1 = edge->tip == _tip::ULAZ ? actualEdgeB->y() : actualEdgeE->y();
            auto y2 = edge->tip == _tip::ULAZ ? actualEdgeE->y() : actualEdgeB->y();
            dodajVertikalnuIvicu(new ivica(
                edge->tip,
                new QLineF(
                  edge->pt1->x(),
                  y1,
                  edge->pt1->x(),
                  y2
                )
            ));
        }
        st_stack.clear();
        AlgoritamBaza_updateCanvasAndBlock();
    };

    AlgoritamBaza_updateCanvasAndBlock();
    faza2();
    emit animacijaZavrsila();
}

void KonturaPravougaonika::crtajAlgoritam(QPainter *painter) const { _crtajAlgoritam(painter); }

/**
 * ZAJEDNICKO
 **/

void KonturaPravougaonika::dodajVertikalnuIvicu(ivica* iv) {
    if (iv->duz->y1() != iv->duz->y2()) {
        ph1_vertikalneIvice.insert(iv);
        iviceKonture.push_back(iv->duz);
    }
}
void KonturaPravougaonika::dodajHorizontalnuIvicu(QLineF *duz) {
    if (duz->x1() != duz->x2()) {
        iviceKonture.push_back(duz);
    }
}

// todo pretvori ph1_vertikalneIvice u parametar
void KonturaPravougaonika::faza2() {
    std::vector<QLineF*> ph2_ivice = {};
    std::cout << "faza 2 " << iviceKonture.size() << " " << ph1_vertikalneIvice.size() << std::endl;
    for (auto iv: ph1_vertikalneIvice) {
//        iviceKonture.push_back(iv->duz);
        ph2_ivice.push_back(iv->duz);
        auto iv2 = new QLineF(iv->duz->x1(), iv->duz->y2(), iv->duz->x2(), iv->duz->y1());
        ph2_ivice.push_back(iv2);
    }
    std::sort(ph2_ivice.begin(), ph2_ivice.end(), [](QLineF* a, QLineF* b) {
        if (a->y1() == b->y1()) return a->x1() < b->x1();
        return a->y1() < b->y1();
    });

    for (unsigned long k = 0; k < ph2_ivice.size()/2; k++) {
        auto a = ph2_ivice[2*k];
        auto b = ph2_ivice[2*k + 1];

        float _y2 = a->y1() > a->y2() ? a->y1() : a->y2();
        float _y1 = a->y1() < a->y2() ? a->y1() : a->y2();

        bool isDownward = ph1_vertikalneIvice.end() == std::find_if(ph1_vertikalneIvice.begin(), ph1_vertikalneIvice.end(), [&](const auto &e) {
            return e->duz->x1() == a->x1() && e->duz->y1() == _y1 && e->duz->y2() == _y2;
        });

        float maxX = a->x1() > b->x1() ? a->x1() : b->x1();
        float minX = a->x1() < b->x1() ? a->x1() : b->x1();

        if ((isDownward && a->y1() < a->y2()) || (!isDownward && a->y2() < a->y1())) {
            dodajHorizontalnuIvicu(new QLineF(minX, a->y1(), maxX, a->y1()));
        } else {
            dodajHorizontalnuIvicu(new QLineF(maxX, a->y1(), minX, a->y1()));
        }
        AlgoritamBaza_updateCanvasAndBlock();
    }
}

void KonturaPravougaonika::_crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    auto olovka = painter->pen();
    olovka.setColor(Qt::black);
    olovka.setWidth(3);
    painter->setPen(olovka);
    for (auto pr : _pravougaonici) {
        painter->drawRect(pr);
        painter->fillRect(pr, QBrush(QColor(0, 0, 255, 100)));
    }
    olovka.setColor(Qt::red);
    painter->setPen(olovka);
    for (const auto duz: iviceKonture) {
        painter->drawLine(*duz);
        qreal arrowSize = 20;
        if (duz->length() > arrowSize) {
            double angle = std::atan2(-duz->dy(), duz->dx());
            QPointF arrowP1 = duz->p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
            QPointF arrowP2 = duz->p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);

            QPolygonF arrowHead;
            arrowHead.clear();
            arrowHead << arrowP2 << duz->p1() << arrowP1 << duz->p1();
            painter->drawPolygon(arrowHead);
        }
    }
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
