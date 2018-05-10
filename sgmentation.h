#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QImage>

using namespace cv;
using namespace std;

#include <QObject>

class Sgmentation : public QObject
{
    Q_OBJECT
public:
    explicit Sgmentation(QObject *parent = 0);

    Mat src, res, src_gray;
    Mat kernel, imgLaplance, sharpenImg, resultImg, binaryImg, distImg, k1, dist_8u, markers, mark;
    vector<vector<Point>> contours;
    vector<Vec3b> colors;
    QImage res1;

    int threshold_value = 127;
    int threshold_type = 0;

    int width;
    int height;

    int function(QString, int);

    void delWatershed();
    void delThreshold();

    void getThres_value(int);

    void sendSingal_sgmentation();

signals:
    void sgmentationSignal(QImage);

public slots:
};

#endif // SEGMENTATION_H
