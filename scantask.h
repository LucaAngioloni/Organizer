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

#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QDir>
#include <QRunnable>
#include <QThreadPool>
#include <filelist.h>

class ScanTask : public QRunnable
{
public:
    ScanTask(FileList* l, bool sF, bool* sc);
    ~ScanTask();

protected:
    void run();

private:
    FileList* list;
    bool subFolder;
    bool* scanCanc;
};

#endif // SCANTHREAD_H
