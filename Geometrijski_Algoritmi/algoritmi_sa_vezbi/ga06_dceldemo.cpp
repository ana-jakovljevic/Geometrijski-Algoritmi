#include "ga06_dceldemo.h"

DCELDemo::DCELDemo(QWidget *pCrtanje,
                   int pauzaKoraka,
                   const bool &naivni,
                   std::string imeDatoteke,
                   int) /* brojTacaka, odnosno nasumicno ne koristimo u ovom primeru */
    :AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)/*,
     _polygon(imeDatoteke, pCrtanje->height(), pCrtanje->width())*/
{
}

void DCELDemo::pokreniAlgoritam() {
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void DCELDemo::crtajAlgoritam(QPainter *painter) const {
    if (!painter) return;

    /*
    QPen pen = painter->pen();

    for(auto i=0ul; i<_polygon.edges().size(); i+=2)
    {
         Crta se poligon.
        pen.setColor(Qt::yellow);
        painter->setPen(pen);
        painter->drawLine(_polygon.edges()[i]->origin()->coordinates(),
                         _polygon.edges()[i]->next()->origin()->coordinates());


         Crta se poligon "u suprotnom smeru", koriscenjem twin.
         * Da bi se video efekat na crtezu, koordinate su za malo pomerene u odnosu na
         * originalnu tacku.

        pen.setColor(Qt::red);
        painter->setPen(pen);
        painter->drawLine(_polygon.edges()[i]->twin()->origin()->coordinates().x() + 5,
                         _polygon.edges()[i]->twin()->origin()->coordinates().y() + 5,
                         _polygon.edges()[i]->twin()->next()->origin()->coordinates().x() - 5,
                         _polygon.edges()[i]->twin()->next()->origin()->coordinates().y() + 5);
   }

    int curr_num = 1;
    painter->setBrush(Qt::red);
    painter->setPen(Qt::white);
     Crtaju se temena, ali kao elipsa, radi lepote.
    for(Vertex* v: _polygon.vertices())
    {
       painter->drawEllipse(v->coordinates(), 10, 10);
       painter->drawText(v->coordinates().x() - 4, v->coordinates().y() + 4, QString::fromStdString(std::to_string(curr_num)));
       curr_num++;
    }
    */
}

void DCELDemo::pokreniNaivniAlgoritam() {
    emit animacijaZavrsila();
}

void DCELDemo::crtajNaivniAlgoritam(QPainter *painter) const
{
    if (!painter) return;
}
