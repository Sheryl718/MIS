#include "registration.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

Registration::Registration(QObject *parent) : QObject(parent)
{

}

int Registration::function(QString path1, QString path2, QString registration_type)
{
    src1 = imread(path1.toStdString(), IMREAD_GRAYSCALE);
    src2 = imread(path2.toStdString(), IMREAD_GRAYSCALE);
    if ("surf" == registration_type)
    {
        delSurf();
    }
    else if ("sift" == registration_type)
    {
        delSift();
    }
    else if ("kaze" == registration_type)
    {
        delKaze();
    }
    else if ("akaze" == registration_type)
    {
        delAkaze();
    }
//    else if ("lbp" == registration_type)
//    {
//        delLbp();
//    }
    else if ("brisk" == registration_type)
    {
        delBrisk();
    }
    else if ("flann" == registration_type)
    {
        delFlann();
    }
    else if ("brute force" == registration_type)
    {
        delBf();
    }
    else if ("good points" == registration_type)
    {
        delGood_points();
    }
    else if ("registration" == registration_type)
    {
        delRegistration();
    }

    cvtColor(res, res, CV_BGR2RGB);
    qDebug() << "res.depth()  "  << res.type() << "res.channels()" << res.channels();
    res1 = QImage(res.data, res.cols, res.rows, QImage::Format_RGB888);
    return 0;
}

void Registration::delSurf()
{
    minHessian = 100;
    surf_detector = SURF::create(minHessian);
    surf_detector->detectAndCompute(src1, Mat(), keypoints_src1, descriptor_src1);
    surf_detector->detectAndCompute(src2, Mat(), keypoints_src2, descriptor_src2);
    drawKeypoints(src1, keypoints_src1, res, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
}
void Registration::delSift()
{
    numFeatures = 400;
    sift_detector = SIFT::create(numFeatures);
    sift_detector->detectAndCompute(src1, Mat(), keypoints_src1, descriptor_src1);
    sift_detector->detectAndCompute(src2, Mat(), keypoints_src2, descriptor_src2);
    drawKeypoints(src1, keypoints_src1, res, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
}
void Registration::delAkaze()
{
    akaze_detector = AKAZE::create();
    akaze_detector->detectAndCompute(src1, Mat(), keypoints_src1, descriptor_src1);
    akaze_detector->detectAndCompute(src2, Mat(), keypoints_src2, descriptor_src2);
    drawKeypoints(src1, keypoints_src1, res, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
}
void Registration::delKaze()
{
    kaze_detector = KAZE::create();
    kaze_detector->detectAndCompute(src1, Mat(), keypoints_src1, descriptor_src1);
    kaze_detector->detectAndCompute(src2, Mat(), keypoints_src2, descriptor_src2);
    drawKeypoints(src1, keypoints_src1, res, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
}
void Registration::delBrisk()
{
    brisk_detector = BRISK::create();
    brisk_detector->detectAndCompute(src1, Mat(), keypoints_src1, descriptor_src1);
    brisk_detector->detectAndCompute(src2, Mat(), keypoints_src2, descriptor_src2);
    drawKeypoints(src1, keypoints_src1, res, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
}
void Registration::delFlann()
{
    flann_matcher.match(descriptor_src1, descriptor_src2, matches);
    drawMatches(src1, keypoints_src1, src2, keypoints_src2, matches, res);
}
void Registration::delBf()
{
    bf_matcher = BFMatcher(NORM_L2);
    bf_matcher.match(descriptor_src1, descriptor_src2, matches);
    drawMatches(src1, keypoints_src1, src2, keypoints_src2, matches, res);
}
void Registration::delGood_points()
{
    minDist = 1000;
    maxDist = 0;
    for (int i = 0; i < descriptor_src1.rows; i++)
    {
        dist = matches[i].distance;
        if (dist > maxDist)
        {
            maxDist = dist;
        }
        if (dist < minDist)
        {
            minDist = dist;
        }
    }
    count = 0;
    for (int i = 0; i < descriptor_src1.rows; i++)
    {
        dist = matches[i].distance;
        if (dist < max(3 * minDist, 0.02))
        {
            count += 1;
            goodMatches.push_back(matches[i]);
            goodkeypoints_src1.push_back(keypoints_src1[matches[i].queryIdx]);
            goodkeypoints_src2.push_back(keypoints_src2[matches[i].trainIdx]);
        }
    }
    drawMatches(src1, keypoints_src1, src2, keypoints_src2, goodMatches, res, Scalar::all(-1),
        Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS
    );
}
void Registration::delRegistration()
{
    for (int i = 0; i < count; i++)
    {
        tmpPoint.x = goodkeypoints_src1[i].pt.x;
        tmpPoint.y = goodkeypoints_src1[i].pt.y;
        forTrans_src1.push_back(tmpPoint);

        tmpPoint.x = goodkeypoints_src2[i].pt.x;
        tmpPoint.y = goodkeypoints_src2[i].pt.y;
        forTrans_src2.push_back(tmpPoint);
    }
    matrix = estimateRigidTransform(forTrans_src1, forTrans_src2, true);

    res = Mat(src1.size(), src1.type());
    warpAffine(src1, res, matrix, Size(src1.size()));

    cvtColor(res, res, CV_GRAY2BGR);

}

void Registration::sendSingal_registration()
{
    emit registrationSignal(res1);
}
