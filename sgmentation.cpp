#include "sgmentation.h"
#include <qdebug.h>

Sgmentation::Sgmentation(QObject *parent) : QObject(parent)
{

}

int Sgmentation::function(QString path, int segmentation_type)
{
    src = imread(path.toStdString());
    if (5 == segmentation_type)
    {
        delWatershed();
    }
    else
    {
       threshold_type = segmentation_type;
        delThreshold();
    }

    if (3 == res.channels())
    {
        cvtColor(res, res, CV_BGR2RGB);
        res1 = QImage(res.data, res.cols, res.rows, QImage::Format_RGB888);
    }
    else if (1 == res.channels())
    {
        res1 = QImage(res.data, res.cols, res.rows, QImage::Format_Grayscale8);
    }
    return 0;
}

void Sgmentation::delWatershed()
{
    for (int row = 0; row < src.rows; row++)
    {
        for (int col = 0; col < src.cols; col++)
        {
            if (src.at<Vec3b>(row, col) == Vec3b(255, 255, 255))
            {
                src.at<Vec3b>(row, col)[0] = 0;
                src.at<Vec3b>(row, col)[1] = 0;
                src.at<Vec3b>(row, col)[2] = 0;
            }
        }
    }

    kernel = (Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
    sharpenImg = src;
    filter2D(src, imgLaplance, CV_32F, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
    src.convertTo(sharpenImg, CV_32F);
    resultImg = sharpenImg - imgLaplance;
    resultImg.convertTo(resultImg, CV_8UC3);
    imgLaplance.convertTo(imgLaplance, CV_8UC3);

    cvtColor(src, resultImg, CV_BGR2GRAY);
    threshold(resultImg, binaryImg, 40, 255, THRESH_BINARY | THRESH_OTSU);

    distanceTransform(binaryImg, distImg, CV_DIST_L2, 3, 5);
    normalize(distImg, distImg, 0, 1, NORM_MINMAX);

    threshold(distImg, distImg, 0.4, 1, THRESH_BINARY);

    k1 = Mat::ones(3, 3, CV_8UC1); //结构元素
    erode(distImg, distImg, k1, Point(-1, -1));

    distImg.convertTo(dist_8u, CV_8U);

    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    markers = Mat::zeros(src.size(), CV_32SC1);
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1);
    }
    circle(markers, Point(5, 5), 3, Scalar(255, 255, 255), -1);

    watershed(src, markers);

    mark = Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    bitwise_not(mark, mark, Mat());

    for (size_t i = 0; i < contours.size(); i++)
    {
        int r = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int b = theRNG().uniform(0, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    res = Mat::zeros(markers.size(), CV_8UC3);
    for (int row = 0; row < markers.rows; row++)
    {
        for (int col = 0; col < markers.cols; col++)
        {
            int index = markers.at<int>(row, col);
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                res.at<Vec3b>(row, col) = colors[index - 1];
            }
            else
            {
                res.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
            }
        }
    }
}

void Sgmentation::delThreshold()
{
    res = Mat(src.size(), src.type());
    width = src.cols * src.channels();
    height = src.rows;

    if (0 == threshold_type)
    {
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
            {
                if (src.at<uchar>(row, col) > threshold_value)
                {
                    res.at<uchar>(row, col) = 255;
                }
                else
                {
                    res.at<uchar>(row, col) = 0;
                }
            }
        }
    }
    else if(1 == threshold_type)
    {
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
            {
                if (src.at<uchar>(row, col) > threshold_value)
                {
                    res.at<uchar>(row, col) = 0;
                }
                else
                {
                    res.at<uchar>(row, col) = 255;
                }
            }
        }
    }
    else if(2 == threshold_type)
    {
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
            {
                if (src.at<uchar>(row, col) > threshold_value)
                {
                    res.at<uchar>(row, col) = threshold_value;
                }
                else
                {
                     res.at<uchar>(row, col) = src.at<uchar>(row, col);
                }
            }
        }
    }
    else if (3 == threshold_type)
    {
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
            {
                if (src.at<uchar>(row, col) > threshold_value)
                {
                    res.at<uchar>(row, col) = src.at<uchar>(row, col);
                }
                else
                {
                     res.at<uchar>(row, col) = 0;
                }
            }
        }
    }
    else if (4 == threshold_type)
    {
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
            {
                if (src.at<uchar>(row, col) > threshold_value)
                {
                    res.at<uchar>(row, col) = 0;
                }
                else
                {
                     res.at<uchar>(row, col) = src.at<uchar>(row, col);
                }
            }
        }
    }
}

void Sgmentation::getThres_value(int arg)
{
    threshold_value = arg;
}

void Sgmentation::sendSingal_sgmentation()
{
    emit sgmentationSignal(res1);
}
