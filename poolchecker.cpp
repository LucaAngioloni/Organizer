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

#include "poolchecker.h"
#include <QTimer>
#include <QThreadPool>

PoolChecker::PoolChecker(QObject *parent) : QObject(parent)
{
    threadNum = 0;
    waitTime = 95;
}

PoolChecker::~PoolChecker()
{

}
int PoolChecker::getThreadNum() const
{
    return threadNum;
}

void PoolChecker::startChecking()
{
    threadNum = QThreadPool::globalInstance()->activeThreadCount();
    if(threadNum){
        if(waitTime < 2000){
             waitTime += 5;
        }
        QTimer::singleShot(waitTime, this, SLOT(startChecking()));
    }
    else{
        QThreadPool::globalInstance()->waitForDone();
        emit scanFinished();
    }
}


