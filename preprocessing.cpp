#include "preprocessing.h"
#include <QImage>

Preprocessing::Preprocessing(QObject *parent) : QObject(parent)
{

}

int Preprocessing::function(QString path, QString preprocessing_type)
{
    src = imread(path.toStdString());

    element_size = 3;
    element_max_size = 21;
    structringElement = getStructuringElement(MORPH_RECT, Size(element_size, element_size), Point(-1, -1));

    if ("BGR2GRAY" == preprocessing_type)
    {
        delBGR2GRAY();
    }
    else if("mask" == preprocessing_type)
    {
        delMask();
    }
    else if ("customImprove" == preprocessing_type)
    {
        delCustomImprove();
    }
    else if ("erode" == preprocessing_type)
    {
        delErode();
    }
    else if ("dilate" == preprocessing_type)
    {
        delDilate();
    }
    else if ("open" == preprocessing_type)
    {
        delOpen();
    }
    else if ("close" == preprocessing_type)
    {
        delClose();
    }
    else if ("morphological gradient" == preprocessing_type)
    {
        delMorGradient();
    }
    else if ("top hat" == preprocessing_type)
    {
        delTopHat();
    }
    else if ("black hat" == preprocessing_type)
    {
        delBlackHat();
    }
    else if ("pyrDown" == preprocessing_type)
    {
        delpyrDown();
    }
    else if (("robert_x" == preprocessing_type) || ("robert_y" == preprocessing_type))
    {
        delRobert(preprocessing_type);
    }
    else if (("sobel_x" == preprocessing_type) || ("sobel_y" == preprocessing_type))
    {
        delSobel(preprocessing_type);
    }
    else if ("laplace" == preprocessing_type)
    {
        delLaplace();
    }
    else if ("customConvolution" == preprocessing_type)
    {
        delCustomConvolution();
    }
    else if ("canny" == preprocessing_type)
    {
        delCanny();
    }
    else if (("edge_sobel_x" == preprocessing_type) || ("edge_sobel_y" == preprocessing_type))
    {
        delEdge_sobel(preprocessing_type);
    }
    else if (("scharr_x" == preprocessing_type) || ("scharr_y" == preprocessing_type))
    {
        delScharr(preprocessing_type);
    }
    else if ("edge_laplace" == preprocessing_type)
    {
        delEdge_laplace();
    }
    else if ("contours" == preprocessing_type)
    {
        delContours();
    }
    else if ("convertScaleAbs" == preprocessing_type)
    {
        delConvertScaleAbs();
    }


    if (3 == res.channels())
    {
        cvtColor(res, res, CV_BGR2RGB);
        res1 = QImage(res.data, res.cols, res.rows, QImage::Format_RGB888);
    }
    else if(1 == res.channels())
    {
         res1 = QImage(res.data, res.cols, res.rows, QImage::Format_Grayscale8);
    }

    return 0;
}

void Preprocessing::delBGR2GRAY()
{
    cvtColor(src, res, CV_BGR2GRAY);
}

void Preprocessing::delMask()
{
    cols = (src.cols-1) * src.channels();
    rows = src.rows;
    offsetx = src.channels();
    res = Mat::zeros(src.size(), src.type());
    for (int row = 1; row < (rows - 1); row++)
    {
        const uchar *current = src.ptr<uchar>(row);
        const uchar *previous = src.ptr<uchar>(row-1);
        const uchar *next = src.ptr<uchar>(row + 1);
        uchar* output = res.ptr<uchar>(row);
        for (int col = offsetx; col < cols; col++)
        {
            output[col] = saturate_cast<uchar>(5 * current[col] - (current[col - offsetx] + current[col + offsetx] + previous[col] + next[col]));
        }
    }
}
void Preprocessing::delCustomImprove()
{
    res = Mat::zeros(src.size(), src.type());

    alpha = 2;
    beta = 3;

    src.convertTo(temp1, CV_32F);

   // temp1.convertTo(temp2, CV_8U);

    rows = src.rows;
    cols = src.cols;

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (src.channels() == 3)
            {
                b = temp1.at<Vec3f>(row, col)[0];
                g = temp1.at<Vec3f>(row, col)[1];
                r = temp1.at<Vec3f>(row, col)[2];

                res.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(b * alpha + beta);
                res.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(g * alpha + beta);
                res.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(r * alpha + beta);
            }
            else if (src.channels() == 1)
            {
                h = src.at<uchar>(row, col);
                res.at<uchar>(row, col) = saturate_cast<uchar>(h * alpha + beta);
            }
        }
    }
}

