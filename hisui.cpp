#include "hisui.h"
#include "ui_hisui.h"

hisUI::hisUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hisUI)
{
    ui->setupUi(this);

}

void hisUI::setHisUi()
{
    QSqlQuery query;
    QString sql = "select * from qt.licenplate;";
    query.prepare(sql);
    query.exec();

    ui->tableWidget->clear(); //清空,清空后要设置
    QStringList lst;
    lst << "时间" << "车牌" << "停留时长/秒" << "收费(10秒/元)";
    ui->tableWidget->setHorizontalHeaderLabels(lst);// 设置表格水平头

    ui->tableWidget->setRowCount(10);//设置行数
    ui->tableWidget->setColumnCount(query.record().count()); //设置列数

    row1 = 0;
    while(query.next()){
        for(int i = 0;i< query.record().count(); i++){
            qDebug() << query.value(i).toString();
            qDebug() << "*8888888888888888888888888888888888";
            QTableWidgetItem *items = new QTableWidgetItem(query.value(i).toString());

            items->setTextAlignment(Qt::AlignCenter);
            items->setFont(QFont("楷体",20));

            ui->tableWidget->setItem(row1,i,items);//设置某行,某列的项
        }
        row1++;
    }
    this->show();
}

hisUI::~hisUI()
{
    delete ui;
}

void hisUI::on_pushButton_clicked()
{
    this->hide();
}
