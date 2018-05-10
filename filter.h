#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(QObject *parent = 0);

    Mat src, res;
    QImage res1;
    double sigma = 1.0;
    int size = 3;

    int function(QString, QString);

    void CustomBlur();
    uchar Median(uchar, uchar, uchar, uchar, uchar, uchar, uchar, uchar, uchar);
    void CustomMedianBlur();
    double **getGuassionArray();
    void CustomGaussianBlur();

    void getSigma(double);
    void getKisze(int);

    void sendSingal_filter();

signals:
    void filterSignal(QImage);

public slots:
};

#endif // FILTER_H
