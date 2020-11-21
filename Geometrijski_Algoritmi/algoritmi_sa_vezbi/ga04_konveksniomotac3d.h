#ifndef KONVEKSNIOMOTAC3D_H
#define KONVEKSNIOMOTAC3D_H

#include<vector>
#include<QList>
#include <unordered_set>
#include <experimental/unordered_set>

#include "algoritambaza.h"
#include "ga04_konveksni3dDatastructures.h"

class KonveksniOmotac3D : public AlgoritamBaza
{
public:
    KonveksniOmotac3D(QWidget *pCrtanje,
                      int pauzaKoraka,
                      std::string imeDatoteke = "",
                      int broj_tacaka = BROJ_NASUMICNIH_TACAKA);


public:
    void pokreniAlgoritam();
    void crtajAlgoritam(QPainter *painter) const;
    void pokreniNaivniAlgoritam();

    /* Pomocne funkcije za generisanje konveksnog omotaca. */
    /* Glavne funkcije. */
    bool Tetraedar();
    void DodajTeme(Teme* t);
    void ObrisiVisak();

    /* Pomocne funkcije. */
    double zapremina6(Stranica* s, Teme* t) const;
    bool kolinearne(Teme* a, Teme* b, Teme* c) const;
    Stranica* napraviDruguStranicu(Ivica* i, Teme* t);
    Stranica* napraviPrvuStranicu(Ivica* iv, Teme* t);

    /* Ucitavanje podataka. */
    std::vector<Teme*> generisiNasumicneTacke(int broj_tacaka) const;
    std::vector<Teme*> ucitajPodatkeIzDatoteke(std::string imeDatoteke) const;

private:
    std::vector<Teme*> _tacke;
    QList<Stranica*> _stranice;

    /* Neuredjeni skup za cuvanje ivica; nije nam sustinski vazan redosled
     * ivica u skupu, pa nema potrebe za nizovima kod kojih su pretraga i
     * brisanje reda O(n) umesto jednostavnog O(1) */
    std::unordered_set<Ivica*, HashIvica, EqIvica> _ivice;

    /* Ipak, neophodno je tacno znati koje su novododate ivice, tako da
     * je potreban jedan pomocni vektor, ali se kroz njega samo iterira */
    std::vector<Ivica*> _noveIvice;
};

#endif // KONVEKSNIOMOTAC3D_H