void Preprocessing::delErode()
{
    erode(src, res, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}
void Preprocessing::delDilate()
{
    dilate(src, res, structringElement, Point(-1, -1));
    cvtColor(res, res, CV_BGR2RGB);
}
void Preprocessing::delOpen()
{
    morphologyEx(src, res, CV_MOP_OPEN, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}
void Preprocessing::delClose()
{
    morphologyEx(src, res, CV_MOP_CLOSE, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}
void Preprocessing::delMorGradient()
{
    morphologyEx(src, res, CV_MOP_GRADIENT, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
    res1 = QImage(res.data, res.cols, res.rows, QImage::Format_RGB888);
}
void Preprocessing::delTopHat()
{
    morphologyEx(src, res, CV_MOP_TOPHAT, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}
void Preprocessing::delBlackHat()
{
    morphologyEx(src, res, CV_MOP_BLACKHAT, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}

void Preprocessing::delpyrDown()
{
    pyrDown(src, res, Size(src.cols/2, src.rows/2));
    cvtColor(res, res, CV_BGR2RGB);
}
void Preprocessing::delpyrUp()
{
    pyrUp(src, res, Size(src.cols*2, src.rows*2));
    cvtColor(res, res, CV_BGR2RGB);
}

void Preprocessing::delRobert(QString preprocessing_type)
{
    if ("robert_x" == preprocessing_type)
    {
        kernel_x = (Mat_<int>(2, 2) << 1, 0, 0, -1);
        filter2D(src, res, -1, kernel_x, Point(-1, -1));
    }
    else
    {
        kernel_y = (Mat_<int>(2, 2) << 0, 1, -1, 0);
        filter2D(src, res, -1, kernel_y, Point(-1, -1));
    }
}
void Preprocessing::delSobel(QString preprocessing_type)
{
    if ("sobel_x" == preprocessing_type)
    {
        kernel_x = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
        filter2D(src, res, -1, kernel_x, Point(-1, -1), 0.0);
    }
    else
    {
        kernel_y = (Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        filter2D(src, res, -1, kernel_y, Point(-1, -1), 0.0);
    }
}
void Preprocessing::delLaplace()
{
    kernel_x = (Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    filter2D(src, res, -1, kernel_x, Point(-1, -1), 0.0);
}
void Preprocessing::delCustomConvolution()
{
    kernel_x = Mat::ones(Size(3, 3), CV_32F) / (float)(3 * 3);
    filter2D(src, res, -1, kernel_x, Point(-1, -1));
}

void Preprocessing::delCanny()
{
    cvtColor(src, temp1, CV_BGR2GRAY);
    blur(temp1, temp1, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
    Canny(temp1, res, threshold_value, threshold_value * 2, 3, false);
}

void Preprocessing::delEdge_sobel(QString preprocessing_type)
{
    if ("edge_sobel_x" == preprocessing_type)
    {
        Sobel(src, res, CV_8U, 1, 0, 3);
    }
    else if ("edge_sobel_y" == preprocessing_type)
    {
        Sobel(src, res, CV_8U, 0, 1, 3);
    }
}
void Preprocessing::delScharr(QString preprocessing_type)
{
    if ("scharr_x" == preprocessing_type)
    {
        Scharr(src, res, CV_8U, 1, 0, 3);
    }
    else if ("scharr_y" == preprocessing_type)
    {
        Scharr(src, res, CV_8U, 0, 1, 3);
    }
}
void Preprocessing::delEdge_laplace()
{
    Laplacian(src, res, CV_8U, 3);
}

void Preprocessing::delContours()
{
    cvtColor(src, src, CV_BGR2GRAY);
    Canny(src, temp1, threshold_value, threshold_value * 2, 3, false);

    findContours(temp1, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    res = Mat::zeros(src.size(), CV_8UC3);
    RNG rng(12345);
    for (size_t i = 0; i < contours.size(); i++)
    {
        color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(res, contours, i, color, 1, 8, hierachy, 0, Point(0, 0));
    }
}

void Preprocessing::delConvertScaleAbs()
{
    convertScaleAbs(src, res);
}

void Preprocessing::sendSingal_preprocessing()
{
    emit preprocessingSignal(res1);
}

void Preprocessing::getThres_value(int arg)
{
    threshold_value = arg;
}
