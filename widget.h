#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtTextToSpeech/QTextToSpeech> //文本转语音
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
//#include <QImageWriter>
//调用摄像头
#include <QCameraImageCapture>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>//取景器
#include <QPixmap>
//弹窗
#include <QMessageBox>
//容器
#include <QList>
#include <QByteArray>
#include <QBuffer>
#include <QTextCodec>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QLocale>
#include <QMovie>
#include <QTimeZone>
#include "hisui.h"
#include "aksk.h"

const QString accessTokenUrl = "https://aip.baidubce.com/oauth/2.0/token";//获取tokenUrl
const QString requestUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate";//posterl


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_2_clicked();
    void imageSaved(int id, const QImage &preview);

    QByteArray imageToBase64(QImage pix);//图片类型转换

    int licensePlate(QString &jsonResult, const QString &accessToken,QByteArray p);

    int analysisJson(QJsonObject &obj,QString &licencePlate);

    void getDataInform();

    void speak(const QString &text, qreal volume, qreal rate, const QLocale &locale);

    int chargeFun(QString &dateTime);

    void saveInMysql();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();
    void setWUI();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

signals:
    void dataInform();

private:
    Ui::Widget *ui;
    //摄像头对象
    QCamera *ca;
    QCameraImageCapture * imgcapture;
    QCameraViewfinder *viewfinder;
    int captureid;//存储保存图片返回值
    QNetworkAccessManager *manager;
    QTextToSpeech * speech;  //语音播报
    QString accessToken;
    QString AK = "bLYsydehbnUMfu9Mkg3ELGA0";
    QString SK = "kIxGB1OlFolJkZY4pJFHCgLMmiSa7x3u";
    QString jsonResult;//含车牌的json  qjsondocument->json
    QString licencePlate;//车牌
    QJsonObject obj;//可以用来解析车牌的json  qjsonobject -> json

    int count = 1000;//总车位
    bool flag = true;//默认true 入库;
    QString strTime;//时间
    int charge = 0;//收费默认入库0;
    int time = 0;//时长

    hisUI *hisui;
    AkSk *aksk;
};

#endif // WIDGET_H
