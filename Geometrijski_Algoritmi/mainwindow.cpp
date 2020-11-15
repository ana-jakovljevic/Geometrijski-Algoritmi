#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _pAlgoritamBaza(nullptr),
      _imeDatoteke(""),
      _duzinaPauze(DUZINA_PAUZE),
      _broj_nasumicnih_tacaka(BROJ_NASUMICNIH_TACAKA),
      _naivni(false)
{
    ui->setupUi(this);
    ui->tipAlgoritma->insertSeparator(6);
    animacijaButtonAktivni(false);
    animacijaParametriButtonAktivni(true);

    _pOblastCrtanjaOpenGL = ui->openGLWidget;

    _pOblastCrtanja = new OblastCrtanja(this);
    QBoxLayout *oblastCrtanjaLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    oblastCrtanjaLayout->addWidget(_pOblastCrtanja);
    ui->tab->setLayout(oblastCrtanjaLayout);

    ui->brojNasumicniTacaka->setPlaceholderText("Uneti broj nasumicnih tacaka, podrazumevana vrednost je 20.");

    /* Add chart */
    _optimalSeries = new QLineSeries();
    _naiveSeries = new QLineSeries();

    QChart *chart = new QChart();
    _optimalSeries->append(0,0);
    _naiveSeries->append(0,0);

    _optimalSeries->setName("Optimalni algoritam");
    _naiveSeries->setName("Naivni algoritam");

    chart->addSeries(_optimalSeries);
    chart->addSeries(_naiveSeries);

    chart->legend()->show();

    chart->createDefaultAxes();
    chart->setTitle("Poredjenje efikasnosti");

    chart->axes(Qt::Horizontal).back()->setRange(0, X_MAX_VAL);
    chart->axes(Qt::Vertical).back()->setRange(0, Y_MAX_VAL);

    // Same formatting
    chart->setBackgroundVisible(false);
    chart->setPlotAreaBackgroundVisible(true);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QBoxLayout *chartBoxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    chartBoxLayout->addWidget(chartView);

    ui->tab_2->setLayout(chartBoxLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animacijaButtonAktivni(bool param_aktivnosti)
{
    ui->Ispocetka_dugme->setEnabled(param_aktivnosti);
    ui->Sledeci_dugme->setEnabled(param_aktivnosti);
    ui->Zaustavi_dugme->setEnabled(param_aktivnosti);
    ui->Zapocni_dugme->setEnabled(param_aktivnosti);
    ui->merenjeButton->setEnabled(param_aktivnosti);
}

void MainWindow::animacijaParametriButtonAktivni(bool param_aktivnosti)
{
    ui->datoteka_dugme->setEnabled(!param_aktivnosti);
    ui->Ponisti_dugme->setEnabled(param_aktivnosti);
    ui->tipAlgoritma->setEnabled(param_aktivnosti);
    ui->Nasumicni_dugme->setEnabled(!param_aktivnosti);
}

void MainWindow::on_datoteka_dugme_clicked()
{
    QString imeDatoteke = QFileDialog::getOpenFileName(this, tr("Datoteka sa koordinatama tacaka"), "./ulazni_podaci/", "*.*");
    if (imeDatoteke.isEmpty())
        return;

    _imeDatoteke = imeDatoteke.toStdString();

    napraviNoviAlgoritam();
    animacijaButtonAktivni(true);
}

void MainWindow::on_Nasumicni_dugme_clicked()
{
    if (ui->brojNasumicniTacaka->text() != "" )
        _broj_nasumicnih_tacaka = ui->brojNasumicniTacaka->text().toInt();

    napraviNoviAlgoritam();
    animacijaButtonAktivni(true);
}

void MainWindow::on_Ponisti_dugme_clicked()
{
    ui->tipAlgoritma->setCurrentIndex(0);
    animacijaButtonAktivni(false);
    animacijaParametriButtonAktivni(true);

    _pOblastCrtanja->set_obrisiSve(true);
    _pOblastCrtanja->update();

    _pOblastCrtanjaOpenGL->set_obrisiSve(true);
    _pOblastCrtanjaOpenGL->update();

    ui->brojNasumicniTacaka->clear();
    ui->brojNasumicniTacaka->setPlaceholderText("Uneti broj nasumicnih tacaka, podrazumevana vrednost je 20.");
}

void MainWindow::on_Zapocni_dugme_clicked()
{
    ui->Zapocni_dugme->setEnabled(false);
    animacijaParametriButtonAktivni(false);
    ui->merenjeButton->setEnabled(false);

    if (_pAlgoritamBaza)
        _pAlgoritamBaza->pokreniAnimaciju();
}

void MainWindow::on_Zaustavi_dugme_clicked()
{
    if (_pAlgoritamBaza)
        _pAlgoritamBaza->pauzirajIliNastaviAnimaciju();
}

void MainWindow::on_Sledeci_dugme_clicked()
{
    if (_pAlgoritamBaza)
        _pAlgoritamBaza->sledeciKorakAnimacije();
}

void MainWindow::on_Ispocetka_dugme_clicked()
{
    ui->Zapocni_dugme->setEnabled(false);
    ui->Zaustavi_dugme->setEnabled(true);
    ui->Sledeci_dugme->setEnabled(true);

    if (_pAlgoritamBaza)
    {
        _pAlgoritamBaza->zaustaviAnimaciju();
        napraviNoviAlgoritam();
        _pAlgoritamBaza->pokreniAnimaciju();
    }
}

void MainWindow::on_tipAlgoritma_currentIndexChanged(int index)
{
    QString trenutniAlg = ui->tipAlgoritma->itemText(index);

    if (trenutniAlg == "SA CASOVA VEZBI" || trenutniAlg == "STUDENTSKI PROJEKTI")
    {
        ui->datoteka_dugme->setEnabled(false);
        ui->Nasumicni_dugme->setEnabled(false);
        animacijaButtonAktivni(false);
    }
    else
    {
        ui->datoteka_dugme->setEnabled(true);
        ui->Nasumicni_dugme->setEnabled(true);
        animacijaButtonAktivni(false);
        ui->merenjeButton->setEnabled(true);
    }
}

/* za Chart, poredjenje. */
void MainWindow::on_merenjeButton_clicked()
{
    QString tipAlgoritma = ui->tipAlgoritma->currentText();

    _mThread = new TimeMeasurementThread(tipAlgoritma, MIN_DIM, STEP, MAX_DIM);

    connect(_mThread, &TimeMeasurementThread::updateChart, this, &MainWindow::on_lineSeriesChange);
    _mThread->start();
}

void MainWindow::on_lineSeriesChange(double dim, double optimal, double naive)
{
    _optimalSeries->append(dim, optimal);
    _naiveSeries->append(dim, naive);
}


void MainWindow::na_krajuAnimacije()
{
    ui->tipAlgoritma->setEnabled(true);
    ui->Ponisti_dugme->setEnabled(true);
    animacijaButtonAktivni(false);
    ui->Ispocetka_dugme->setEnabled(true);
}

void MainWindow::napraviNoviAlgoritam()
{
    _pOblastCrtanja->set_obrisiSve(false);
    _pOblastCrtanjaOpenGL->set_obrisiSve(false);

    delete _pAlgoritamBaza;
    _pAlgoritamBaza = nullptr;

    QString tipAlgoritma = ui->tipAlgoritma->currentText();

    /* Ovde se kreiraju instance algoritama pozivom njihovih konstruktora. Svi
       2D algoritmi crtaju po _pOblastCrtanja, a 3D po _pOblastCrtanjaOpenGL. */
    if (tipAlgoritma == "Demonstracija iscrtavanja")
        _pAlgoritamBaza = new DemoIscrtavanja(_pOblastCrtanja, _duzinaPauze, _imeDatoteke, _broj_nasumicnih_tacaka);
    else if (tipAlgoritma == "Brisuca prava")
        _pAlgoritamBaza = new BrisucaPrava(_pOblastCrtanja, _duzinaPauze, _imeDatoteke, _broj_nasumicnih_tacaka);
    else if (tipAlgoritma == "3D iscrtavanje")
        _pAlgoritamBaza = new Discrtavanje(_pOblastCrtanjaOpenGL, _duzinaPauze, _imeDatoteke, _broj_nasumicnih_tacaka);
    else if (tipAlgoritma == "Konveksni omotac")
        _pAlgoritamBaza = new konveksniomotac(_pOblastCrtanja, _duzinaPauze, _imeDatoteke, _broj_nasumicnih_tacaka);
    else if (tipAlgoritma == "Konveksni omotac 3D")
        _pAlgoritamBaza = new KonveksniOmotac3D(_pOblastCrtanjaOpenGL, _duzinaPauze, _imeDatoteke, _broj_nasumicnih_tacaka);

    if (_pAlgoritamBaza)
    {
        _pOblastCrtanja->postaviAlgoritamKojiSeIzvrsava(_pAlgoritamBaza);
        _pOblastCrtanja->update();

        _pOblastCrtanjaOpenGL->postaviAlgoritamKojiSeIzvrsava(_pAlgoritamBaza);
        _pOblastCrtanjaOpenGL->update();

        connect(_pAlgoritamBaza, &AlgoritamBaza::animacijaZavrsila, this, &MainWindow::na_krajuAnimacije);
    }
}
