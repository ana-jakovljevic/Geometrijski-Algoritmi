#include <stdlib.h>
#include <float.h>

#include "ga12_closestpair.h"



ClosestPair::ClosestPair(QWidget *pCrtanje, int pauzaKoraka,
                         const bool &naivni,
                         std::string imeDatoteke,
                         int brojTacaka)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    if (imeDatoteke == "")
      m_tacke = generisiNasumicneTacke(brojTacaka);
    else
      m_tacke = ucitajPodatkeIzDatoteke(imeDatoteke);

    m_best1 = m_tacke[0];
    m_best2 = m_tacke[1];
}

static float distance(QPoint p1, QPoint p2)
{
    return sqrtf((p1.x() - p2.x()) * (p1.x() - p2.x()) +
                 (p1.y() - p2.y()) * (p1.y() - p2.y()));
}


float ClosestPair::bruteforce(unsigned long l, unsigned long r)
{
    float min = FLT_MAX;

    for (auto i = l; i < r; i++) {
        for (auto j = i + 1; j < r; j++) {
            float dst = distance(m_tacke[i], m_tacke[j]);

            if (dst < min) {  // Locally
                min = dst;
            }

            if (dst < m_min_dst) {  // Globally for visualization
                m_min_dst = dst;
                m_best1 = m_tacke[i];
                m_best2 = m_tacke[j];
            }
        }
    }

    return min;
}

float ClosestPair::nadji_najblize(std::vector<QPoint> tacke, float d)
{
    float min = d;
    m_curr_d = d;

    for (unsigned i = 0; i < tacke.size(); i++) {
        m_curr1 = tacke[i];

        for (unsigned j = i + 1; j < tacke.size() && (tacke[j].y() - tacke[i].y()) < min; j++) {
            m_curr2 = tacke[j];

            float dst = distance(tacke[i], tacke[j]);

            if (dst < min) { // Locally
                min = dst;
            }

            if (dst < m_min_dst) {  // Globally for visualization
                m_min_dst = dst;
                m_best1 = tacke[i];
                m_best2 = tacke[j];
            }

            draw();
        }
    }

    return min;
}

float ClosestPair::min_dst() const
{
    return m_min_dst;
}

float ClosestPair::min_dst_naive() const
{
    return m_min_dst_naive;
}

QPoint ClosestPair::best1() const
{
    return m_best1;
}

QPoint ClosestPair::best2() const
{
    return m_best2;
}

QPoint ClosestPair::best1_naive() const
{
    return m_best1_naive;
}

QPoint ClosestPair::best2_naive() const
{
    return m_best2_naive;
}

float ClosestPair::closest_pair_pomocna(unsigned long l, unsigned long r, std::vector<QPoint> tacke_sort_y)
{
    auto n = r - l;
    if ((r - l) <= 3) {
        return bruteforce(l, r);
    }

    auto mid = l + n / 2;
    QPoint srednja = m_tacke[mid];
    m_curr_mid = srednja;

    std::vector<QPoint> tacke_y_sort_levo;
    std::vector<QPoint> tacke_y_sort_desno;
    for (unsigned i = 0; i < tacke_sort_y.size(); i++) {
        if (tacke_sort_y[i].x() < srednja.x() || (tacke_sort_y[i].x() == srednja.x() && tacke_sort_y[i].y() < srednja.y())) { // fali li < mid
            tacke_y_sort_levo.push_back(tacke_sort_y[i]);
        } else {
            tacke_y_sort_desno.push_back(tacke_sort_y[i]);
        }
    }

    float dl = closest_pair_pomocna(l, mid, tacke_y_sort_levo);
    float dr = closest_pair_pomocna(mid+1, r, tacke_y_sort_desno);

    float d = (dl < dr) ? dl : dr;
    m_curr_d = d;

    std::vector<QPoint> tmp;
    for (unsigned i = 0; i < n; i++) {
        if (abs(tacke_sort_y[i].x() - srednja.x()) < d) {
            tmp.push_back(tacke_sort_y[i]);
        }
    }

    return std::min(d, nadji_najblize(tmp, d));
}

