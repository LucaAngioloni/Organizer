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

#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::setPointers(bool *r, bool *d){
    recursive = r;
    deleteOriginals = d;

    ui->DeleteOriginals->setChecked(*deleteOriginals);
    ui->RecursiveSearch->setChecked(*recursive);

    rec = *r;
    del = *d;
}

void Preferences::on_DeleteOriginals_toggled(bool checked)
{
    del = checked;
}


void Preferences::on_RecursiveSearch_toggled(bool checked)
{
    rec = checked;
}

void Preferences::on_OK_clicked()
{
    *recursive = rec;
    *deleteOriginals = del;
    emit this->Closed();
    this->hide();
}

void Preferences::on_Cancel_clicked()
{
    this->hide();
}
