#ifndef GA04_KONVEKSNI3DDATASTRUCTURES_H
#define GA04_KONVEKSNI3DDATASTRUCTURES_H

#include <QVector3D>

/* Flegovi */
#define NAOMOTACU  	true
#define UKLONI  	true
#define VIDLJIVA  	true
#define OBRADJENA	true

class Teme {
public:
    Teme(QVector3D tacka)
        :_koordinate(tacka), _obradjeno(false)
    {}

    /* Za pristup koordinatama */
    float x() { return _koordinate.x(); }
    float y() { return _koordinate.y(); }
    float z() { return _koordinate.z(); }
    QVector3D koordinate() { return _koordinate; }

    /* Da li je teme obradjeno i stavljeno u konveksni omotac. */
    bool getObradjeno() { return _obradjeno; }
    void setObradjeno(bool param) { _obradjeno = param; }

private:
    QVector3D _koordinate;
    bool _obradjeno;
};

class Stranica;

class Ivica{
public:
    Ivica()
        : _stranice {nullptr, nullptr},
          _obrisati {false},
          _temena {nullptr, nullptr}

    {}

    Ivica(Teme* t1, Teme* t2)
        : _stranice {nullptr, nullptr},
          _obrisati {false},
          _temena {t1, t2}
    {}

    void postavi_stranicu(Stranica* s)
    {
        if (_stranice[0] == nullptr)
            _stranice[0] = s;
        else if (_stranice[1] == nullptr)
            _stranice[1] = s;
    }

    Teme* t1() { return _temena[0]; }
    Teme* t2() { return _temena[1]; }
    Stranica* s1() { return _stranice[0]; }
    Stranica* s2() { return _stranice[1]; }

    /* Ukoliko su obe stranice za koje ova ivica povezuje vidljive
     * onda se ta ivica brise.
     */
    void setObrisati(bool param) { _obrisati = param; }
    bool obrisati() { return _obrisati; }

    /* Ukoliko je stranica vidljiva iz nove tacke, onda se stara stranica brise, a
     * nova stranica se postavlja kao nova stranica za datu ivicu. */
    void zameniVidljivuStranicu(Stranica* s, int i) { _stranice[i] = s; }

    /* Redosled tema odredjuje zapravo smer vektora.
     * Vektor je teme0->teme1.
     * Vektor odredju za datu stranicu koji deo je "unutra" omotaca, a
     * koja strana je "spolja" omotaca.
     * Kako se nove tacke dodaju, ono sto je "unutra" i "spolja" moze se promeniti,
     * pa se redosled tacaka mora azurirati.
     */
    void izmeniRedosledTemena()
    {
        Teme* pom = _temena[0];
        _temena[0] = _temena[1];
        _temena[1] = pom;
    }

private:
    Stranica* _stranice[2];
    bool _obrisati;
    Teme* _temena[2];
};

class Stranica{
public:
    Stranica()
        : _temena{nullptr,nullptr,nullptr},
          _vidljiva(false)
    {}

    Stranica(Teme* t1, Teme* t2, Teme* t3)
        : _temena{t1, t2, t3},
          _vidljiva(false)
    { }

    Teme* t1() { return _temena[0]; }
    Teme* t2() { return _temena[1]; }
    Teme* t3() { return _temena[2]; }

    /* Isto kao i kod ivica. Ukoliko se izmeni sta je "vidljiva strana",
     * onda je potrebno izmeniti redosled tacaka.
     */
    void izmeniRedosledTemena()
    {
        Teme* pom = _temena[0];
        _temena[0] = _temena[1];
        _temena[1] = pom;
    }

    /* Ovaj parametar sluzi za proveru da li je data stranica vidljiva iz nove tacke.
     * Ukoliko je to slucaj, ona se brise.
     */
    void setVidljiva(bool param) { _vidljiva = param; }
    bool getVidljiva() { return _vidljiva; }

private:
    Teme* _temena[3];
    bool _vidljiva;
};

#endif // GA04_KONVEKSNI3DDATASTRUCTURES_H
