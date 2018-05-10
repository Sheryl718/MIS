#ifndef FUSION_H
#define FUSION_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QImage>

using namespace cv;
using namespace std;

class Fusion : public QObject
{
    Q_OBJECT
public:
    explicit Fusion(QObject *parent = 0);

    Mat src1, src2, res;
    int custom_width;
    int custom_height;
    double alpha = 0.5;
    double gamma = 0.0;

    QImage res1;

    void customAdd();
    void customMultiply();
    void customAddWeighted();
    int function(QString, QString, QString);
    void sendSingal_fusion();
    void getAlpha(double);
    void getGamma(double);

signals:
    void fusionSignal(QImage);
    void errorSignal();

public slots:
};

#endif // FUSION_H
