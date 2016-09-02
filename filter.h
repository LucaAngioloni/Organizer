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

#ifndef FILTER_H
#define FILTER_H

#include <QString>
#include <vector>
#include <QFile>
#include <QDir>

class Filter
{
public:
    Filter(QString n = "No Name", QString p = "No Path", bool cs = false, int mind = 0, int maxd = -1, QString ext = "%Any%");
    ~Filter();

    bool isName(QString nameFile);

    void setName(QString n){
        Name = n;
    }
    QString getName(){
        return Name;
    }

    void setPath(QString p){
        Path = p;
    }

    QString getPath(){
        return Path;
    }

    void setMinDimension(int min){
        if(min >= 0){
            minDimension = min;
        }
        else {
            minDimension = 0;
        }
    }

    int getMinDimension(){
        return minDimension;
    }

    void setMaxDimension(int max, bool inf = false){
        if(inf == false){
            if(max > 0){
                maxDimension = max;
            }
            else {
                maxDimension = 1;
            }
        }
        else {
            maxDimension = -1;
        }
    }

    int getMaxDimension(){
        return maxDimension;
    }

    QString getWordAtIndex(int i){
        if(i<complexity())
            return words.at(i);
        else {
            return "%OutOfVector%";
        }
    }

    void setExtension(QString ext){
        if(ext == ""){
            this->extension = "%Any%";
        }
        else{
            this->extension = ext;
        }
    }

    QString getExtension(){
        return extension;
    }

    bool extensionMatch(QString ext);

    void addWord(QString word);
    bool removeWord(int position);
    void clearWords(){
        words.clear();
    }

    int complexity();                           //returns the number of words in the array

    void setCaseSensitivity(bool cs){
        caseSensitive = cs;
    }
    bool getCaseSensitivity(){
        return caseSensitive;
    }

    bool filterMatch(QString fileName);
    bool dimensionMatch(int dimension);

    void clearFilter();

    const QStringList& getList(){
        return words;
    }

    void setList(QStringList& list){
        words = list;
    }

private:
    QString Name;                               //name of the filter
    QStringList words;                          //vector of words that the file must contain to match the filter
    QString Path;                               //path where the files matching the fiter are copied
    int minDimension;                           //minimum dimension in MB of the file to match the filter
    int maxDimension;                           //maximum dimension in MB of the file to match the filter
    QString extension;                          //extension of the file
    bool caseSensitive;                         //if the matching of words in QStringList needs to be case sensitive or not
};

#endif // FILTER_H
