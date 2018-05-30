#ifndef MIS_H
#define MIS_H

#include <QMainWindow>
#include "filter.h"
#include "fusion.h"
#include "sgmentation.h"
#include "registration.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

namespace Ui {
class MIS;
}

class MIS : public QMainWindow
{
    Q_OBJECT

public:
    explicit MIS(QWidget *parent = 0);
    ~MIS();

    Mat save;
    QImage qres;
    QString path1 = "", path2 = "", pathSave;
    QString save_type;
    QString filter_type;
    QString fusion_type;
    QString registration_type;
    QString segmentation_type;
    QAction *action = NULL;
    QAction *action_valueChange = NULL;
    QMenu *menu;

    Filter fi;
    Fusion fu;
    Sgmentation se;
    Registration re;

    void delThreshold_spin();
    void delAlpha_spin();
    void delGamma_spin();
    void delKsize_spin();
    void delSigma_spin();
    void delSaveButton();
    int delOpen();
    int delSaveFile();
    void delMIS_BOX();
    void loadResult(QImage);
    int delFilter();
    int delFusion();
    int delSgmentation();
    int delRegistration();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_cleanButton_clicked();

    void on_changeButton_clicked();

private:
    Ui::MIS *ui;
};

#endif // MIS_H
