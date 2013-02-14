#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
 #include <QPainter>
#include <QDataStream>
namespace Ui {
class Widget;
}
// private final static int width = 480;
// private final static int height = 320;
const int tick = 0;
const int pwidth = 240;
const int pheight = 160;
const int numBands = 3;
const int dataLen = 57600;// 307200 OR 230400//57600 76800
const int tt = 57600;// 14400;//28800//57600;
const int imageLen = pwidth * pheight * numBands;
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

    //QDataStream socketout;
    QDataStream socketin;


    char rgbBuf[imageLen];
    QByteArray yuv420sp;

    int buffersize;
    int tick;

    void decodeYUV420SP(char* rgbBuf,char* yuv420sp, int width, int height);


};

#endif // WIDGET_H
