#ifndef CLOSESTPAIR_H
#define CLOSESTPAIR_H

#include <float.h>

#include "algoritambaza.h"


class ClosestPair : public AlgoritamBaza
{
public:
    ClosestPair(QWidget *pCrtanje,
                int pauzaKoraka,
                const bool &naivni = false,
                std::string imeDatoteke = "",
                int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    float bruteforce(unsigned long l, unsigned long r);
    float closest_pair_pomocna(unsigned long l, unsigned long r, std::vector<QPoint> tacke_sort_y);
    void draw();
    float nadji_najblize(std::vector<QPoint> tacke, float d);

    float min_dst() const;
    float min_dst_naive() const;

    QPoint best1() const;
    QPoint best2() const;

    QPoint best1_naive() const;
    QPoint best2_naive() const;

private:
    std::vector<QPoint> m_tacke;

    QPoint m_best1;
    QPoint m_best2;
    QPoint m_curr1;
    QPoint m_curr2;
    float m_curr_d = -1;
    float m_min_dst = FLT_MAX;
    QPoint m_curr_mid;

    QPoint m_best1_naive;
    QPoint m_best2_naive;
    float m_min_dst_naive = FLT_MAX;
};

#endif // CLOSESTPAIR_H
