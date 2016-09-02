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

#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QDialog>
#include <vector>
#include <QMessageBox>
#include <string>
#include <QUrl>
#include <filterdatabase.h>
#include <QDialog>
#include <QFileDialog>
#include <filter.h>
#include <QCompleter>
#include <QDirModel>

namespace Ui {
class FilterWindow;
}

class FilterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FilterWindow(QWidget *parent = 0);
    ~FilterWindow();

    void setDatabase(FilterDatabase* data);
    void fillInfo(QString filterName);
    FilterDatabase* database;
    void setDefault();

    int getMinDimensionMB();
    int getMaxDimensionMB();

    void setMinDimensionFromMB(int minMB);
    void setMaxDimensionFromMB(int maxMB);

    QString getExt();

    void newFilter();
    void modFilter();

private slots:

    void on_browseButton_clicked();

    void on_cancelButton_clicked();

    void on_okButton_clicked();

    void on_extensionBox_currentIndexChanged(const QString &arg1);

    void on_plusButton_clicked();

    void on_minSlider_valueChanged(int value);

    void on_maxSlider_valueChanged(int value);

    void on_minusButton_clicked();

    void on_caseSensitivityBox_clicked(bool checked);

signals:
    void filterDone();

private:
    Ui::FilterWindow *ui;
    bool addOrMod;                      //0 if Add Filter, 1 if Modify Filter
    QString currentFilter;
    QString ext;
    QCompleter *dirCompleter;
    bool caseSens;

    void updateMBLabel();
};

#endif // FILTERWINDOW_H
