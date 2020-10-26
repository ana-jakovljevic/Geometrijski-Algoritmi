#ifndef OBLASTCRTANJA_H
#define OBLASTCRTANJA_H

#include <QWidget>
#include <QtGui>

#include "algoritambaza.h"

class OblastCrtanja : public QWidget
{
    Q_OBJECT
public:
    explicit OblastCrtanja(QWidget *parent = nullptr);
    void postaviAlgoritamKojiSeIzvrsava(AlgoritamBaza *pAlgoritamBaza);

    void set_obrisiSve(bool param);

protected:
    ///
    /// \brief paintEvent -
    ///         Funkcija (preciznije event handler) u kojoj je implementirano
    ///         iscrtavanje po nasoj oblasti za crtanje.
    ///
    void paintEvent(QPaintEvent*) override;

private:
    AlgoritamBaza *_pAlgoritamBaza;
    bool _obrisiSve;
};

#endif // OBLASTCRTANJA_H
