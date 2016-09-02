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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = 0);
    ~Preferences();
    void setPointers(bool* r, bool* d);

signals:

    void Closed();

private slots:
    void on_DeleteOriginals_toggled(bool checked);

    void on_RecursiveSearch_toggled(bool checked);

    void on_OK_clicked();

    void on_Cancel_clicked();

private:
    Ui::Preferences *ui;
    bool* recursive;
    bool* deleteOriginals;

    bool rec;
    bool del;
};

#endif // PREFERENCES_H
