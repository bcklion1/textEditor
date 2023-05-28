#include "ECTextModel.h"
#include <iostream>

//Add a row to the end of the text
void ECTextModel::AddRow(std::string row){
    text.push_back(row);
}

//Add a row at a specific index
void ECTextModel::AddRow(std::string row, int index){
    if(index>text.size()){
        text.push_back(row);
    }
    else if(index<0){
        text.insert(text.begin(), row);
    }
    else{
        text.insert(text.begin()+index, row);
    }
}

//Remove a row at a specific index
void ECTextModel::RemoveRow(int index){
    text.erase(text.begin()+index);
}

//Set a row at a specific index
void ECTextModel::SetRow(std::string row, int index){
    text[index]=row;
}

//Get a row at a specific index
std::string ECTextModel::GetRow(int index){
    return text[index];
}

//Get the number of rows in the text
int ECTextModel::GetNumRows(){
    return text.size();
}
