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

#include "filterwindow.h"
#include "ui_filterwindow.h"

FilterWindow::FilterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);
    addOrMod = 0;
    ui->extensionEditLine->setPlaceholderText("Extension");
    ui->extensionEditLine->setDisabled(true);
    ui->wordsTable->setColumnWidth(0,370);
    ui->minSlider->setMinimum(0);
    ui->minSlider->setMaximum(13);
    ui->maxSlider->setMinimum(0);
    ui->maxSlider->setMaximum(13);

    dirCompleter = new QCompleter(this);
    QDirModel *model = new QDirModel(dirCompleter);
    model->setFilter(QDir::Dirs);
    dirCompleter->setModel(model);
    dirCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->pathText->setCompleter(dirCompleter);
    caseSens = false;
    ui->caseSensitivityBox->setChecked(caseSens);
}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::setDatabase(FilterDatabase *data){
    database = data;
}

void FilterWindow::fillInfo(QString filterName){
    addOrMod = 1;
    currentFilter = filterName;

    ui->extensionEditLine->clear();

    ui->nameText->setText(database->getElement(currentFilter).getName());
    ui->pathText->setText(database->getElement(currentFilter).getPath());
    setMinDimensionFromMB(database->getElement(currentFilter).getMinDimension());
    setMaxDimensionFromMB(database->getElement(currentFilter).getMaxDimension());
    updateMBLabel();

    if(database->getElement(currentFilter).getExtension() != "%Any%" &&
            database->getElement(currentFilter).getExtension() != "%Image%" &&
            database->getElement(currentFilter).getExtension() != "%Video%" &&
            database->getElement(currentFilter).getExtension() != "%Music%" &&
            database->getElement(currentFilter).getExtension() != "%Document%" &&
            database->getElement(currentFilter).getExtension() != "%Book%" &&
            database->getElement(currentFilter).getExtension() != "%Compressed%")
    {
        ui->extensionEditLine->setEnabled(true);
        ext = database->getElement(currentFilter).getExtension();
        ui->extensionEditLine->setText(ext);
        ui->extensionBox->setCurrentText("Other");
    }
    else{
        ext = database->getElement(currentFilter).getExtension();
        QString tmpE = ext;
        ui->extensionBox->setCurrentText(tmpE.remove("%"));
    }

    ui->wordsTable->clear();
    ui->wordsTable->setRowCount(0);

    for(int i = 0; i< database->getElement(currentFilter).complexity(); i++){
        ui->wordsTable->insertRow(i);
        ui->wordsTable->setItem(i, 0, new QTableWidgetItem(database->getElement(currentFilter).getWordAtIndex(i)));
    }
    Qt::SortOrder order = Qt::AscendingOrder;
    ui->wordsTable->sortItems ( 0, order );
    caseSens = database->getElement(currentFilter).getCaseSensitivity();
    ui->caseSensitivityBox->setChecked(caseSens);
}

void FilterWindow::setDefault(){
    addOrMod = 0;
    currentFilter.clear();
    ui->nameText->clear();
    ui->pathText->clear();
    ui->extensionEditLine->clear();
    ui->extensionEditLine->setDisabled(true);
    ui->extensionBox->setCurrentIndex(0);
    ext = "%Any%";
    ui->minSlider->setValue(0);
    ui->maxSlider->setValue(13);
    ui->wordsTable->clear();
    ui->wordsTable->setRowCount(0);
    caseSens = false;
    ui->caseSensitivityBox->setChecked(caseSens);
}

void FilterWindow::on_browseButton_clicked()
{
    QString filePath;
    filePath = QFileDialog::getExistingDirectory(this,
                                                 tr("Choose Path"), "/Users/");
    ui->pathText->setText(filePath);
}


