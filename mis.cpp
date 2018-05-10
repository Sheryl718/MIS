#include "mis.h"
#include "ui_mis.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDockWidget>
#include <QDateTime>
#include "fusion.h"
#include "preprocessing.h"
#include "Sgmentation.h"
#include "registration.h"
#include "filter.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

MIS::MIS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MIS)
{
    ui->setupUi(this);

    this->setStyleSheet(
                                     "* {"
                                          "color:white;"
                                         "background-color:rgb(40, 40, 40);"
                                           "font: 14pt Comic Sans MS;"
                                       "}"
                                         "QPlainTextEdit, QScrollArea{"
                                         "font:12pt Comic Sans MS rgb(255, 48, 86);"
                                          "border:1px solid rgb(40, 40, 40);"
                                         "}"
                                        "QPushButton:hover, QMenu::item:selected, QMenuBar::item:selected {"
                                            "background-color:rgb(255, 48, 86);"
                                        "}"
                                        "QPushButton:pressed, QMenu::item:pressed, QMenuBar::item:pressed {"
                                            "background-color:rgb(255, 200, 200);"
                                        "}"
                                        "QLabel#result, #resource1, #resource2 {"
                                          "background-color:rgb(0, 0, 0);"
                                         "}"
                                          "QLabel#label_edit{"
                                         "background-color:rgb(37, 37, 37);"
                                          "border:2px solid rgb(35, 35, 35);"
                                         "}"
                                        "QSlider::handle:horizontal, QSpinBox, QDoubleSpinBox {"
                                            "border: 1px solid #5c5c5c; "
                                            "}"
                                       "QSlider::handle:horizontal:hover, QLabel#resource1:hover, QLabel#resource2:hover, QLabel#result:hover, QPlainTextEdit:hover  {"
                                            "border: 1px solid rgb(255, 48, 86);"
                                            "}"
                                        "QLabel:hover, QSpinBox:hover,QDoubleSpinBox:hover, QDockWidget::QWidegt:hover {"
                                            "color:rgb(255, 48, 86);"
                                            "}"
                                     );

    ui->MIS_BOX->hide();

    resize(730, 650);

    connect(ui->saveButton, &QPushButton::clicked,this, &MIS::delSaveButton);

    connect(ui->threshold_spin, static_cast<void (QSpinBox::*)(int) > (&QSpinBox::valueChanged),
            this,
            &MIS::delThreshold_spin
            );
    connect(ui->alpha_spin, static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
            this,
            &MIS::delAlpha_spin
            );
    connect(ui->gamma_spin, static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
            this,
            &MIS::delGamma_spin
            );
    connect(ui->ksize_spin, static_cast<void (QSpinBox::*)(int) > (&QSpinBox::valueChanged),
            this,
            &MIS::delKsize_spin
            );
    connect(ui->sigma_spin, static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
            this,
            &MIS::delSigma_spin
            );

    connect(ui->ksize_spin, static_cast<void (QSpinBox::*)(int) > (&QSpinBox::valueChanged),
           ui->horizontalSlider,
           &QSlider::setValue
           );
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->ksize_spin, &QSpinBox::setValue);

    connect(ui->gamma_spin, static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
           ui->horizontalSlider_2,
           &QSlider::setValue
           );
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, ui->gamma_spin, &QDoubleSpinBox::setValue);

    connect(ui->threshold_spin, static_cast<void (QSpinBox::*)(int) > (&QSpinBox::valueChanged),
          ui->horizontalSlider_3,
          &QSlider::setValue
          );
    connect(ui->horizontalSlider_3, &QSlider::valueChanged, ui->threshold_spin, &QSpinBox::setValue);

    connect(ui->actionOpen, &QAction::triggered, this, &MIS::delOpen);

    connect(ui->actionsrc1, &QAction::triggered, this, &MIS::delSaveFile);
    connect(ui->actionsrc2, &QAction::triggered, this, &MIS::delSaveFile);
    connect(ui->actionres, &QAction::triggered, this, &MIS::delSaveFile);

    connect(ui->actionMIS_BOX, &QAction::triggered, this, &MIS::delMIS_BOX);

    connect(ui->actionabout, &QAction::triggered,
                [=]()
                {
                    QMessageBox::about(this, "about", "This is my graduation design!\nWelcome to use it!");
                }
                );

    connect(ui->actionBGR2GRAY, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionmask_, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actioncustomImprove, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionerode, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actiondialate, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionopen, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionclose, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionmorphological_gradient, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actiontop_hat, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionblack_hat, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionpyrDown, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionpyrUp, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionrobert_x, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionrobert_y, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionsobel_x, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionsobel_y, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionlaplace, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionCustomConvolution, &QAction::triggered, this, &MIS::delProcessing);    
    connect(ui->actioncanny, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionedge_sobel_x, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionedge_sobel_y, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionscharr_x, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionscharr_y, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionedge_laplace, &QAction::triggered, this, &MIS::delProcessing);

    connect(ui->actioncontours, &QAction::triggered, this, &MIS::delProcessing);
    connect(ui->actionconvertScaleAbs, &QAction::triggered, this, &MIS::delProcessing);


    connect(ui->actionmedian_filtering, &QAction::triggered, this, &MIS::delFilter);
    connect(ui->actionmean_filtering, &QAction::triggered, this, &MIS::delFilter);
    connect(ui->actiongauss_filter, &QAction::triggered, this, &MIS::delFilter);

    connect(ui->actionaddWeighted, &QAction::triggered, this, &MIS::delFusion);
    connect(ui->actionmultiply, &QAction::triggered,this, &MIS::delFusion);
    connect(ui->actionadd_2, &QAction::triggered,this, &MIS::delFusion);

    connect(ui->actionsurf, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionsift, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionlbp, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionkaze, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionakaze, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionbrisk, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionflann, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionbf, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actiongood_points, &QAction::triggered, this, &MIS::delRegistration);
    connect(ui->actionregistration, &QAction::triggered, this, &MIS::delRegistration);

    connect(ui->actionthreshold_binary, &QAction::triggered, this, &MIS::delSgmentation);
    connect(ui->actionthreshold_binary_Inverted, &QAction::triggered, this, &MIS::delSgmentation);
    connect(ui->actiontruncate, &QAction::triggered, this, &MIS::delSgmentation);
    connect(ui->actionthreshold_to_zero, &QAction::triggered, this, &MIS::delSgmentation);
    connect(ui->actionthreshold_to_zero_inverted, &QAction::triggered, this, &MIS::delSgmentation);
    connect(ui->actionwatershed, &QAction::triggered, this, &MIS::delSgmentation);
}

