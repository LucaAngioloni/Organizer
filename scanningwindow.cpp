/*
Copyright 2016 Luca Angioloni
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "scanningwindow.h"
#include "ui_scanningwindow.h"
#include <QTimer>

ScanningWindow::ScanningWindow(FileList *l, bool *sc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanningWindow)
{
    ui->setupUi(this);
    list = l;
    scanCanc = sc;
    finished = false;
    waitTime = 100;
    //ui->cancelButton->hide();
}

ScanningWindow::~ScanningWindow()
{
    delete ui;
}

void ScanningWindow::setWaitText()
{
    ui->fileNumberLabel->setText("Please Wait...");
}

void ScanningWindow::on_cancelButton_clicked()
{
    emit cancelClicked();
    *scanCanc = true;
}

void ScanningWindow::showInfo(){
    int numFiles = list->size();
    if(numFiles >2500){
        this->show();
    }
    ui->fileNumberLabel->setText(QString::number(numFiles) + " files...");
    if(!(*scanCanc) && !finished){
        if(waitTime < 2500){
             waitTime += 5;
        }
        QTimer::singleShot(waitTime, this, SLOT(showInfo()));
    }
}

void ScanningWindow::scanFinished()
{
    finished = true;
    this->hide();
}
