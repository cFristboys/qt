#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    hisui = new hisUI;
    aksk = new AkSk();

    ui->label_6->setScaledContents(true);
    ui->label_7->setScaledContents(true);
    //获取可用摄像头
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    QString devicename = cameras.value(0).deviceName();
    for(int i = 0;i < cameras.size();i++){
        qDebug() << cameras.value(0).deviceName() << ":" <<cameras.value(0).description();
    }
    ca = new QCamera(cameras.value(0).deviceName().toUtf8());
    //设置摄像头取景位置到label标签
    viewfinder = new QCameraViewfinder(ui->label);
    viewfinder->resize(ui->label->size());
    ca->setViewfinder(viewfinder);
    imgcapture = new QCameraImageCapture(ca);//获取摄像头图片的对象
    viewfinder->show();//显示实时拍摄
    ca->start();//摄像头启动
    manager=new QNetworkAccessManager(this);//发送get  post请求的对象

    //数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("qt");
    db.setUserName("root");
    db.setPassword("123456");
    bool ok = db.open();
    if (ok){
        QMessageBox::information(this, "infor", "mysql connect success");
    }
    else {
        QMessageBox::information(this, "infor", "open failed");
        qDebug()<<"error open database because"<<db.lastError().text();
    }
    //数据表
    QString sql = "create table if not exists qt.licenplate(intime text,licenp text,time text,charge text);";
    QSqlQuery query(sql);
    if(query.exec()){
        qDebug() << "建表错误1";
    }


    //信号
    connect(imgcapture,&QCameraImageCapture::imageCaptured,this,&Widget::imageSaved);//截取图片后触发

    connect(this,&Widget::dataInform,this,&Widget::getDataInform);


    speech= new QTextToSpeech(this);//语音播报
}

Widget::~Widget()
{
    delete ui;
    delete ca;
    delete imgcapture;
}

//获取token令牌
int getAccessToken(QString &accessToken, const QString &AK, const QString &SK) {
    QNetworkAccessManager manager;
    QUrl url(accessTokenUrl);
    QUrlQuery query;
    query.addQueryItem("grant_type", "client_credentials");
    query.addQueryItem("client_id", AK);
    query.addQueryItem("client_secret", SK);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error()) {
        qDebug() << "Network error:" << reply->errorString();
        delete reply;
        return 1;
    }

    QByteArray response_data = reply->readAll();
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data, &json_error);

    if (json_error.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << json_error.errorString();
        delete reply;
        return 1;
    }

    QJsonObject jsonObject = jsonDoc.object();
    accessToken = jsonObject["access_token"].toString();

    delete reply;
    return 0;
}
//post识别,获得车牌json,并且调用了解析函数analysisJson
int Widget::licensePlate(QString &jsonResult, const QString &accessToken,QByteArray p) {
    QNetworkAccessManager manager;
    QUrl url(requestUrl);
    QUrlQuery query;
    query.addQueryItem("access_token", accessToken);
    url.setQuery(query);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\""));
    imagePart.setBody(p); // Replace "【base64_img】" with the actual base64 encoded image data
    multiPart->append(imagePart);

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply); // Delete the multiPart with the reply

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error()) {
        qDebug() << "Network error:" << reply->errorString();
        delete reply;
        return 1;
    }

    QByteArray response_data = reply->readAll();

    obj =  QJsonDocument::fromJson(response_data).object();
    qDebug() << "obj :"<<obj;
    analysisJson(obj,licencePlate);//解析
    qDebug() << licencePlate;
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data, &json_error);

    if (json_error.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << json_error.errorString();
        delete reply;
        return 1;
    }
//    jsonResult = QString(response_data);
//    qDebug() << jsonResult;
    delete reply;
    return 0;
}
//入库触发,截取车牌图片 发送imageCaptured信号
void Widget::on_pushButton_2_clicked()
{
//    movie->stop();
    QMovie *movie = new QMovie("img/小车1.gif"); // 使用资源系统路径或文件系统路径
    // 将QMovie设置到QLabel
    ui->label_6->setMovie(movie);

    flag = true;//入库
    charge = 0;//收费0
    QString path = "D:/Code/QtCode/img/";
    imgcapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    strTime = dateTime .toString("yyyy-MM-dd-hh:mm:ss");//格式化时间
    qDebug() <<"strTime "<<strTime;
    captureid = imgcapture->capture(QString("D:/Code/QtCode/img/" +strTime + ".jpg"));   //截取图片 成功发送信号
    qDebug() <<QString(path + strTime + ".jpg");
//  QMessageBox::information(this, "通知", "入库成功");
    movie->start();
}

