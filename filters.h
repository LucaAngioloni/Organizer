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

#ifndef FILTERS_H
#define FILTERS_H

#include <filterdatabase.h>
#include <QDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QDropEvent>
#include <QUrl>
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFile>
#include <vector>
#include <QMessageBox>
#include <string>
#include <filter.h>
#include <filterwindow.h>

namespace Ui {
class Filters;
}

class Filters : public QDialog
{
    Q_OBJECT

public:
    Filters(QWidget *parent = 0);
    ~Filters();

    void setDatabase(FilterDatabase* data);
    void fillTable();
    FilterDatabase* database;

private slots:

    void on_addFilter_clicked();

    void on_ClearButton_clicked();

    void on_tableOfFilters_cellClicked(int row, int column);

    void on_tableOfFilters_cellActivated(int row, int column);

    void on_RemoveButton_clicked();

    void on_tableOfFilters_cellDoubleClicked(int row, int column);

    void on_Filters_accepted();

    void fWindowDone();

private:

    Ui::Filters *ui;
    QString filterClicked;
    FilterWindow fWindow;
};

#endif // FILTERS_H
