#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "algoritambaza.h"
#include "oblastcrtanja.h"
#include "oblastcrtanjaopengl.h"
#include "config.h"
#include "timemeasurementthread.h"

/* Ovde ukljuciti zaglavlja novih algoritma. */
#include "ga00_demoiscrtavanja.h"
#include "ga01_brisucaprava.h"
#include "ga02_3discrtavanje.h"
#include "ga03_konveksniomotac.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_datoteka_dugme_clicked();

    void on_Nasumicni_dugme_clicked();

    void on_Ponisti_dugme_clicked();

    void on_Zapocni_dugme_clicked();

    void on_Zaustavi_dugme_clicked();

    void on_Sledeci_dugme_clicked();

    void on_Ispocetka_dugme_clicked();

    /* za Chart */
    void on_merenjeButton_clicked();
    void on_lineSeriesChange(double dim, double optimal, double naive);

    void on_tipAlgoritma_currentIndexChanged(int index);

public slots:
    void na_krajuAnimacije();

private:
    void animacijaButtonAktivni(bool param_aktivnosti);

    void animacijaParametriButtonAktivni(bool param_aktivnosti);

    void napraviNoviAlgoritam();

private:
    Ui::MainWindow *ui;

    AlgoritamBaza *_pAlgoritamBaza;
    OblastCrtanja *_pOblastCrtanja;
    OblastCrtanjaOpenGL *_pOblastCrtanjaOpenGL;
    std::string _imeDatoteke;
    int _duzinaPauze;
    int _broj_nasumicnih_tacaka;
    bool _naivni;

    /* Chart deo */
    QLineSeries *_naiveSeries;
    QLineSeries *_optimalSeries;

    TimeMeasurementThread *_mThread;
};

#endif // MAINWINDOW_H
