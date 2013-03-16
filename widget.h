#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
 #include <QPainter>
#include <QDataStream>
namespace Ui {
class Widget;
}

const int tick = 0;
const int mPreviewWidth = 480;
const int mPreviewHeight = 320;
const int numBands = 3;

const int imageLen = mPreviewWidth * mPreviewHeight * numBands;
const int dataLen = imageLen / 2 ;

const int port= 52000;

const int borderlen = 40;

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void newConnection();
    void readMessage();


private:
    Ui::Widget *ui;

    QTcpServer *tcpServer;
    QTcpSocket *m_tcpSocket;

    QImage image_24;
    QDataStream socketin;


    char rgbBuf[imageLen];
    QByteArray yuv420sp;

    int buffersize;
    int tick;

    void decodeYUV420SP(char* rgbBuf,char* yuv420sp, int width, int height);


};

#endif // WIDGET_H
