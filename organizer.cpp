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

#include "organizer.h"
#include "ui_organizer.h"
#include <QWindow>

Organizer::Organizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TvSeries)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    removeOriginal = false;
    subFolderSearch = false;
    working = false;
    Database = new FilterDatabase();
    loadDatabase();
    loadPrefs();
    ui->LogWindow->insertPlainText("Drag and Drop files to copy\n\n");
    threadPool = QThreadPool::globalInstance();
    threadPool->setMaxThreadCount(40);
    fileList = new FileList();
    scanCancel = false;
    dropList = new QStringList();

    treeView = new QTreeView();
    model = new QFileSystemModel();
    treeView->setModel(model);
    treeView->setColumnWidth(0, 300);
    treeView->setColumnWidth(1, 180);
}

Organizer::~Organizer()
{
    delete ui;
}

void Organizer::dragEnterEvent(QDragEnterEvent *ev)
{
    if(!working){
        ev->accept();
    }
}

void Organizer::dropEvent(QDropEvent *ev)
{
    ev->accept();
    if(ev->isAccepted()){
        this->setAcceptDrops(false);
        working = true;
        scanWindow = new ScanningWindow(fileList, &scanCancel, this);
        QList<QUrl> tmpL;
        tmpL = ev->mimeData()->urls();
        QStringList folderTmp;
        QStringList fileTmp;
        for(int i = 0; i<tmpL.size(); i++){
            QString sTmp = tmpL.at(i).toString();
            if(!sTmp.endsWith("/")){
                fileTmp.append(sTmp);
            }
            else {
                folderTmp.append(sTmp);
            }
            dropList->append(sTmp);
        }
        fileList->pushQueue(folderTmp);
        ScanTask* t = new ScanTask(fileList, subFolderSearch, &scanCancel);
        t->setAutoDelete(true);
        QThreadPool::globalInstance()->start(t);
        checker = new PoolChecker();
        QObject::connect(this, SIGNAL(checkPool()), checker, SLOT(startChecking()));
        QObject::connect(checker, SIGNAL(scanFinished()), this, SLOT(onScanFinished()));
        QObject::connect(this, SIGNAL(checkPool()), scanWindow, SLOT(showInfo()));
        QObject::connect(checker, SIGNAL(scanFinished()), scanWindow, SLOT(scanFinished()));
        QObject::connect(scanWindow, SIGNAL(cancelClicked()), this, SLOT(onScanCanceled()));
        checkerThread = new QThread;
        checker->moveToThread(checkerThread);
        checkerThread->start();
        emit checkPool();
        fileList->appendFile(fileTmp); //executes after the creation of threads
    }
}

void Organizer::showDropList()
{
    ui->Lista->clear();
    for(int i = 0; i< dropList->size(); i++){
        QListWidgetItem *item = new QListWidgetItem(ui->Lista);
        QString tmp = dropList->at(i);
        tmp.remove(0,7);
        if(tmp.endsWith("/")){
            QDir d(tmp);
            QString name = d.dirName();
            item->setText(name);
            QFileIconProvider iconSource;
            QIcon icon = iconSource.icon(iconSource.Folder);
            item->setIcon(icon);
        }
        else {
            QFileInfo fi(tmp);
            QString name = fi.fileName();
            item->setText(name);
            QFileIconProvider iconSource;
            QIcon icon = iconSource.icon(fi);
            if(!icon.isNull()){
                item->setIcon(icon);
            }
            else{
                icon = iconSource.icon(iconSource.File);
            }
        }
        ui->Lista->addItem(item);
    }
    ui->pathLabel->clear();
}

void Organizer::onScanFinished(){
    checker->deleteLater();
    checkerThread->exit();
    scanWindow->deleteLater();
    showDropList();
    QString numFiles = QString::number(fileList->size());
    ui->LogWindow->insertPlainText("\n" + numFiles + " files to copy\n");
    if(fileList->getDimension() < 1500){
        ui->LogWindow->insertPlainText(QString::number(fileList->getDimension()) + " MB\n\n");
    }
    else{
        ui->LogWindow->insertPlainText(QString::number(fileList->getDimension()/1000) + " GB\n\n");
    }
    QScrollBar *sb = ui->LogWindow->verticalScrollBar();
    sb->setValue(sb->maximum());
    scanCancel = false;
    this->setAcceptDrops(true);
    working = false;
}

