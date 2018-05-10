#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Preprocessing : public QObject
{
    Q_OBJECT
public:
    explicit Preprocessing(QObject *parent = 0);

    Mat src, res, temp1, temp2;
    QImage res1;

    Mat structringElement;
    Mat kernel_x, kernel_y;
    int element_size;
    int element_max_size;

    int threshold_value = 127;
    int cols;
    int rows;
    int offsetx;
    float alpha;
    float beta;
    float b;
    float g;
    float r;
    float h;

    int function(QString, QString);

    void getThres_value(int);

    void delBGR2GRAY();
    void delMask();
    void delCustomImprove();
    void delErode();
    void delDilate();
    void delOpen();
    void delClose();
    void delMorGradient();
    void delTopHat();
    void delBlackHat();
    void delpyrDown();
    void delpyrUp();
    void delRobert(QString);
    void delSobel(QString);
    void delLaplace();
    void delCustomConvolution();
    void delCanny();
    void delEdge_sobel(QString);
    void delScharr(QString);
    void delEdge_laplace();
    void delContours();
    void delConvertScaleAbs();

    void sendSingal_preprocessing();

    Scalar color;
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    //RNG rng;
signals:
    void preprocessingSignal(QImage);

public slots:
};

#endif // PREPROCESSING_H
