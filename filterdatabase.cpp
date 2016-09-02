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

#include "filterdatabase.h"

FilterDatabase::FilterDatabase()
{
}

QString FilterDatabase::getPath(QString nameofFile, int dimension){
    int comp = -1;
    QString path = "No Path";
    QFileInfo info(nameofFile);
    QString ext = info.suffix();
    for (int i = 0; i<fData.size(); i++){
        if (fData.at(i).filterMatch(nameofFile) && fData.at(i).dimensionMatch(dimension) && fData.at(i).extensionMatch(ext) && fData.at(i).complexity() > comp){
                path = fData.at(i).getPath();
                comp = fData.at(i).complexity();
        }
    }
    return path;
}

bool FilterDatabase::addFilter(Filter newf){
    if(newf.getName() == "" || nameIsOccupied(newf.getName())){
        return false;
    }
    else{
        fData.push_back(newf);
        return true;
    }
}

bool FilterDatabase::nameIsOccupied(QString nameCheck){
    unsigned i = 0;
    bool found = false;
    while (i<fData.size()) {
        if (fData.at(i).isName(nameCheck)){
            found = true;
            break;
        }
        i++;
    }

    return found;
}

void FilterDatabase::load(){
    QSettings database;
    database.beginGroup("Filters");
    database.beginGroup("Info");
    int size = database.value("Size").toInt();
    database.endGroup();
    QString Name, Path, Extension;
    QStringList List;
    int max, min;
    bool cs;
    for(int i = 0; i < size; i++){
        QString ID = "Filter" + QString::number(i);
        database.beginGroup(ID);
        Name = database.value("Name", "No Name").toString();
        Path = database.value("Path").toString();
        List = database.value("Words").toStringList();
        Extension = database.value("Extension").toString();
        min = database.value("Min Dimension").toInt();
        max = database.value("Max Dimension").toInt();
        cs = database.value("CaseSensitivity").toBool();
        database.endGroup();
        Filter loaded(Name,Path,cs,min,max,Extension);
        loaded.setList(List);
        addFilter(loaded);
    }
    database.endGroup();
}


void FilterDatabase::save(){
    QSettings database;
    database.beginGroup("Filters");
    database.beginGroup("Info");
    int s = size();
    database.setValue("Size", s);
    database.endGroup();
    for(int i = 0; i < size(); i++){
        QString ID = "Filter" + QString::number(i);
        database.beginGroup(ID);
        database.setValue("Name", getElement(i).getName());
        database.setValue("Path", getElement(i).getPath());
        database.setValue("Words", getElement(i).getList());
        database.setValue("Extension", getElement(i).getExtension());
        database.setValue("Min Dimension", getElement(i).getMinDimension());
        database.setValue("Max Dimension", getElement(i).getMaxDimension());
        database.setValue("CaseSensitivity", getElement(i).getCaseSensitivity());
        database.endGroup();
    }
    database.endGroup();
}

int FilterDatabase::size(){
    return fData.size();
}

void FilterDatabase::clear(){
    fData.clear();
}

void FilterDatabase::removeFilter(QString filterClicked){
    unsigned i = 0;
    bool found = false;
    while (i<fData.size()) {
        if (fData.at(i).isName(filterClicked)){
            found = true;
            break;
        }
        i++;
    }

    if (found == true){
        fData.erase(fData.begin() + i);
    }
}

Filter& FilterDatabase::getElement(QString nameOfFile){
    unsigned i = 0;
    bool found = false;
    while (i<fData.size()) {
        if (fData.at(i).isName(nameOfFile)){
            found = true;
            break;
        }
        i++;
    }

    if(found){
        return getElement(i);
    }
    else{
        notFound.clearFilter();
        return notFound;
    }
}
