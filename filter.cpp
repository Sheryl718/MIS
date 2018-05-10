#include "filter.h"

Filter::Filter(QObject *parent) : QObject(parent)
{

}

int Filter::function(QString path, QString filter_type)
{
    src = imread(path.toStdString());
    res = Mat(src.size(), src.type());

   if ("mean filtering" == filter_type)
    {
       CustomBlur();
    }
   else if ("median filtering" == filter_type)
    {
       CustomMedianBlur();
    }
  else if ("gauss filter" == filter_type)
   {
       CustomGaussianBlur();
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


void Filter::CustomBlur()
{
    for (int i = 0; i<src.rows; ++i)
    {
        for (int j = 0; j<src.cols; ++j)
        {
            if ((i - 1 >= 0) && (j - 1) >= 0 && (i + 1)<src.rows && (j + 1)<src.cols)
            {
                res.at<Vec3b>(i, j)[0] = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i - 1, j - 1)[0] + src.at<Vec3b>(i - 1, j)[0] + src.at<Vec3b>(i, j - 1)[0] +src.at<Vec3b>(i - 1, j + 1)[0] + src.at<Vec3b>(i + 1, j - 1)[0] + src.at<Vec3b>(i + 1, j + 1)[0] + src.at<Vec3b>(i, j + 1)[0] + src.at<Vec3b>(i + 1, j)[0]) / 9;
                res.at<Vec3b>(i, j)[1] = (src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i - 1, j - 1)[1] + src.at<Vec3b>(i - 1, j)[1] + src.at<Vec3b>(i, j - 1)[1] +src.at<Vec3b>(i - 1, j + 1)[1] + src.at<Vec3b>(i + 1, j - 1)[1] + src.at<Vec3b>(i + 1, j + 1)[1] + src.at<Vec3b>(i, j + 1)[1] + src.at<Vec3b>(i + 1, j)[1]) / 9;
                res.at<Vec3b>(i, j)[2] = (src.at<Vec3b>(i, j)[2] + src.at<Vec3b>(i - 1, j - 1)[2] + src.at<Vec3b>(i - 1, j)[2] + src.at<Vec3b>(i, j - 1)[2] +src.at<Vec3b>(i - 1, j + 1)[2] + src.at<Vec3b>(i + 1, j - 1)[2] + src.at<Vec3b>(i + 1, j + 1)[2] + src.at<Vec3b>(i, j + 1)[2] + src.at<Vec3b>(i + 1, j)[2]) / 9;
            }
            else
            {
                res.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i, j)[0];
                res.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
                res.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
            }
        }
    }
}

uchar Filter::Median(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5, uchar n6, uchar n7, uchar n8, uchar n9)
{
    uchar arr[9];
    arr[0] = n1;
    arr[1] = n2;
    arr[2] = n3;
    arr[3] = n4;
    arr[4] = n5;
    arr[5] = n6;
    arr[6] = n7;
    arr[7] = n8;
    arr[8] = n9;
    sort(arr, arr+9);
    return arr[4];
}

void Filter::CustomMedianBlur()
{
    for (int i = 0; i<src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            if ((i - 1 >= 0) && (j - 1) >= 0 && (i + 1)<src.rows && (j + 1)<src.cols)
            {
                res.at<Vec3b>(i, j)[0] = Median(src.at<Vec3b>(i, j)[0], src.at<Vec3b>(i - 1, j - 1)[0], src.at<Vec3b>(i - 1, j)[0], src.at<Vec3b>(i, j - 1)[0], src.at<Vec3b>(i - 1, j + 1)[0], src.at<Vec3b>(i + 1, j - 1)[0], src.at<Vec3b>(i + 1, j + 1)[0], src.at<Vec3b>(i, j + 1)[0], src.at<Vec3b>(i + 1, j)[0]);
                res.at<Vec3b>(i, j)[1] = Median(src.at<Vec3b>(i, j)[1], src.at<Vec3b>(i - 1, j - 1)[1], src.at<Vec3b>(i - 1, j)[1], src.at<Vec3b>(i, j - 1)[1], src.at<Vec3b>(i - 1, j + 1)[1], src.at<Vec3b>(i + 1, j - 1)[1], src.at<Vec3b>(i + 1, j + 1)[1], src.at<Vec3b>(i, j + 1)[1], src.at<Vec3b>(i + 1, j)[1]);
                res.at<Vec3b>(i, j)[2] = Median(src.at<Vec3b>(i, j)[2], src.at<Vec3b>(i - 1, j - 1)[2], src.at<Vec3b>(i - 1, j)[2], src.at<Vec3b>(i, j - 1)[2], src.at<Vec3b>(i - 1, j + 1)[2], src.at<Vec3b>(i + 1, j - 1)[2], src.at<Vec3b>(i + 1, j + 1)[2], src.at<Vec3b>(i, j + 1)[2], src.at<Vec3b>(i + 1, j)[2]);
            }
            else
            {
                res.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i, j)[0];
                res.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
                res.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
            }
        }
    }
}

double ** Filter::getGuassionArray()
{
    int i, j;
    double sum = 0.0;
    int center = size/2;
    double **arr = new double*[size];
    for (i = 0; i < size; ++i)
    {
        arr[i] = new double[size];
    }
    for (i = 0; i < size; ++i)
    {
        for (j = 0; j < size; ++j)
        {
            arr[i][j] = exp(-((i-center)*(i-center) + (j-center)*(j-center)) / (sigma*sigma * 2));
            sum += arr[i][j];
        }
    }
    for (i = 0; i < size; ++i)
    {
        for (j = 0; j < size; ++j)
        {
            arr[i][j] /= sum;
        }
    }
    return arr;
}

void Filter::CustomGaussianBlur()
{
    double **arr;
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            if ((i - size/2) > 0 && (i + size/2) < src.rows && (j - size/2) > 0 && (j + size/2) < src.cols)
            {
                arr = getGuassionArray();
                res.at<Vec3b>(i, j)[0] = 0;
                res.at<Vec3b>(i, j)[1] = 0;
                res.at<Vec3b>(i, j)[2] = 0;
                for (int x = 0; x < size; ++x)
                {
                    for (int y = 0; y < size; ++y)
                    {
                            res.at<Vec3b>(i, j)[0] += arr[x][y] * src.at<Vec3b>(i + 1 - x, j + 1 - y)[0];
                            res.at<Vec3b>(i, j)[1] += arr[x][y] * src.at<Vec3b>(i + 1 - x, j + 1 - y)[1];
                            res.at<Vec3b>(i, j)[2] += arr[x][y] * src.at<Vec3b>(i + 1 - x, j + 1 - y)[2];
                    }
                }
            }
            else
            {
                res.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i, j)[0];
                res.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
                res.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
            }
        }
    }
    double sum = 0;
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            cout << arr[i][j] << ' ';
            sum += arr[i][j];
        }
        cout << endl;
    }
    cout << endl;
  //  cout << "sum  " << sum << endl;;
}

void Filter::getSigma(double arg)
{
    sigma = arg;
}
void Filter::getKisze(int arg)
{
    size = arg;
}

void Filter::sendSingal_filter()
{
    emit filterSignal(res1);
}
