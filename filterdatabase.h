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

#ifndef FDATABASE_H
#define FDATABASE_H

#include <filter.h>
#include <vector>
#include <string>
#include <QString>
#include <QFile>
#include <fstream>
#include <QDir>
#include <QSettings>

class FilterDatabase
{
public:
    FilterDatabase();

    QString getPath(QString nameofFile, int dimension = 0);
    bool addFilter(Filter newf);
    int size();
    Filter& getElement(int i){
        if(i < this->size() && i >= 0){
            return fData.at(i);
        }
        else{
            notFound.clearFilter();
            return notFound;
        }
    }

    bool nameIsOccupied(QString nameCheck);

    Filter& getElement(QString nameOfFile);
    void clear();

    void removeFilter (QString filterClicked);

    void save ();
    void load ();
private:
    std::vector<Filter> fData;
    Filter notFound;
};

#endif // FDATABASE_H
