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

#ifndef FILELIST_H
#define FILELIST_H


#include <QString>
#include <QStringList>
#include <QMutex>

class FileList
{
public:
    FileList();
    ~FileList();

    void appendFile(QString str);
    void appendFile(QStringList l);
    QString fileAt(int i);
    bool removeFileAtIndex(int i);
    void clear();
    int size();

    void pushQueue(QString str);
    void pushQueue(QStringList l);
    QString popQueue();
    int queueSize();

    QStringList* getList(){
        return stringList;
    }

    QStringList* getQueue(){
        return queueList;
    }

    float getDimension() const;

private:
    QStringList* stringList;
    QStringList* queueList;
    float dimension;
    QMutex mutex;
    QMutex dimMutex;
    QMutex queueMutex;
};

#endif // FILELIST_H
