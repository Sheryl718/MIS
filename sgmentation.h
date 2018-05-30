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

    Mat pixelsChange;
    Mat temp1, temp2;
    Mat src, res, src_gray;
    Mat kernel, resultImg, binaryImg, distImg, k1, dist_8u, markers, mark;
    Mat structringElement;
    Mat kernel_x, kernel_y;
    vector<vector<Point>> contours;
    vector<Vec3b> colors;
    Scalar color;
    vector<Vec4i> hierachy;
    QImage res1;

    int threshold_value = 127;
    QString threshold_type;

    int width;
    int height;

    int cols;
    int rows;
    int offsetx;
    float alpha;
    float beta;
    float b;
    float g;
    float r;
    float h;
    int element_size;
    int element_max_size;

    int function(QString, QString);

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
    void delPixelsChange(QString);
    void delDistanceTransform();
    void delNormalize();

    void delWatershed();
    void delThreshold();

    void getThres_value(int);

    void sendSingal_sgmentation();

signals:
    void sgmentationSignal(QImage);
public slots:
};

#endif // SEGMENTATION_H