void FilterWindow::newFilter(){
    Filter newFilter(ui->nameText->text(), ui->pathText->text(), caseSens, getMinDimensionMB(), getMaxDimensionMB(), getExt());
    for(int i = 0; i<ui->wordsTable->rowCount(); i++){
        if(ui->wordsTable->item(i,0)){
            newFilter.addWord(ui->wordsTable->item(i,0)->text());
        }
    }
    //check if path is an actual directory. (forse rompe le scatole ed è superfluo).
//    QFileInfo temp(ui->pathText->text());
//    if(!temp.isDir()){
//        QMessageBox msgBox;
//        msgBox.setText("Path error");
//        msgBox.setInformativeText("The path is not a folder. Please choose a new one");
//        msgBox.setStandardButtons(QMessageBox::Ok);
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        msgBox.exec();
//    }
//    else{
        if(!database->addFilter(newFilter)){
            QMessageBox msgBox;
            msgBox.setText("Name error");
            msgBox.setInformativeText("This name has already been used for an other filter or is empty. Please choose a new one");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        else {
            this->hide();
            emit filterDone();
        }
//    }
}

void FilterWindow::modFilter(){
    if (ui->nameText->text() == "" || ((database->nameIsOccupied(ui->nameText->text()) && ui->nameText->text() != currentFilter))){
        QMessageBox msgBox;
        msgBox.setText("Name error");
        msgBox.setInformativeText("This name has already been used for an other filter or is empty. Please choose a new one");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else{
        //check if path is an actual directory. (forse rompe le scatole ed è superfluo).
//        QFileInfo temp(ui->pathText->text());
//        if(!temp.isDir()){
//            QMessageBox msgBox;
//            msgBox.setText("Path error");
//            msgBox.setInformativeText("The path is not correct or is empty. Please choose a new one");
//            msgBox.setStandardButtons(QMessageBox::Ok);
//            msgBox.setDefaultButton(QMessageBox::Ok);
//            msgBox.exec();
//        }
//        else{
            database->getElement(currentFilter).setPath(ui->pathText->text());
            database->getElement(currentFilter).setMinDimension(getMinDimensionMB());
            if(getMaxDimensionMB() == -1){
                database->getElement(currentFilter).setMaxDimension(-1, true);
            }
            else {
                database->getElement(currentFilter).setMaxDimension(getMaxDimensionMB());
            }
            database->getElement(currentFilter).setExtension(getExt());
            database->getElement(currentFilter).clearWords();
            for(int i = 0; i<ui->wordsTable->rowCount(); i++){
                if(ui->wordsTable->item(i,0)){
                    database->getElement(currentFilter).addWord(ui->wordsTable->item(i,0)->text());
                }
            }
            database->getElement(currentFilter).setCaseSensitivity(caseSens);
            database->getElement(currentFilter).setName(ui->nameText->text());              //name goes last otherwise getElement returns empty
            this->hide();
            emit filterDone();
//        }
    }
}

void FilterWindow::on_cancelButton_clicked()
{
    this->hide();
}

void FilterWindow::on_okButton_clicked()
{
    if(addOrMod){
        modFilter();
    }
    else{
        newFilter();
    }
}

//methods to convert the value of sliders in usable MB values
int FilterWindow::getMinDimensionMB(){
    int minDim;
    switch (ui->minSlider->value()){
    case 0 :
        minDim = 0;
        break;
    case 1 :
        minDim = 1;
        break;
    case 2 :
        minDim = 2;
        break;
    case 3:
        minDim = 10;
        break;
    case 4 :
        minDim = 30;
        break;
    case 5 :
        minDim = 50;
        break;
    case 6 :
        minDim = 100;
        break;
    case 7 :
        minDim = 500;
        break;
    case 8 :
        minDim = 1024;
        break;
    case 9 :
        minDim = 2048;
        break;
    case 10 :
        minDim = 5120;
        break;
    case 11 :
        minDim = 10240;
        break;
    case 12 :
        minDim = 51200;
        break;
    case 13 :
        minDim = 102400;
        break;
    }
    return minDim;
}

int FilterWindow::getMaxDimensionMB(){
    int maxDim;
    switch (ui->maxSlider->value()){
    case 0 :
        maxDim = 1;
        break;
    case 1 :
        maxDim = 2;
        break;
    case 2 :
        maxDim = 10;
        break;
    case 3:
        maxDim = 30;
        break;
    case 4 :
        maxDim = 50;
        break;
    case 5 :
        maxDim = 100;
        break;
    case 6 :
        maxDim = 500;
        break;
    case 7 :
        maxDim = 1024;
        break;
    case 8 :
        maxDim = 2048;
        break;
    case 9 :
        maxDim = 5120;
        break;
    case 10 :
        maxDim = 10240;
        break;
    case 11 :
        maxDim = 51200;
        break;
    case 12 :
        maxDim = 102400;
        break;
    case 13 :
        maxDim = -1;
        break;
    }
    return maxDim;
}

QString FilterWindow::getExt(){
    if(ext == "%Other%"){
        QString tmpExt = ui->extensionEditLine->text();
        tmpExt.remove(" ");
        return tmpExt;
    }
    else{
        return ext;
    }
}

void FilterWindow::setMinDimensionFromMB(int minMB){
    switch (minMB){
    case 0 :
        ui->minSlider->setValue(0);
        break;
    case 1 :
        ui->minSlider->setValue(1);
        break;
    case 2 :
        ui->minSlider->setValue(2);
        break;
    case 10 :
        ui->minSlider->setValue(3);
        break;
    case 30 :
        ui->minSlider->setValue(4);
        break;
    case 50 :
        ui->minSlider->setValue(5);
        break;
    case 100 :
        ui->minSlider->setValue(6);
        break;
    case 500 :
        ui->minSlider->setValue(7);
        break;
    case 1024 :
        ui->minSlider->setValue(8);
        break;
    case 2048 :
        ui->minSlider->setValue(9);
        break;
    case 5120 :
        ui->minSlider->setValue(10);
        break;
    case 10240 :
        ui->minSlider->setValue(11);
        break;
    case 51200 :
        ui->minSlider->setValue(12);
        break;
    case 102400 :
        ui->minSlider->setValue(13);
        break;
    }
}

void FilterWindow::setMaxDimensionFromMB(int maxMB){
    if(maxMB == -1){
        ui->maxSlider->setValue(13);
    }
    else{
        switch (maxMB){
        case 1 :
            ui->maxSlider->setValue(0);
            break;
        case 2 :
            ui->maxSlider->setValue(1);
            break;
        case 10 :
            ui->maxSlider->setValue(2);
            break;
        case 30 :
            ui->maxSlider->setValue(3);
            break;
        case 50 :
            ui->maxSlider->setValue(4);
            break;
        case 100 :
            ui->maxSlider->setValue(5);
            break;
        case 500 :
            ui->maxSlider->setValue(6);
            break;
        case 1024 :
            ui->maxSlider->setValue(7);
            break;
        case 2048 :
            ui->maxSlider->setValue(8);
            break;
        case 5120 :
            ui->maxSlider->setValue(9);
            break;
        case 10240 :
            ui->maxSlider->setValue(10);
            break;
        case 51200 :
            ui->maxSlider->setValue(11);
            break;
        case 102400 :
            ui->maxSlider->setValue(12);
            break;
        }
    }
}

void FilterWindow::on_extensionBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 != "Other"){
        ui->extensionEditLine->setDisabled(true);
        QPalette pal = ui->extensionEditLine->palette();
        pal.setColor(QPalette::Background, Qt::gray);
        ui->extensionEditLine->setPalette(pal);
        ui->extensionEditLine->setAutoFillBackground(true);
    }
    else{
        ui->extensionEditLine->setEnabled(true);
    }
    ext = "%" + arg1 + "%";
}