void Widget::on_pushButton_3_clicked()
{
//    movie->stop();
    QMovie *movie = new QMovie("img/小车2.gif"); // 使用资源系统路径或文件系统路径
    // 将QMovie设置到QLabel
    ui->label_7->setMovie(movie);
    flag = false;//出库
    charge = 0;//收费0
    QString path = "D:/Code/QtCode/img/";
    imgcapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
     strTime = dateTime .toString("yyyy-MM-dd-hh:mm:ss");//格式化时间
    qDebug() <<"strTime "<<strTime;
    captureid = imgcapture->capture(QString("D:/Code/QtCode/img/" +strTime + ".jpg"));
    qDebug() <<QString(path + strTime + ".jpg");
    movie->start();
}

//接收imageCaptured信号,主动发送一个信号,调用getAccessToken获取accesstoken  通过licensePlate获取jsonResult和obj
void Widget::imageSaved(int id, const QImage &preview)
{
        qDebug() << 1;
        QPixmap pix = QPixmap::fromImage(preview);
        QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
        strTime = dateTime .toString("yyyy-MM-dd-hh:mm:ss");//格式化时间
        qDebug() << strTime;
        QString dd = dateTime.toString("yyyymmddhhmmss");
        QString filename ="./img/"+ dd + ".jpg";
        qDebug()<<"filename : "<<filename;
       if(!pix.save(QString(filename)))
       {
           qDebug() << "图片保存失败";
       }

        ui->label_8->setPixmap(pix.scaled(ui->label_8->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        QImage p = preview;

        QByteArray body= imageToBase64(p);
        if(0 == getAccessToken(accessToken,AK,SK))//h获取令牌
        {
            qDebug() << "成功";
        }
        licensePlate(jsonResult,accessToken,body);//函数体内obj获得车牌 解析了车牌licencePlate
//        speak(licencePlate,1,0.1,QLocale::Chinese);
        if(licencePlate.isEmpty())
        {
            QMessageBox::warning(this,"警告","识别错误");
            QString text("识别错误");
            speak(text,1,0.1,QLocale::Chinese);
            return;
        }
        emit dataInform();//知道车牌了,主动触发信号

}
//json解析
int Widget::analysisJson(QJsonObject &obj,QString &licencePlate){
    QJsonValue wordsResultValue = obj["words_result"];
      if (wordsResultValue.isObject()) {
          QJsonObject wordsResultObj = wordsResultValue.toObject();
          licencePlate = wordsResultObj["number"].toString();
          qDebug() << "License Plate Number:" << licencePlate;
      } else {
          qDebug() << "words_result is not an object.";
      }
       return 0;
}

void Widget::getDataInform()
{
    //获取当前时间
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    strTime = dateTime .toString("yyyy-MM-dd-hh:mm:ss");//格式化时间
    //存入数据库
    saveInMysql();
    //收费
    //ui设置
}

QByteArray Widget::imageToBase64(QImage pix)//图片类型转换
{
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    pix.save(&buf, "JPG"); // 保存图片到QBuffer
    QString base64String = ba.toBase64(); // 转换为Base64

    return base64String.toUtf8();
}
//语音播放
void Widget::speak(const QString &text, qreal volume = 1.0, qreal rate = 1.0, const QLocale &locale = QLocale::Chinese) {
    // 检查QTextToSpeech是否可用
    if (speech->state() == QTextToSpeech::Ready) {
        // 设置音量
        speech->setVolume(volume);

        // 设置速率
        speech->setRate(rate);

        // 获取支持的语音区域列表
        QVector<QLocale> localesVector = speech->availableLocales();

        // 将QVector转换为QList
        QList<QLocale> localesList;
        foreach (const QLocale &locale, localesVector) {
            localesList.append(locale);
        }

        // 设置语言
        if (localesList.contains(locale)) {
            speech->setLocale(locale);
        } else {
            qDebug() << "The desired locale is not supported.";
        }

        // 播报文本
        speech->say(text);
    } else {
        qDebug() << "Text-to-speech not ready.";
    }
}
//时长和钱
int Widget::chargeFun(QString &dateTime)
{
    QDateTime dt= QDateTime::currentDateTime();//获取系统当前的时间
    QDateTime dt2 = QDateTime::fromString(dateTime, "yyyy-MM-dd-hh:mm:ss");//数据库中的历史时间转回qdatetime

    // 获取时区
       QTimeZone timeZone = QTimeZone::systemTimeZone();

       // 计算时差（以秒为单位）
       qint64 startTimeStamp = dt2.toSecsSinceEpoch();
       qint64 endTimeStamp = dt.toSecsSinceEpoch();
       qint64 timeDifference = endTimeStamp - startTimeStamp;

       time = static_cast<int>(timeDifference);
       charge = time/10;
       if(charge == 0){
           charge = 1;
       }
       qDebug() << startTimeStamp;
       qDebug() << endTimeStamp;
       qDebug() << timeDifference<< "秒"<<"10秒一元";
    return 0;
}
//存取数据 调用数据库,获取数据
void Widget::saveInMysql()
{
    QSqlQuery query;
    QString sql;
    QVariant dTime;
    QString dateTime;
    if(flag){//入库
        charge = 0;
        sql = "select * from qt.licenplate where licenp = ?;";
        query.prepare(sql);
        query.addBindValue(licencePlate);
        query.exec();
        if(query.next()){
            sql = "update qt.licenplate set intime = ? where licenp = ?;";
            query.prepare(sql);
            query.addBindValue(strTime);
            query.addBindValue(licencePlate);
            query.exec();
        }else{
            sql = "insert into qt.licenplate(intime,licenp) values (?,?);";
            query.prepare(sql);
            query.addBindValue(strTime);
            query.addBindValue(licencePlate);
            query.exec();

        }
        count--;
        std::string str = std::to_string(count);
        QString text(licencePlate+ "入库,剩余车位"+QString::fromStdString(str));
        speak(text,1,0.1,QLocale::Chinese);
    }
    else
    {//出库
        sql = "SELECT intime FROM qt.licenplate where licenp=?;";

        query.prepare(sql);
//        query.addBindValue(time);
        query.addBindValue(licencePlate);
//        qDebug()<<"123 : "<<licencePlate;
        query.exec();
        dTime = query.value(0);
        dateTime = dTime.toString();
        if(query.next()){
            dTime = query.value(0);
            dateTime = dTime.toString();
        }else
        {
            qDebug()<<query.lastError();
            qDebug()<<"无数据";
            QMessageBox :: warning(this,"警告","陌生车牌");
            return ;
        }

        chargeFun(dateTime);

        sql = "update qt.licenplate set time = ?,charge = ? where licenp=?;";
        query.prepare(sql);
        query.addBindValue(time);
        query.addBindValue(charge);
        query.addBindValue(licencePlate);
        query.exec();
        std::string str = std::to_string(charge);
        QString text(licencePlate+ "出库,收费"+QString::fromStdString(str)+"元");
        speak(text,1,0.1,QLocale::Chinese);
        count++;
    }
    setWUI();
    //这里可以出库或者入库一次,就生成一次报表
    //sethisUI,历史报表查看可以通过按钮来做
}
//历史报表
void Widget::on_pushButton_clicked()
{
    hisui->setWindowTitle("历史报表");
    hisui->setHisUi();
}
//显示数据
void Widget::setWUI()
{
    ui->label_2->setText(QString("车牌:"+licencePlate));//车牌  时间  剩余车位,收费

    ui->label_3->setText(QString("时间: "+strTime));
    std::string str = std::to_string(count);
    ui->label_4->setText(QString("剩余车位: "+QString::fromStdString(str)));
    str = std::to_string(charge);
    ui->label_5->setText(QString("收费: "+QString::fromStdString(str)));
}

void Widget::on_pushButton_4_clicked()
{
    QString filename = QFileDialog::getOpenFileName();

    // 创建QFile对象，指定文件路径
        QFile file(filename);

        // 尝试打开文件
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "无法打开文件进行写入";
            return ;
        }
        QSqlQuery query;
        QString sql = "select * from qt.licenplate;";
        query.prepare(sql);
        query.exec();
        QTextStream out(&file);
        out << QString("时间-车牌-时长-收费").toUtf8() << endl;

        while(query.next()){
            for(int i = 0;i< query.record().count(); i++){
                qDebug() << query.value(i).toString();
                out<<">"<< query.value(i).toString().toUtf8()<<">";
            }
           out << endl;
        }
        QMessageBox::information(this,"提示","导出成功");
        file.close();
}
//aksk获取;


void Widget::on_pushButton_5_clicked()
{
    aksk->setshow();
    AK = aksk->getAk();
    SK = aksk->getSk();
}
