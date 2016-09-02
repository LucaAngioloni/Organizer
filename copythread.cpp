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

#include "copythread.h"

CopyThread::CopyThread(QObject *parent) : QObject(parent)
{
    removeOriginal = false;
}

CopyThread::~CopyThread()
{

}

void CopyThread::setPointers(QStringList* s, FilterDatabase* d, bool r){
    stringListPath = s;
    Database = d;
    removeOriginal = r;
}


void CopyThread::copyStart(){
    QDir pd;
    for (int i = 0; i<stringListPath->size(); i++){
        QString nameOfFile, pathDest, sourcePath, fileDest;
        sourcePath = stringListPath->at(i);
        sourcePath = sourcePath.remove(0,7);
        QFileInfo info(sourcePath);
        int size = info.size()/1000000;
        nameOfFile = info.fileName();
        pathDest = Database->getPath(nameOfFile, size);
        fileDest = pathDest + "/" + nameOfFile;

        pd.setPath(pathDest);
        info.setFile(pathDest);

        if (pathDest != "No Path" && pd.exists()){
            if(info.isWritable()){
                if(QFile::exists(fileDest)){
                    emit fileElab(nameOfFile + " already exists!\n");
                }
                else{
                    if(QFile::exists(sourcePath)){
                        copy(sourcePath, fileDest);
                        emit fileElab(nameOfFile + " copied!\n");
                        if (removeOriginal == true){
                            QFile::remove(sourcePath);
                        }
                    }
                    else{
                        emit fileElab(nameOfFile + " was moved or deleted!\n");
                    }
                }
            }
            else{
                emit fileElab("Destination Folder has Write Protection");
            }
        }
        else{
            emit fileElab("No path where to copy " + nameOfFile + "\n");
        }
    }
    emit copyFinished();
}


void CopyThread::copy(QString source, QString dest){
    QFile::copy(source, dest);
    while (QFile::exists(dest) == false) {
    }
}
