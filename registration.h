#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

class Registration : public QObject
{
    Q_OBJECT
public:
    explicit Registration(QObject *parent = 0);
    Mat src1, src2, res;
    Mat matrix;
    QImage res1;
    Mat descriptor_src1, descriptor_src2;
    int count;
    int minHessian;
    int numFeatures;
    double minDist;
    double maxDist;
    double dist;
    FlannBasedMatcher flann_matcher;
    BFMatcher bf_matcher;
    vector<DMatch> matches;
    vector<DMatch> goodMatches;
    vector<KeyPoint> keypoints_src1, keypoints_src2, goodkeypoints_src1, goodkeypoints_src2;
    vector<Point2f> forTrans_src1, forTrans_src2;
    Point2f tmpPoint;
    Ptr<SURF> surf_detector;
    Ptr<SIFT> sift_detector;
    Ptr<AKAZE> akaze_detector;
    Ptr<KAZE> kaze_detector;
    Ptr<Feature2D> brisk_detector;

    int function(QString, QString, QString);
    void delSurf();
    void delSift();
//    void delLbp();
    void delKaze();
    void delAkaze();
    void delBrisk();
    void delFlann();
    void delBf();
    void delGood_points();
    void delRegistration();

    void sendSingal_registration();
signals:
    void registrationSignal(QImage);

public slots:
};

#endif // REGISTRATION_H
