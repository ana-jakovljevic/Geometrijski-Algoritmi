#ifndef BRISUCAPRAVA_H
#define BRISUCAPRAVA_H

#include "algoritambaza.h"

class BrisucaPrava : public AlgoritamBaza
{
public:
    BrisucaPrava(QWidget *pCrtanje, QOpenGLWidget *pCrtanjeGL,
                 int pauzaKoraka, std::string imeDatoteke = "",
                 int broj_tacaka = BROJ_NASUMICNIH_TACAKA);

    void pokreniAlgoritam();
    void crtajAlgoritam(QPainter &painter) const;
    void crtajAlgoritam3D() const;
    void pokreniNaivniAlgoritam();
    bool is_3D() const;

private:
    std::vector<QPoint> _tacke;
    int _yPoz;
};

#endif // BRISUCAPRAVA_H
