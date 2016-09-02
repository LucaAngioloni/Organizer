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

#include "filter.h"

Filter::Filter(QString n, QString p, bool cs, int mind, int maxd, QString ext) : Name(n), Path(p), caseSensitive(cs)
{
    bool infd = false;
    setMinDimension(mind);
    if(maxd == -1){
        infd = true;
    }
    setMaxDimension(maxd, infd);
    extension = ext;
}

Filter::~Filter(){

}

int Filter::complexity(){
    return words.size();
}

void Filter::addWord(QString word){
    if(word != "")
        words.push_back(word);
}

bool Filter::removeWord(int position){
    if (position < words.size() && position >= 0){
        words.erase(words.begin()+position);
        return true;
    }
    else {
        return false;
    }
}

bool Filter::dimensionMatch(int dimension){
    return dimension >= minDimension && (dimension <= maxDimension || maxDimension == -1);
}

bool Filter::filterMatch(QString fileName){
    Qt::CaseSensitivity cs;
    if(caseSensitive){
        cs = Qt::CaseSensitive;
    }
    else{
        cs = Qt::CaseInsensitive;
    }
    for (int i = 0; i < words.size(); i++){
        if(fileName.contains(words.at(i), cs) == false)
            return false;
    }
    return true;
}

bool Filter::isName(QString nameFile){
    if (nameFile == Name)
        return true;
    else
        return false;
}

bool Filter::extensionMatch(QString ext){
    if(extension != "%Image%" &&
       extension != "%Video%" &&
       extension != "%Music%" &&
       extension != "%Document%" &&
       extension != "%Book%" &&
       extension != "%Compressed%")
    {
        QFileInfo suff(extension);
        if (extension == "%Any%" || ext == suff.suffix()){
            return true;
        }
        else {
            return false;
        }
    }
    else{
        if(extension == "%Image%"){
            if(ext == "jpg" || ext == "jpeg" || ext == "tiff" || ext == "tif" || ext == "gif" || ext == "bmp" || ext =="png" || ext == "psd" || ext == "dng")
                return true;
            else
                return false;
        }
        if(extension == "%Video%"){
            if(ext == "3gp" || ext == "avi" || ext == "flv" || ext == "m4v" || ext == "mkv" || ext == "mov" || ext == "mp4" || ext == "mpg" || ext == "mpeg" || ext == "ogg" || ext == "webm" || ext == "wmv" || ext == "srt" || ext == "sub")
                return true;
            else
                return false;
        }
        if(extension == "%Music%"){
            if(ext == "aiff" || ext == "aac" || ext == "amr" || ext == "flac" || ext == "m4a" || ext == "m4p" || ext == "mp3" || ext == "ogg" || ext == "wav" || ext == "wma")
                return true;
            else
                return false;
        }
        if(extension == "%Document%"){
            if(ext == "doc" || ext == "docx" || ext == "pdf" || ext == "rtf" || ext == "txt" || ext == "odt")
                return true;
            else
                return false;
        }
        if(extension == "%Book%"){
            if(ext == "pdf" || ext == "rtf" || ext == "txt" || ext == "epub" || ext == "mobi" || ext == "lit")
                return true;
            else
                return false;
        }
        if(extension == "%Compressed%"){
            if(ext == "bz2" || ext == "gz" || ext == "lz" || ext == "lzma" || ext == "lzo" || ext == "7z" || ext == "alz" || ext == "s7z" || ext == "dmg" || ext == "rar" || ext == "tgz" || ext == "tbz2" || ext == "tlz" || ext == "zip")
                return true;
            else
                return false;
        }
    }
}


void Filter::clearFilter(){
    Name = "No Name";
    Path = "No Path";
    extension = "%Any%";
    clearWords();
    setMaxDimension(-1, true);
    setMinDimension(0);
    caseSensitive = false;
}
