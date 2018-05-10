#include "fusion.h"
#include <QDebug>
#include <math.h>

Fusion::Fusion(QObject *parent) : QObject(parent)
{

}

int Fusion::function(QString path1, QString path2, QString fusion_type)
{

    src1 = imread(path1.toStdString());
    src2 = imread(path2.toStdString());

    if (!src1.data)
    {
        printf("Could not load image src1...\n");
        return -1;
    }
    if (!src2.data)
    {
        printf("Could not load image src2...\n");
        return -1;
    }

    if (src1.cols == src2.cols && src1.rows == src2.rows && src1.type() == src2.type())
    {
        if (fusion_type == "addWeighted")
        {
            customAddWeighted();
            qDebug() << "alpha:  " << alpha;
            qDebug() << "gamma:  " << gamma;
        }
        else if(fusion_type == "multiply")
        {
           customMultiply();
        }
        else if(fusion_type == "add")
        {
            customAdd();
        }

        if (3 == res.channels())
        {
            cvtColor(res, res, CV_BGR2RGB);
            // res1 = QImage((const unsigned char *)dst.data, dst.cols, dst.rows, QImage::Format_RGB888);
            res1 = QImage(res.data, res.cols, res.rows, QImage::Format_RGB888);
        }
        else if (1 == res.channels())
        {
            res1 = QImage(res.data, res.cols, res.rows, QImage::Format_Grayscale8);
        }
    }
    else
    {
        emit errorSignal();
        return -1;
    }

    waitKey(0);

    return 0;
}

void Fusion::getAlpha(double arg)
{
    alpha = arg;
}

void Fusion::getGamma(double arg)
{
    gamma = arg;
}

void Fusion::customAdd()
{
        res = Mat(src1.size(), src1.type());
        custom_width = src1.cols * src1.channels();
        custom_height = src1.rows;

        qDebug() << "src1 type:  " << src1.type();
        for(int row = 0; row < custom_height; row++)
        {
            for(int col = 0; col < custom_width; col++)
            {
                int s1 = src1.at<uchar>(row, col);
                int s2 = src2.at<uchar>(row, col);
                int s12 = s1 + s2;
                res.at<uchar>(row, col) = saturate_cast<uchar>(s12);
            }
        }
}

void Fusion::customMultiply()
{
    res = Mat(src1.size(), src1.type());
    custom_width = src1.cols * src1.channels();
    custom_height = src1.rows;

    qDebug() << "src1 type:  " << src1.type();
    for(int row = 0; row < custom_height; row++)
    {
        for(int col = 0; col < custom_width; col++)
        {
            int s1 = src1.at<uchar>(row, col);
            int s2 = src2.at<uchar>(row, col);
            int s12 = s1 * s2;
            res.at<uchar>(row, col) = saturate_cast<uchar>(s12);
        }
    }
}

void Fusion:: customAddWeighted()
{
    res = Mat(src1.size(), src1.type());
    custom_width = src1.cols * src1.channels();
    custom_height = src1.rows;

    qDebug() << "src1 type:  " << src1.type();
    for(int row = 0; row < custom_height; row++)
    {
        for(int col = 0; col < custom_width; col++)
        {
            int s1 = src1.at<uchar>(row, col);
            int s2 = src2.at<uchar>(row, col);
            int s12 = alpha*s1 + (1.0-alpha)*s2 + gamma;
            res.at<uchar>(row, col) = saturate_cast<uchar>(s12);
        }
    }
}

void Fusion::sendSingal_fusion()
{
    emit fusionSignal(res1);
}
