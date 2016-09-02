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

#ifndef POOLCHECKER_H
#define POOLCHECKER_H

#include <QObject>

class PoolChecker : public QObject
{
    Q_OBJECT
public:
    explicit PoolChecker(QObject *parent = 0);
    ~PoolChecker();

    int getThreadNum() const;

signals:

    void scanFinished();

public slots:

    void startChecking();

private:
    int threadNum;
    int waitTime;
};

#endif // POOLCHECKER_H