MIS::~MIS()
{
    delete ui;
}

void MIS::delThreshold_spin()
{
    action_valueChange = action;
    void (QSpinBox::*setThreshold_value)(int) = &QSpinBox::valueChanged;
    if ("canny" == action_valueChange->text() || "contours" == action_valueChange->text())
    {
        void (Preprocessing::*getThreshold_value)(int) = &Preprocessing::getThres_value;
        connect(ui->threshold_spin, setThreshold_value, &pr, getThreshold_value);
        delProcessing();
    }
    else
    {
        void (Sgmentation::*getSe_threshold_value)(int) = &Sgmentation::getThres_value;
        connect(ui->threshold_spin, setThreshold_value, &se, getSe_threshold_value);
        delSgmentation();
    }
}
void MIS::delAlpha_spin()
{
    void (Fusion::*getal)(double) = &Fusion::getAlpha;
    void (QDoubleSpinBox::*setal)(double) = &QDoubleSpinBox::valueChanged;
    connect(ui->alpha_spin, setal, &fu, getal);
    action_valueChange = action;
    delFusion();
}
void MIS::delGamma_spin()
{
    void (Fusion::*getga)(double) = &Fusion::getGamma;
    void (QDoubleSpinBox::*setga)(double) = &QDoubleSpinBox::valueChanged;
    connect(ui->gamma_spin, setga, &fu, getga);
    action_valueChange = action;
    delFusion();
}
void MIS::delKsize_spin()
{
    void (Filter::*getks)(int) = &Filter::getKisze;
    void (QSpinBox::*setks)(int) = &QSpinBox::valueChanged;
    connect(ui->ksize_spin, setks, &fi, getks);
    action_valueChange = action;
    delFilter();
}
void MIS::delSigma_spin()
{
    void (Filter::*getsi)(double) = &Filter::getSigma;
    void (QDoubleSpinBox::*setsi)(double) = &QDoubleSpinBox::valueChanged;
    connect(ui->sigma_spin, setsi, &fi, getsi);
    action_valueChange = action;
    delFilter();
}
void MIS::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(this, "close", "Are you sure close the window?");
    if (ret == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
void MIS::loadResult(QImage res1)
{
    ui->result->setPixmap(QPixmap::fromImage(res1));
    ui->application_output->appendPlainText("loadResult: height:" + QString::number(res1.height(), 10) + ",width:" + QString::number(res1.width(), 10) +  ",depth:"+ QString::number(res1.depth(), 10) + ";  " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
    qres = res1;
}

void MIS::delSaveButton()
{
    if (24 == qres.format())
    {
        save = Mat(qres.height(), qres.width(), CV_8UC1, (void*)qres.constBits(), qres.bytesPerLine());
    }
    else
    {
        save = Mat(qres.height(), qres.width(), CV_8UC3, (void*)qres.constBits(), qres.bytesPerLine());
        cvtColor(save, save, CV_BGR2RGB);
    }
    imwrite(path1.toStdString(), save);
    ui->application_output->appendPlainText(path1+ ";\nheight:" + QString::number(qres.height(), 10) + ",width:" + QString::number(qres.width(), 10) +  ",depth:"+ QString::number(qres.depth(), 10) + ";  " +QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
}
void MIS::on_cleanButton_clicked()
{
    path1 = "";
    path2 = "";
    qres = QImage();
    ui->resource1->setPixmap(path1);
    ui->resource2->setPixmap(path2);
    ui->result->setPixmap(path1);
    ui->application_output->appendPlainText("\n");
}
void MIS::on_changeButton_clicked()
{
    action_valueChange = NULL;
}

int MIS::delOpen()
{
    if(path1 == "")
    {
        path1 = QFileDialog::getOpenFileName(this, "open", "./", "Image(*.png *.jpg *.tif);;all(*.*)" );
        ui->resource1->setPixmap(QPixmap(path1));
        if (path1 != "")
        ui->application_output->appendPlainText(path1+ ";\nheight:" + QString::number(QPixmap(path1).height(), 10) + ",width:" + QString::number(QPixmap(path1).width(), 10) +  ",depth:"+ QString::number(QPixmap(path1).depth(), 10) + ";  " +QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
    }
    else
    {
        path2 = QFileDialog::getOpenFileName(this, "open", "./", "Image(*.png *.jpg *.tif);;all(*.*)" );
        ui->resource2->setPixmap(QPixmap(path2));
        if (path2 != "")
        ui->application_output->appendPlainText(path2+ ";\nheight:" + QString::number(QPixmap(path2).height(), 10) + ",width:" + QString::number(QPixmap(path2).width(), 10) +  ",depth:"+ QString::number(QPixmap(path2).depth(), 10) + ";  " +QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
    }
    return 0;
}
int MIS::delSaveFile()
{
    action = (QAction *)sender();
    save_type = action->text();
    if ("resource1" == save_type)
    {
        if("" == path1)
        {
            QMessageBox::warning(this, "error", "Save failure..!");
            ui->application_output->appendPlainText("Save failure..!");
            return -1;
        }
        pathSave = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                           "",
                                                           tr("Images (*.png *.tif *.jpg)"));
        if (!pathSave.isNull())
        {
            imwrite(pathSave.toStdString(), imread(path1.toStdString()));
            ui->application_output->appendPlainText(pathSave + ";\nheight:" + QString::number(QPixmap(path1).height(), 10) + ",width:" + QString::number(QPixmap(path1).width(), 10) +  ",depth:"+ QString::number(QPixmap(path1).depth(), 10) + ";  " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
        }
    }
    else if("resource2" == save_type)
    {
        if("" == path2)
        {
            QMessageBox::warning(this, "error", "Save failure..!");
            ui->application_output->appendPlainText("Save failure..!");
            return -1;
        }
        pathSave = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                           "",
                                                           tr("Images (*.png *.tif *.jpg)"));
         if (!pathSave.isNull())
        {
             imwrite(pathSave.toStdString(), imread(path2.toStdString()));
             ui->application_output->appendPlainText(pathSave+ ";\nheight:" + QString::number(QPixmap(path2).height(), 10) + ",width:" + QString::number(QPixmap(path2).width(), 10) +  ",depth:"+ QString::number(QPixmap(path2).depth(), 10) + ";  " +QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
        }
    }
    else if("result" == save_type)
    {
        if(qres.isNull())
        {
            QMessageBox::warning(this, "error", "Save failure..!");
            ui->application_output->appendPlainText("Save failure..!");
            return -1;
        }
        pathSave = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                           "",
                                                           tr("Images (*.png *.tif *.jpg)"));
        if (24 == qres.format())
        {
            save = Mat(qres.height(), qres.width(), CV_8UC1, (void*)qres.constBits(), qres.bytesPerLine());
        }
        else
        {
            save = Mat(qres.height(), qres.width(), CV_8UC3, (void*)qres.constBits(), qres.bytesPerLine());
            cvtColor(save, save, CV_BGR2RGB);
        }
        if (!pathSave.isNull())
        {
             imwrite(pathSave.toStdString(), save);
             ui->application_output->appendPlainText(pathSave+ ";\nheight:" + QString::number(qres.height(), 10) + ",width:" + QString::number(qres.width(), 10) +  ",depth:"+ QString::number(qres.depth(), 10) + ";  " +QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
        }
    }

    return 0;
}
void MIS::delMIS_BOX()
{
    ui->MIS_BOX->show();
}

int MIS::delProcessing()
{
    if (path1 == "")
    {
       QMessageBox::warning(this, "error", "please input a picture...");
       return -1;
    }
    if (NULL != action_valueChange)
    {
        action = action_valueChange;
    }
    else
    {
        action = (QAction *)sender();
    }
    qDebug() << action->text();
    preprocessing_type = action->text();
    pr.function(path1, preprocessing_type);
    void (Preprocessing::*m_sig)(QImage) = &Preprocessing::preprocessingSignal;
    void (MIS::*m_slo)(QImage) = &MIS::loadResult;
    connect(&pr, m_sig, this, m_slo);
    pr.sendSingal_preprocessing();
    return 0;
}

int MIS::delFilter()
{
    if (path1 == "")
    {
       QMessageBox::warning(this, "error", "please input a picture...");
       return -1;
    }
    if (NULL != action_valueChange)
    {
        action = action_valueChange;
    }
    else
    {
        action = (QAction *)sender();
    }
    filter_type = action->text();

    fi.function(path1, filter_type);
    void (Filter::*fi_sig)(QImage) = &Filter::filterSignal;
    void (MIS::*m_slo)(QImage) = &MIS::loadResult;
    connect(&fi, fi_sig, this, m_slo);
    fi.sendSingal_filter();
    return 0;
}

int MIS::delFusion()
{
    if (path1 == "" || path2 == "" || path1 == path2)
    {
       QMessageBox::warning(this, "error", "please input two different pictures...");
       return -1;
    }
    if (NULL != action_valueChange)
    {
        action = action_valueChange;
    }
    else
    {
        action = (QAction *)sender();
    }
    fusion_type = action->text();
    qDebug() << fusion_type;

    fu.function(path1, path2, fusion_type);
    connect(&fu, &Fusion::errorSignal,
            [=]()
            {
                QMessageBox::warning(this, "error", "Could not blend images, the size or the type of image is not same....");
            }
            );

    void (Fusion::*f_sig)(QImage) = &Fusion::fusionSignal;
    void (MIS::*f_slo)(QImage) = &MIS::loadResult;
    connect(&fu,f_sig, this, f_slo);
    fu.sendSingal_fusion();
    return 0;
}

int MIS::delRegistration()
{
    if (path1 == " ")
    {
        QMessageBox::warning(this, "error", "please input two different pictures...");
        return -1;
    }
    action = (QAction *)sender();
    registration_type = action->text();
    re.function(path1, path2, registration_type);

    void (Registration::*r_sig)(QImage) = &Registration::registrationSignal;
    void (MIS::*s_slo)(QImage) = &MIS::loadResult;
    connect(&re, r_sig, this, s_slo);
    re.sendSingal_registration();
    return 0;
}

int MIS::delSgmentation()
{
    if (path1 == "")
    {
       QMessageBox::warning(this, "error", "please input a picture...");
       return -1;
    }

    if (NULL != action_valueChange)
    {
        action = action_valueChange;
    }
    else
    {
        action = (QAction *)sender();
    }
    if ( "threshold binary" == action->text())
    {
        segmentation_type = 0;
    }
    else if ("threshold binary Inverted" == action->text())
    {
        segmentation_type = 1;
    }
    else if ("truncate" == action->text())
    {
        segmentation_type = 2;
    }
    else if ("threshold to zero" == action->text())
    {
        segmentation_type = 3;
    }
    else if ("threshold to zero inverted" == action->text())
    {
        segmentation_type = 4;
    }
    else if ("watershed" == action->text())
    {
        segmentation_type = 5;
    }

    se.function(path1, segmentation_type);
    void (Sgmentation::*s_sig)(QImage) = &Sgmentation::sgmentationSignal;
    void (MIS::*s_slo)(QImage) = &MIS::loadResult;
    connect(&se, s_sig, this, s_slo);
    se.sendSingal_sgmentation();
    return 0;
}

