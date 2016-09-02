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

#include "scantask.h"
#include <exception>

ScanTask::ScanTask(FileList* l, bool sF, bool *sc)
{
    list = l;
    subFolder = sF;
    scanCanc = sc;
}

ScanTask::~ScanTask()
{

}

void ScanTask::run(){
    while(list->queueSize() != 0 && !(*scanCanc)){
        try{
            QString start = list->popQueue().remove(0,7);
            QDir folder;
            folder.setPath(start);
            folder.setFilter(QDir::Files);
            QStringList tmp = folder.entryList();
            for (int j = 0; j<tmp.size(); j++){
                tmp.replace(j,"file://" + start + tmp.at(j));
            }
            list->appendFile(tmp);
            tmp.clear();
            if(subFolder && !(*scanCanc)){
                folder.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
                tmp = folder.entryList();
                for(int i = 0; i < tmp.size(); i++){
                    tmp.replace(i,"file://" + start + tmp.at(i) + "/");
                }
                list->pushQueue(tmp);
                for(int j = QThreadPool::globalInstance()->activeThreadCount(); j<QThreadPool::globalInstance()->maxThreadCount(); j++){
                ScanTask* t = new ScanTask(list, subFolder, scanCanc);
                t->setAutoDelete(true);
                QThreadPool::globalInstance()->start(t);
                }
            }
        }
        catch(int e){}
    }
}
