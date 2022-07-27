#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    wiringPiSetup ();
    fd = wiringPiI2CSetup(PCF8591);
    mytimer=new QTimer(this);
    connect(mytimer,SIGNAL(timeout()),this,SLOT(dashboard()));
    mytimer ->start(1000);
    pix_map=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/map4.png");
    //if(pixmap.isNull())
    //    ui->playerX->display(100);
    ui->map->setPixmap(pix_map);
    ui->map->setScaledContents(true);
    ui->map->show();
    pix_player=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/player0.png");
    //if(pixmap.isNull())
    //    ui->playerX->display(100);
    ui->player->setPixmap(pix_player);
    ui->player->setScaledContents(true);
    ui->player->show();
    pix_target=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/objective1.png");
    //if(pixmap.isNull())
    //    ui->playerX->display(100);
    ui->target->setPixmap(pix_target);
    ui->target->setScaledContents(true);
    ui->target->show();
    pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass0.png");
    ui->compass->setPixmap(pix_compass);
    ui->compass->setScaledContents(true);
    ui->compass->show();
    /*pix_complane=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/plane1.png");
    //if(pixmap.isNull())
    //    ui->playerX->display(100);
    ui->plane_compass->setPixmap(pix_complane);
    ui->plane_compass->setScaledContents(true);
    ui->plane_compass->show();*/
    playrx=220;
    playry=400;
    playrz=0;
    pitch=0;
    xyangle=0;
    ui->pitchSlider->setMinimum(-45);
    ui->pitchSlider->setMaximum(45);
    ui->yawSlider->setMinimum(-45);
    ui->yawSlider->setMaximum(45);
    ui->player->move(playrx+250,playry+100);
    trgtx=220;
    trgty=200;
    trgtz=50;
    ui->target->move(trgtx+250,trgty+100);
    score=0;
    ui->score->display(score);
    hold_altitude=false;

}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::dashboard()
{

    wiringPiI2CReadReg8(fd, PCF8591 + 3) ; // dummy
    adcx = wiringPiI2CReadReg8(fd, PCF8591 + 3) ;

    wiringPiI2CReadReg8(fd, PCF8591 + 2) ; // dummy
    adcy = wiringPiI2CReadReg8(fd, PCF8591 + 2);

    ui->targetX->display(trgtx);
    ui->targetY->display(trgty);
    ui->targetZ->display(trgtz);

    pitch=0;
    yaw=0;

    if( adcy>120)
    {
        if(adcx<70)
        {
            //joy_up
            pitch=-45;
        }
        else if(adcx>140)
        {
            //joy_left
            yaw=-45;
        }
    }
    else
    {
        if(adcx==-1)
        {
            //joy_down
            pitch=45;
        }
        else
        {
            //joy_right
            yaw=45;
        }
    }

    ui->pitchSlider->setValue(pitch);
    ui->yawSlider->setValue(yaw);

    if(!hold_altitude)
        playrz+=2*SPEED*(pitch/45);
    if(playrz<0)
        playrz=0;
    if(playrz>200)
        playrz=200;
    xyangle+=yaw;
    xyangle=xyangle%360;
    if(xyangle<0)
        xyangle=360+xyangle;
    switch(xyangle)
    {
        case 0:
            playry-=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass0.png");
            break;
        case 45:
            playry-=SPEED;
            playrx+=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass45.png");
            break;
        case 90:
            playrx+=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass90.png");
            break;
        case 135:
            playry+=SPEED;
            playrx+=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass135.png");
            break;
        case 180:
            playry+=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass180.png");
            break;
        case 225:
            playry+=SPEED;
            playrx-=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass225.png");
            break;
        case 270:
            playrx-=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass270.png");
            break;
        case 315:
            playry-=SPEED;
            playrx-=SPEED;
            pix_compass=QPixmap("/home/pi/RacEle/plane_dashboard/plane_dashboard/compass315.png");
            break;
    }


    difx=abs(playrx-trgtx);
    dify=abs(playry-trgty);
    difz=abs(playrz-trgtz);

    ui->compass->setPixmap(pix_compass);
    ui->compass->setScaledContents(true);
    ui->compass->show();

    ui->playerX->display(playrx);
    ui->playerY->display(playry);
    ui->playerZ->display(playrz);
    ui->player->move(playrx+250,playry+100);


    if((difx<=10)&&(dify<=10)&&(difz<10))
    {
        score+=50;
        trgtx=QRandomGenerator::global()->bounded(30,350);
        trgty=QRandomGenerator::global()->bounded(30,350);
        trgtz=QRandomGenerator::global()->bounded(20,200);
        ui->target->move(trgtx+250,trgty+100);

    }
       ui->score->display(score);

       inbounds=true;
       if((playrx<0)||(playrx>420))
           inbounds=false;
       if((playry<0)||(playry>420))
               inbounds=false;
       if(!inbounds)
       {
           msBox.setText("Your score is:"+QString::number(score));
           msBox.setInformativeText("Play again?");
           msBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
           msBox.setDefaultButton(QMessageBox::No);
           ret=msBox.exec();
       }
       switch(ret)
       {
           case QMessageBox::Yes:
                //Reset
                playrx=220;
                playry=400;
                playrz=0;
                pitch=0;
                xyangle=0;
                ui->player->move(playrx+250,playry+100);
                trgtx=QRandomGenerator::global()->bounded(30,350);
                trgty=QRandomGenerator::global()->bounded(30,350);
                trgtz=QRandomGenerator::global()->bounded(20,200);
                ui->target->move(trgtx+250,trgty+100);
                score=0;
                ui->score->display(score);
                break;
           case QMessageBox::No:
                QCoreApplication::exit();
                break;
           default:
                break;
       }
       ret=1000;

}



void Dialog::on_radioButton_toggled(bool checked)
{
    hold_altitude=checked;
}
