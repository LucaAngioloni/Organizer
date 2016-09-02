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

#include "filelist.h"

#include <QFileInfo>
#include <exception>

FileList::FileList()
{
    stringList = new QStringList();
    queueList = new QStringList();
    dimension = 0;
}

FileList::~FileList()
{
    delete stringList;
    delete queueList;
}

void FileList::appendFile(QString str)
{
    mutex.lock();
    stringList->append(str);
    mutex.unlock();
    str = str.remove(0,7);
    QFileInfo info(str);
    dimMutex.lock();
    dimension = dimension + (float)info.size()/1000000;
    dimMutex.unlock();
}

void FileList::appendFile(QStringList l)
{
    mutex.lock();
    stringList->append(l);
    mutex.unlock();
    QString str;
    for(int i = 0; i< l.size(); i++){
        str = l.at(i);
        str = str.remove(0,7);
        QFileInfo info(str);
        dimMutex.lock();
        dimension = dimension + (float)info.size()/1000000;
        dimMutex.unlock();
    }
}

QString FileList::fileAt(int i)
{
    QString r;
    mutex.lock();
    r = stringList->at(i);
    mutex.unlock();
    return r;
}

bool FileList::removeFileAtIndex(int i)
{
    if(i<this->size()){
    mutex.lock();
    QString str = stringList->at(i);
    str = str.remove(0,7);
    QFileInfo info(str);
    dimension = dimension - (float)info.size()/1000000;
    stringList->erase(stringList->begin() + i);
    mutex.unlock();
    return true;
    }
    else{
        return false;
    }
}

void FileList::clear()
{
    mutex.lock();
    stringList->clear();
    queueMutex.lock();
    queueList->clear();
    queueMutex.unlock();
    dimension = 0;
    mutex.unlock();
}

int FileList::size()
{
    int i = 0;
    mutex.lock();
    i = stringList->size();
    mutex.unlock();
    return i;
}

void FileList::pushQueue(QString str)
{
    queueMutex.lock();
    queueList->append(str);
    queueMutex.unlock();
}

void FileList::pushQueue(QStringList l)
{
    queueMutex.lock();
    queueList->append(l);
    queueMutex.unlock();
}

QString FileList::popQueue()
{
    QString pop;
    queueMutex.lock();
    if(!queueList->empty()){
    pop = queueList->at(0);
    }
    else{
        throw 11;
    }
    queueList->removeFirst();
    queueMutex.unlock();
    return pop;
}

int FileList::queueSize()
{
    int size = 0;
    queueMutex.lock();
    size = queueList->size();
    queueMutex.unlock();
    return size;
}

float FileList::getDimension() const
{
    return dimension;
}


