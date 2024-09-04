#ifndef HISUI_H
#define HISUI_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QSqlDatabase>   //数据库管理文件
#include <QSqlRecord>       //数据库查询看记录文件
#include <QSqlError>        //数据库错误管理文件
#include <QSqlQuery>          //执行sql语句头文件,提供执行方法
#include <QDebug>
#include <QTableWidget>

namespace Ui {
class hisUI;
}

class hisUI : public QWidget
{
    Q_OBJECT

public:
    explicit hisUI(QWidget *parent = 0);
    void setHisUi();
    ~hisUI();

private slots:
    void on_pushButton_clicked();

private:
    Ui::hisUI *ui;
    QTableWidgetItem * items;
        int row1 = 0;
};

#endif // HISUI_H
