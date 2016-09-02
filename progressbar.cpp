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

#include "progressbar.h"
#include "ui_progressbar.h"

ProgressBar::ProgressBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressBar)
{
    ui->setupUi(this);
}

ProgressBar::~ProgressBar()
{
    delete ui;
}

void ProgressBar::on_progressBar_valueChanged(int value)
{
    ui->percentageLabel->setText(QString::number(percentage) + " %");
    this->update();
    QApplication::processEvents();
}

void ProgressBar::fileDone(){
    progress++;
    int tmp = (progress*100)/number;
    if(tmp != percentage){
        percentage = tmp;
        ui->progressBar->setValue(progress);
    }
    if(progress == number){
        ui->pushButton->setText("Done");
        //this->hide();

    }
}

void ProgressBar::setNumber(int n){
    number = n;
    ui->progressBar->setMaximum(number);
    ui->progressBar->setValue(0);
    ui->percentageLabel->setText("0 %");
    percentage = 0;
    progress = 0;
    ui->pushButton->setText("Cancel");
    //this->repaint();
    //QApplication::processEvents();
}

void ProgressBar::on_pushButton_clicked()
{
    emit cancelSignal();
    this->hide();
}
