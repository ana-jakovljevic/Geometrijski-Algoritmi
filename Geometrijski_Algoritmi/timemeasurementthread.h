#ifndef TIMEMEASUREMENTTHREAD_H
#define TIMEMEASUREMENTTHREAD_H

#include <QThread>

/* QChart */
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

#include "algoritambaza.h"
enum class TipAlgoritma;

class TimeMeasurementThread : public QThread
{
    Q_OBJECT

public:
    TimeMeasurementThread(TipAlgoritma algorithmType, int minValue, int step, int maxValue);

    // QThread interface
    void run();

signals:
    void updateChart(double dim, double optimal, double naive);
    void finishChart();

private:
    TipAlgoritma _algorithmType;
    int _minValue;
    int _step;
    int _maxValue;
};

#endif // TIMEMEASUREMENTTHREAD_H
