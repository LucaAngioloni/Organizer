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

#include "filters.h"
#include "ui_filters.h"
#include "organizer.h"

Filters::Filters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filters)
{
    ui->setupUi(this);
    ui->tableOfFilters->setColumnCount(2);
    ui->tableOfFilters->setHorizontalHeaderLabels(QStringList() << "Name" << "Path");
    ui->tableOfFilters->setColumnWidth(0, 163);
    ui->tableOfFilters->setColumnWidth(1, 350);
    ui->tableOfFilters->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableOfFilters->setEditTriggers(QAbstractItemView::NoEditTriggers);

    fWindow.setModal(true);

    QObject::connect(&fWindow, SIGNAL(filterDone()), this, SLOT(fWindowDone()));
}

Filters::~Filters()
{
    delete ui;
}

void Filters::fWindowDone(){
    fillTable();
}

void Filters::on_addFilter_clicked()
{
    fWindow.setDefault();
    fWindow.show();
}

void Filters::setDatabase(FilterDatabase *data){
    database = data;
    fWindow.setDatabase(data);
}

void Filters::on_ClearButton_clicked()
{
    QMessageBox msgClear;
    msgClear.setWindowTitle("Clear");
    msgClear.setText("Are you really sure you want to delete every single filter you created?");
    msgClear.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgClear.setDefaultButton(QMessageBox::No);
    if(msgClear.exec() == QMessageBox::Yes){
        database->clear();
        ui->label->setText(":");
    }
    fillTable();
}

void Filters::fillTable(){
    ui->tableOfFilters->clear();
    ui->tableOfFilters->setRowCount(0);
    ui->tableOfFilters->setHorizontalHeaderLabels(QStringList() << "Name" << "Path");
    for(int i = 0; i<database->size(); i++){
        ui->tableOfFilters->insertRow(i);
        ui->tableOfFilters->setItem(i, 0, new QTableWidgetItem(database->getElement(i).getName()));
        ui->tableOfFilters->setItem(i, 1, new QTableWidgetItem(database->getElement(i).getPath()));
    }
    Qt::SortOrder order = Qt::AscendingOrder;
    ui->tableOfFilters->sortItems ( 0, order );
}

void Filters::on_tableOfFilters_cellClicked(int row, int column)
{
    filterClicked = ui->tableOfFilters->item(row, 0)->text();
    ui->label->setText(":" + filterClicked);
}


void Filters::on_tableOfFilters_cellActivated(int row, int column)
{
     filterClicked = ui->tableOfFilters->item(row, 0)->text();
     ui->label->setText(":" + filterClicked);
}

void Filters::on_RemoveButton_clicked()
{
    if (filterClicked != ""){
        QMessageBox msgRemove;
        msgRemove.setWindowTitle("Remove");
        msgRemove.setText("Are you really sure you want to delete the filter:" + filterClicked + "?");
        msgRemove.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgRemove.setDefaultButton(QMessageBox::No);
        if(msgRemove.exec() == QMessageBox::Yes){
            database->removeFilter(filterClicked);
            ui->label->setText(":");
        }
    }
    fillTable();
}

void Filters::on_tableOfFilters_cellDoubleClicked(int row, int column)
{
    filterClicked = ui->tableOfFilters->item(row, 0)->text();
    ui->label->setText(":" + filterClicked);
    fWindow.fillInfo(filterClicked);
    fWindow.show();
}

void Filters::on_Filters_accepted()
{
    //save database when this window is closed.
    database->save();
}
