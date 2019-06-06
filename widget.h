#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPushButton>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onCaptureButton();
    void onImageCaptured(int id , const QImage &preview);
    void onImageSaved(int id , const QString &fileName);
    void onError(int id , QCameraImageCapture::Error error, const QString & errorString);
    void onLocked();
    void onLockFailed();

protected:
    QCamera *m_camera;
    QCameraViewfinder *m_viewFinder;
    QCameraImageCapture *m_imageCapture;
    QPushButton *m_captureButton;
    QLabel *m_imageLabel;
};

#endif // WIDGET_H
