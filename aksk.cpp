#include "aksk.h"
#include "ui_aksk.h"

AkSk::AkSk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AkSk)
{
    ui->setupUi(this);
}

AkSk::~AkSk()
{
    delete ui;
}

QString AkSk::getAk()
{
    if(ui->Ak->text().isEmpty())
    {
        return "ak is null";
    }else
    {
        ak = ui->Ak->text();
        return ak;
    }
}

QString AkSk::getSk()
{
    if(ui->Sk->text().isEmpty())
    {
        return "sk is null";
    }else
    {
        sk = ui->Sk->text();
        return sk;
    }
}

void AkSk::setshow()
{
    this->show();
}

void AkSk::on_pushButton_clicked()
{
    if(ui->Sk->text().isEmpty() ||ui->Ak->text().isEmpty())
        return ;
    this->hide();
}