void FilterWindow::on_minSlider_valueChanged(int value)
{
    if(value > ui->maxSlider->value()){
        ui->maxSlider->setValue(value);
    }
    updateMBLabel();
}

void FilterWindow::on_maxSlider_valueChanged(int value)
{
    if(value < ui->minSlider->value()){
        ui->minSlider->setValue(value);
    }
    updateMBLabel();
}

void FilterWindow::updateMBLabel(){
    QString MBValue;
    switch (ui->minSlider->value()){
    case 0 :
        MBValue = "0 MB";
        break;
    case 1 :
        MBValue = "1 MB";
        break;
    case 2 :
        MBValue = "2 MB";
        break;
    case 3:
        MBValue = "10 MB";
        break;
    case 4 :
        MBValue = "30 MB";
        break;
    case 5 :
        MBValue = "50 MB";
        break;
    case 6 :
        MBValue = "100 MB";
        break;
    case 7 :
        MBValue = "500 MB";
        break;
    case 8 :
        MBValue = "1 GB";
        break;
    case 9 :
        MBValue = "2 GB";
        break;
    case 10 :
        MBValue = "5 GB";
        break;
    case 11 :
        MBValue = "10 GB";
        break;
    case 12 :
        MBValue = "50 GB";
        break;
    case 13 :
        MBValue = "100 GB";
        break;
    }
    ui->minMBLabel->setText(MBValue);

    switch (ui->maxSlider->value()){
    case 0 :
        MBValue = "1 MB";
        break;
    case 1 :
        MBValue = "2 MB";
        break;
    case 2 :
        MBValue = "10 MB";
        break;
    case 3:
        MBValue = "30 MB";
        break;
    case 4 :
        MBValue = "50 MB";
        break;
    case 5 :
        MBValue = "100 MB";
        break;
    case 6 :
        MBValue = "500 MB";
        break;
    case 7 :
        MBValue = "1 GB";
        break;
    case 8 :
        MBValue = "2 GB";
        break;
    case 9 :
        MBValue = "5 GB";
        break;
    case 10 :
        MBValue = "10 GB";
        break;
    case 11 :
        MBValue = "50 GB";
        break;
    case 12 :
        MBValue = "100 GB";
        break;
    case 13 :
        MBValue = "∞";
        break;
    }
    ui->maxMBLabel->setText(MBValue);
}

void FilterWindow::on_plusButton_clicked()
{
    int i = ui->wordsTable->rowCount();
    if(ui->wordsTable->item(i-1,0) || i == 0){
        ui->wordsTable->insertRow(i);
        ui->wordsTable->selectRow(i);
    }
}

void FilterWindow::on_minusButton_clicked()
{
    QList <QTableWidgetItem *> selected = ui->wordsTable->selectedItems();
    for(int j = 0; j < selected.size(); j++){
        ui->wordsTable->removeRow(selected.at(j)->row());
    }
}

void FilterWindow::on_caseSensitivityBox_clicked(bool checked)
{
    caseSens = checked;
}
