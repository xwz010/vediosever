#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    buffersize(0),
    tick(0)
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any,52000))
    {
        //监听本地主机的52000端口，如果出错就输出错误信息，并关闭
        qDebug() << "tcpServer listen failed."<< tcpServer->errorString();
        exit(0);
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
    qDebug()<<"listen to port 52000.";
}



void Widget::newConnection()
{
    qDebug()<<"\nclient connect.";
    m_tcpSocket = tcpServer->nextPendingConnection(); //得到每个连进来的socket
    connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage())); //有可读的信息，触发读函数槽
    socketin.setDevice(m_tcpSocket);
}

void Widget::readMessage()
{
    qDebug()<<"get message from client.";
    QByteArray qba = m_tcpSocket->readAll();
    bytebuffer.append(qba);
    buffersize += qba.size();
    qDebug()<<buffersize;

    if(buffersize >= 57600)
    {
        //qDebug()<<"buffer read ok.";
        memset(rgbBuf,0,imageLen);
        decodeYUV420SP(rgbBuf,bytebuffer.data(),pwidth,pheight);
        //qDebug()<<"decode ok.";

        QImage image = QImage((uchar*)rgbBuf,pwidth,pheight,QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(image));
        ui->label->update();

        qDebug()<<"display tick:"<<tick;
        tick ++;

        bytebuffer.resize(0);
        buffersize = 0;
        socketin<<(quint8)1;
    }
}

void Widget::decodeYUV420SP(char* rgbBuf,char* yuv420sp, int width, int height)
{
    int frameSize = width * height;
    //        if (rgbBuf == null)
    //          throw new NullPointerException("buffer 'rgbBuf' is null");
    //      if (rgbBuf.size() < frameSize * 3)
    //          throw new IllegalArgumentException("buffer 'rgbBuf' size "
    //                  + rgbBuf.length + " < minimum " + frameSize * 3);

    //      if (yuv420sp == null)
    //          throw new NullPointerException("buffer 'yuv420sp' is null");

    //      if (yuv420sp.size() < frameSize * 3 / 2)
    //          throw new IllegalArgumentException("buffer 'yuv420sp' size " + yuv420sp.length
    //                  + " < minimum " + frameSize * 3 / 2);

    int i = 0, y = 0;
    int uvp = 0, u = 0, v = 0;
    int y1192 = 0, r = 0, g = 0, b = 0;

    for (int j = 0, yp = 0; j < height; j++)
    {
        uvp = frameSize + (j >> 1) * width;
        u = 0;
        v = 0;
        for (i = 0; i < width; i++, yp++)
        {
            y = (0xff & ((int) yuv420sp[yp])) - 16;
            if (y < 0) y = 0;
            if ((i & 1) == 0) {
                v = (0xff & yuv420sp[uvp++]) - 128;
                u = (0xff & yuv420sp[uvp++]) - 128;
            }

            y1192 = 1192 * y;
            r = (y1192 + 1634 * v);
            g = (y1192 - 833 * v - 400 * u);
            b = (y1192 + 2066 * u);

            if (r < 0) r = 0; else if (r > 262143) r = 262143;
            if (g < 0) g = 0; else if (g > 262143) g = 262143;
            if (b < 0) b = 0; else if (b > 262143) b = 262143;

            rgbBuf[yp * 3] = (r >> 10);
            rgbBuf[yp * 3 + 1] = (g >> 10);
            rgbBuf[yp * 3 + 2] = (b >> 10);
        }
    }
}

Widget::~Widget()
{
    delete ui;
}

