#ifndef AKSK_H
#define AKSK_H

#include <QWidget>

namespace Ui {
class AkSk;
}

class AkSk : public QWidget
{
    Q_OBJECT

public:
    explicit AkSk(QWidget *parent = 0);
    ~AkSk();

    QString getAk();
    QString getSk();
    void setshow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AkSk *ui;
    QString ak;
    QString sk;
};

#endif // AKSK_H
