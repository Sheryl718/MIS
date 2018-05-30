#include "sgmentation.h"
#include <qdebug.h>

Sgmentation::Sgmentation(QObject *parent) : QObject(parent)
{

}

int Sgmentation::function(QString path, QString segmentation_type)
{
    src = imread(path.toStdString());

    element_size = 3;
    element_max_size = 21;
    structringElement = getStructuringElement(MORPH_RECT, Size(element_size, element_size), Point(-1, -1));

    if ("BGR2GRAY" == segmentation_type)
    {
        delBGR2GRAY();
    }
    else if("mask" == segmentation_type)
    {
        delMask();
    }
    else if ("customImproveBC" == segmentation_type)
    {
        delCustomImprove();
    }
    else if ("erode" == segmentation_type)
    {
        delErode();
    }
    else if ("dilate" == segmentation_type)
    {
        delDilate();
    }
    else if ("open" == segmentation_type)
    {
        delOpen();
    }
    else if ("close" == segmentation_type)
    {
        delClose();
    }
    else if ("morphological gradient" == segmentation_type)
    {
        delMorGradient();
    }
    else if ("top hat" == segmentation_type)
    {
        delTopHat();
    }
    else if ("black hat" == segmentation_type)
    {
        delBlackHat();
    }
    else if ("pyrDown" == segmentation_type)
    {
        delpyrDown();
    }
    else if (("robert_x" == segmentation_type) || ("robert_y" == segmentation_type))
    {
        delRobert(segmentation_type);
    }
    else if (("sobel_x" == segmentation_type) || ("sobel_y" == segmentation_type))
    {
        delSobel(segmentation_type);
    }
    else if ("laplace" == segmentation_type)
    {
        delLaplace();
    }
    else if ("customConvolution" == segmentation_type)
    {
        delCustomConvolution();
    }
    else if ("canny" == segmentation_type)
    {
        delCanny();
    }
    else if (("edge_sobel_x" == segmentation_type) || ("edge_sobel_y" == segmentation_type))
    {
        delEdge_sobel(segmentation_type);
    }
    else if (("scharr_x" == segmentation_type) || ("scharr_y" == segmentation_type))
    {
        delScharr(segmentation_type);
    }
    else if ("edge_laplace" == segmentation_type)
    {
        delEdge_laplace();
    }
    else if ("contours" == segmentation_type)
    {
        delContours();
    }
    else if ("convertScaleAbs" == segmentation_type)
    {
        delConvertScaleAbs();
    }
    else if ("white2black" == segmentation_type)
    {
        delPixelsChange(segmentation_type);
    }
    else if ("distanceTransform" == segmentation_type)
    {
        delDistanceTransform();
    }
    else if ("normalize" == segmentation_type)
    {
        delNormalize();
    }
    else if ("watershed" == segmentation_type)
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

void Sgmentation::delBGR2GRAY()
{
    if (pixelsChange.data)
    {
        src = pixelsChange;
    }
    cvtColor(src, res, CV_BGR2GRAY);
    resultImg = res;
}

void Sgmentation::delMask()
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
void Sgmentation::delCustomImprove()
{
    res = Mat::zeros(src.size(), src.type());

    alpha = 2;
    beta = 3;

    src.convertTo(temp1, CV_32F);

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

void Sgmentation::delErode()
{
    if (distImg.data)
    {
        k1 = Mat::ones(3, 3, CV_8UC1);
        erode(distImg, distImg, k1, Point(-1, -1));
    }
    else
    {
        erode(src, res, structringElement);
        cvtColor(res, res, CV_BGR2RGB);
    }
}
void Sgmentation::delDilate()
{
    dilate(src, res, structringElement, Point(-1, -1));
    cvtColor(res, res, CV_BGR2RGB);
}
void Sgmentation::delOpen()
{
    morphologyEx(src, res, CV_MOP_OPEN, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}
void Sgmentation::delClose()
{
    morphologyEx(src, res, CV_MOP_CLOSE, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}
void Sgmentation::delMorGradient()
{
    morphologyEx(src, res, CV_MOP_GRADIENT, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
    res1 = QImage(res.data, res.cols, res.rows, QImage::Format_RGB888);
}
void Sgmentation::delTopHat()
{
    morphologyEx(src, res, CV_MOP_TOPHAT, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}
void Sgmentation::delBlackHat()
{
    morphologyEx(src, res, CV_MOP_BLACKHAT, structringElement);
    cvtColor(res, res, CV_BGR2RGB);
}

void Sgmentation::delpyrDown()
{
    pyrDown(src, res, Size(src.cols/2, src.rows/2));
    cvtColor(res, res, CV_BGR2RGB);
}
void Sgmentation::delpyrUp()
{
    pyrUp(src, res, Size(src.cols*2, src.rows*2));
    cvtColor(res, res, CV_BGR2RGB);
}

void Sgmentation::delRobert(QString segmentation_type)
{
    if ("robert_x" == segmentation_type)
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
void Sgmentation::delSobel(QString segmentation_type)
{
    if ("sobel_x" == segmentation_type)
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
void Sgmentation::delLaplace()
{
    kernel_x = (Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    filter2D(src, res, -1, kernel_x, Point(-1, -1), 0.0);
}
void Sgmentation::delCustomConvolution()
{
    kernel_x = Mat::ones(Size(3, 3), CV_32F) / (float)(3 * 3);
    filter2D(src, res, -1, kernel_x, Point(-1, -1));
}

void Sgmentation::delCanny()
{
    cvtColor(src, temp1, CV_BGR2GRAY);
    blur(temp1, temp1, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
    Canny(temp1, res, threshold_value, threshold_value * 2, 3, false);
}

void Sgmentation::delEdge_sobel(QString segmentation_type)
{
    if ("edge_sobel_x" == segmentation_type)
    {
        Sobel(src, res, CV_8U, 1, 0, 3);
    }
    else if ("edge_sobel_y" == segmentation_type)
    {
        Sobel(src, res, CV_8U, 0, 1, 3);
    }
}
void Sgmentation::delScharr(QString segmentation_type)
{
    if ("scharr_x" == segmentation_type)
    {
        Scharr(src, res, CV_8U, 1, 0, 3);
    }
    else if ("scharr_y" == segmentation_type)
    {
        Scharr(src, res, CV_8U, 0, 1, 3);
    }
}
void Sgmentation::delEdge_laplace()
{
    Laplacian(src, res, CV_8U, 3);
}

void Sgmentation::delContours()
{
    if (distImg.data)
    {
        distImg.convertTo(dist_8u, CV_8U);

        findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

        markers = Mat::zeros(src.size(), CV_32SC1);
        for (size_t i = 0; i < contours.size(); i++)
        {
            drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1);
        }
        circle(markers, Point(5, 5), 3, Scalar(255, 255, 255), -1);
        res = markers;
    }
    else
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
}

void Sgmentation::delConvertScaleAbs()
{
    convertScaleAbs(src, res);
}

void Sgmentation::delPixelsChange(QString segmentation_type)
{
    if ("white2black" == segmentation_type)
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
    }
    pixelsChange = src;
    res = src;
}

void Sgmentation::delDistanceTransform()
{
    if (binaryImg.data)
    {
        src = binaryImg;
    }
    distanceTransform(src, res, CV_DIST_L2, 3, 5);
    distImg = res;
}

void Sgmentation::delNormalize()
{
    if (distImg.data)
    {
        src = Mat(distImg.size(), distImg.type());
        src = distImg;
    }
    normalize(src, res, 0, 1, NORM_MINMAX);
    threshold(res, res, 0.4, 1, THRESH_BINARY);
    distImg = res;
}

void Sgmentation::delWatershed()
{
    watershed(src, markers);

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
    if (resultImg.data)
    {
        src = resultImg;
    }
    res = Mat(src.size(), src.type());
    width = src.cols * src.channels();
    height = src.rows;

    if ("threshold binary" == threshold_type)
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
    else if("threshold binary Inverted" == threshold_type)
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
    else if("truncate" == threshold_type)
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
    else if ("threshold to zero" == threshold_type)
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
    else if ("threshold to zero Inverted" == threshold_type)
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
    binaryImg = res;
}

void Sgmentation::getThres_value(int arg)
{
    threshold_value = arg;
}

void Sgmentation::sendSingal_sgmentation()
{
    emit sgmentationSignal(res1);
}

