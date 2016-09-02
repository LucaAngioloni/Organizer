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

#ifndef TVSERIES_H
#define TVSERIES_H

#include "filelist.h"

#include <QApplication>
#include <QMainWindow>
#include <QDropEvent>
#include <QThread>
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QScrollBar>
#include <QFileSystemModel>
#include <vector>
#include <QMessageBox>
#include <QSettings>
#include <string>
#include <filterdatabase.h>
#include <filter.h>
#include <filters.h>
#include <info.h>
#include <preferences.h>
#include <progressbar.h>
#include <copythread.h>
#include <scanningwindow.h>
#include <QThreadPool>
#include "scantask.h"
#include "poolchecker.h"
#include <QTreeView>

namespace Ui {
class TvSeries;
}

class Organizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Organizer(QWidget *parent = 0);
    ~Organizer();

    void setPrefs(Preferences* pref){
        prefWindow = pref;
    }

    void setProgress(ProgressBar* p){
        progressBar = p;
    }

signals:
    void startCopy();

    void checkPool();

public slots:

    void on_prefClosed();

    void cancelCopy();

    void onScanFinished();

    void onScanCanceled();

private slots:

    void on_Clear_clicked();

    void on_Copy_clicked();

    void on_actionInfo_triggered();

    void on_actionPreferences_triggered();

    void on_filterButton_clicked();

    void fileCopyDone(QString text);

    void copyThreadFinished();

    void showDropList();

    void on_Lista_clicked(const QModelIndex &index);

    void on_Lista_doubleClicked(const QModelIndex &index);

private:
    Ui::TvSeries *ui;
    void dropEvent(QDropEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
    QStringList *dropList;
    QThread* parallelCopy;
    CopyThread* copyThread;
    bool working;
    FilterDatabase* Database;
    bool removeOriginal;
    bool subFolderSearch;
    Preferences* prefWindow;
    ProgressBar* progressBar;
    QThreadPool* threadPool;
    FileList* fileList;
    bool scanCancel;
    PoolChecker* checker;
    QThread* checkerThread;

    ScanningWindow* scanWindow;

    QTreeView *treeView;
    QFileSystemModel *model;

    void loadDatabase();
    void savePrefs();
    void loadPrefs();

};

#endif // TVSERIES_H
