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

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QApplication>
#include <QDialog>

namespace Ui {
class ProgressBar;
}

class ProgressBar : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressBar(QWidget *parent = 0);
    ~ProgressBar();

    void setNumber(int n);

signals:

    void cancelSignal();

private slots:
    void fileDone();
    void on_progressBar_valueChanged(int value);

    void on_pushButton_clicked();

private:
    Ui::ProgressBar *ui;
    int number;
    int progress;
    int percentage;
};

#endif // PROGRESSBAR_H
