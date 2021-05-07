#ifndef KERNEL_H
#define KERNEL_H

#include "algoritambaza.h"

class Kernel : public AlgoritamBaza
{
public:
    Kernel(QWidget *pCrtanje, int pauzaKoraka, const bool &naivni, const std::string& imeDatoteke,
           unsigned brojNasumicnihTacaka);
    void pokreniAlgoritam() override;
    void crtajAlgoritam(QPainter *painter) const override;
    void pokreniNaivniAlgoritam() override;
    void crtajNaivniAlgoritam(QPainter *painter) const override;

private:
    std::vector<QPoint> mCvorovi;
    std::vector<QPointF> mKernel;
};

#endif // KERNEL_H
