#ifndef TIMEMEASUREMENTTHREAD_H
#define TIMEMEASUREMENTTHREAD_H

#include <QThread>

/* QChart */
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

#include "algoritambaza.h"

class TimeMeasurementThread : public QThread
{
    Q_OBJECT

public:
    TimeMeasurementThread(QString algorithmType, int minValue, int step, int maxValue);

    // QThread interface
    void run();

signals:
    void updateChart(double dim, double optimal, double naive);

private:
    QString _algorithmType;
    int _minValue;
    int _step;
    int _maxValue;
};

#endif // TIMEMEASUREMENTTHREAD_H
