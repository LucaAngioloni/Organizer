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
#include <QUrl>
#include <QApplication>
#include <preferences.h>
#include <progressbar.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Luca Angioloni");
    QCoreApplication::setOrganizationDomain("lucaangioloni.com");
    QCoreApplication::setApplicationName("Organizer");

    Organizer mainWindow;
    Preferences pref;
    ProgressBar progress;

    mainWindow.setPrefs(&pref);
    mainWindow.setProgress(&progress);
    QObject::connect(&progress, SIGNAL(cancelSignal()), &mainWindow, SLOT(cancelCopy()));
    QObject::connect(&pref, SIGNAL(Closed()), &mainWindow, SLOT(on_prefClosed()));
    mainWindow.show();

    return a.exec();
}