void Organizer::onScanCanceled()
{
    ui->LogWindow->insertPlainText("Scanning was canceled! \n\n");
    this->setAcceptDrops(true);
    working = false;
}


void Organizer::on_Clear_clicked()
{
    if(!working){
        dropList->clear();
        ui->Lista->clear();
        fileList->clear();
        ui->LogWindow->update();
        ui->LogWindow->insertPlainText("Clear\nDrag and Drop files to copy\n\n");
        QScrollBar *sb = ui->LogWindow->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}

void Organizer::on_Copy_clicked()
{
    if(!working){
        this->setAcceptDrops(false);
        working = true;
        parallelCopy = new QThread;
        copyThread = new CopyThread;

        QObject::connect(this, SIGNAL(startCopy()), copyThread, SLOT(copyStart()));
        QObject::connect(copyThread, SIGNAL(fileElab(QString)), this, SLOT(fileCopyDone(QString)));
        QObject::connect(copyThread, SIGNAL(copyFinished()), this, SLOT(copyThreadFinished()));
        QObject::connect(copyThread, SIGNAL(fileElab(QString)), progressBar, SLOT(fileDone()));

        copyThread->setPointers(fileList->getList(), Database, removeOriginal);
        copyThread->moveToThread(parallelCopy);

        if(fileList->size() > 0){
            progressBar->setNumber(fileList->size());
            progressBar->show();
            progressBar->setVisible(true);
        }

        parallelCopy->start();
        emit startCopy();
    }
}

void Organizer::fileCopyDone(QString text){
    ui->LogWindow->insertPlainText(text);
}

void Organizer::copyThreadFinished(){
    working = false;
    ui->LogWindow->insertPlainText("\n\n");
    on_Clear_clicked();

    parallelCopy->exit();
    parallelCopy->quit();
    parallelCopy->terminate();
    parallelCopy->deleteLater();
    copyThread->deleteLater();
    this->setAcceptDrops(true);
}

void Organizer::cancelCopy(){
    if(working){
        copyThreadFinished();
        ui->LogWindow->insertPlainText("\n\nCopy interrupted!!\n\n");
        QScrollBar *sb = ui->LogWindow->verticalScrollBar();
        sb->setValue(sb->maximum());
        this->setAcceptDrops(true);
        working = false;
    }
}


void Organizer::loadDatabase(){
    Database->load();
}

void Organizer::on_actionInfo_triggered()
{
    Info aboutWindow;
    aboutWindow.setModal(true);

    aboutWindow.exec();
}

void Organizer::on_prefClosed(){
    savePrefs();
}

void Organizer::savePrefs(){
    QSettings preferences;
    preferences.beginGroup("Preferences");
    preferences.setValue("Remove Original", removeOriginal);
    preferences.setValue("SubFolder Search", subFolderSearch);
    preferences.endGroup();
}

void Organizer::loadPrefs(){
    QSettings preferences;
    preferences.beginGroup("Preferences");
    removeOriginal = preferences.value("Remove Original", false).toBool();
    subFolderSearch = preferences.value("SubFolder Search", false).toBool();
    preferences.endGroup();
}

void Organizer::on_actionPreferences_triggered()
{
    prefWindow->setModal(true);
    prefWindow->show();
    prefWindow->setPointers(&subFolderSearch, &removeOriginal);
}

void Organizer::on_filterButton_clicked()
{
    Filters filterWindow;
    filterWindow.setDatabase(Database);
    filterWindow.fillTable();
    filterWindow.setModal(true);


    filterWindow.exec();
}

void Organizer::on_Lista_clicked(const QModelIndex &index)
{
    QString txt = dropList->at(index.row());
    txt.remove(0,7);
    ui->pathLabel->setText(txt);
}

void Organizer::on_Lista_doubleClicked(const QModelIndex &index)
{
    QString path = dropList->at(index.row());
    path.remove(0,7);
    if(path.endsWith("/")){
        QDir d(path);
        treeView->setRootIndex(model->setRootPath(d.absolutePath()));
        QRect rect = this->geometry();
        rect.setHeight(rect.height() - (int)(rect.height()/10));
        rect.setWidth(rect.width() - (int)(rect.width()/10));
        rect.moveCenter(this->geometry().center());
        treeView->setGeometry(rect);
        treeView->show();
    }
}