void ClosestPair::pokreniAlgoritam()
{
    std::sort(m_tacke.begin(), m_tacke.end(), [](const auto& p1, const auto& p2) {
        return (p1.x() != p2.x()) ? (p1.x() < p2.x()) : (p1.y() < p2.y());
    });

    std::vector<QPoint> tacke_sort_y;
    for (unsigned i = 0; i < m_tacke.size(); i++) {
        tacke_sort_y.push_back(m_tacke[i]);
    }

    std::sort(tacke_sort_y.begin(), tacke_sort_y.end(), [](const auto& p1, const auto& p2) {
        return (p1.y() != p2.y()) ? (p1.y() < p2.y()) : (p1.x() < p2.x());
    });

    m_min_dst = closest_pair_pomocna(0, m_tacke.size(), tacke_sort_y);
    m_curr_d = -1;
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void ClosestPair::pokreniNaivniAlgoritam()
{
    float min = FLT_MAX;

    for (unsigned i = 0; i < m_tacke.size(); i++) {
        m_curr1 = m_tacke[i];

        for (unsigned j = i+1; j < m_tacke.size(); j++) {
            m_curr2 = m_tacke[j];

            if (distance(m_tacke[i], m_tacke[j]) < min) {
                min = distance(m_tacke[i], m_tacke[j]);

                m_best1_naive = m_tacke[i];
                m_best2_naive = m_tacke[j];
            }

            AlgoritamBaza_updateCanvasAndBlock()
        }
    }

    m_min_dst_naive = min;

    emit animacijaZavrsila();
}

void ClosestPair::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    QPen p = painter->pen();
    p.setWidth(5);

    for(const QPoint &pt: m_tacke) {
        p.setColor(Qt::black);

        painter->setPen(p);
        painter->drawPoint(pt);
    }

    p.setWidth(3);
    p.setStyle(Qt::DashLine);

    p.setColor(Qt::green);
    painter->setPen(p);
    painter->drawLine(m_best1_naive.x(), m_best1_naive.y(), m_best2_naive.x(), m_best2_naive.y());

    p.setColor(Qt::red);
    painter->setPen(p);
    painter->drawLine(m_curr1.x(), m_curr1.y(), m_curr2.x(), m_curr2.y());
}

void ClosestPair::crtajAlgoritam(QPainter *painter) const
{
    if (!painter) return;

    QPen p = painter->pen();
    p.setWidth(5);
    painter->setPen(p);

    for(const QPoint &pt: m_tacke) {
        p.setColor(Qt::black);

        painter->setPen(p);
        painter->drawPoint(pt);
    }

    if (!pomocneFunkcije::bliski(m_curr_d, -1)) {
        p.setWidth(3);

        p.setColor(Qt::red);
        painter->setPen(p);

        QPoint point = m_curr_mid;
        painter->drawLine(point.x(), 0, point.x(), _pCrtanje->height());

        p.setStyle(Qt::DotLine);
        p.setColor(Qt::yellow);
        painter->setPen(p);
        painter->drawLine(static_cast<int>(point.x() - m_curr_d),
                          0,
                          static_cast<int>(point.x() - m_curr_d),
                          _pCrtanje->height());
        painter->drawLine(static_cast<int>(point.x() + m_curr_d),
                          0,
                          static_cast<int>(point.x() + m_curr_d),
                          _pCrtanje->height());

        p.setStyle(Qt::DashLine);
        p.setColor(Qt::red);
        painter->setPen(p);
        painter->drawLine(m_curr1.x(), m_curr1.y(), m_curr2.x(), m_curr2.y());

    }

    p.setStyle(Qt::DashLine);
    p.setColor(Qt::green);
    p.setWidth(3);
    painter->setPen(p);
    painter->drawLine(m_best1.x(), m_best1.y(), m_best2.x(), m_best2.y());
}

void ClosestPair::draw()
{
   AlgoritamBaza_updateCanvasAndBlock()
}
