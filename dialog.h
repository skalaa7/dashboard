#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
//#include "QtTheme.h"
#include <QFile>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QRandomGenerator>
#define SPEED 5
QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private slots:
    void dashboard();

    void on_radioButton_toggled(bool checked);

private:
    Ui::Dialog *ui;
    QPixmap pix_map,pix_player,pix_compass,pix_target;
    const char PCF8591 = 0x48; // adresa
    int fd, adcx,adcy;
    QTimer *mytimer;
    int playrx,playry,playrz,trgtx,trgty,trgtz;
    int pitch,yaw,xyangle;
    int difx,dify,difz;
    int score;
    bool hold_altitude;
};
#endif // DIALOG_H
