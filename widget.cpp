#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImageEncoderSettings>
#include <QPixmap>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout;
    mainLayout->addLayout(topLayout , 1);

    //构造一个QCamera
    m_camera = new QCamera(this);
    connect(m_camera , SIGNAL(locked()) , this , SLOT(onLocked()));
    connect(m_camera , SIGNAL(lockFailed()) , this , SLOT(onLockFailed()));

    //构造取景器
    m_viewFinder = new QCameraViewfinder;
    m_viewFinder->setFixedSize(480 , 320);
    //设置取景器
    m_camera->setViewfinder(m_viewFinder);
    //捕获静态图片
    m_camera->setCaptureMode(QCamera::CaptureStillImage);

    //构造一个捕获器
    m_imageCapture = new QCameraImageCapture(m_camera , this);
    QImageEncoderSettings imageSettings;
    imageSettings.setCodec("image/jpeg");
    imageSettings.setResolution(1600 , 1200);
    //设置图像编码方式
    m_imageCapture->setEncodingSettings(imageSettings);
    //设置将指定的内容保存到文件或者内存
    m_imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    connect(m_imageCapture , SIGNAL(imageCaptured(int,QImage)) , this , SLOT(onImageCaptured(int,QImage)));
    connect(m_imageCapture , SIGNAL(error(int,QCameraImageCapture::Error,QString)) , this , SLOT(onError(int,QCameraImageCapture::Error,QString)));

    topLayout->addWidget(m_viewFinder);

    m_imageLabel = new QLabel;
    m_imageLabel->setFixedSize(480 , 320);
    topLayout->addWidget(m_imageLabel);

    m_captureButton = new QPushButton("Capture");
    connect(m_captureButton , SIGNAL(clicked()) , this , SLOT(onCaptureButton()));
    mainLayout->addWidget(m_captureButton , 0 , Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addStretch(1);
    m_camera->start();
}

Widget::~Widget()
{

}


void Widget::onCaptureButton()
{
    qDebug()<<"onCaptureButton"<<m_camera->supportedLocks();
   // m_camera->searchAndLock();
    //onLocked();
}

void Widget::onImageCaptured(int id , const QImage &preview)
{
    qDebug()<<"onImageCaptured id -"<<id;
    m_imageLabel->setPixmap(QPixmap::fromImage(preview));
    m_camera->unlock();
}

void Widget::onImageSaved(int id , const QString &fileName)
{
    qDebug()<<"onImageSaved id - "<<id;
    m_imageLabel->setPixmap(QPixmap(fileName));
}

void Widget::onError(int id , QCameraImageCapture::Error error, const QString & errorString)
{
    qDebug()<<"id - "<<id<<" error - "<<error<<"onError - "<<errorString;
}

void Widget::onLocked()
{
    qDebug()<<"onLocked";
    m_imageCapture->capture("capture.jpg");
}

void Widget::onLockFailed()
{
    qDebug()<<"lock failed!";
}
