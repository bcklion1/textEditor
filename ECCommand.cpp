#include "ECCommand.h"

//Command Design function implimentation

//Adding one character
void ChangeTextCommand::Execute(){
    if(row>=model->GetNumRows()){
        model->AddRow("");
    }
    std::string rowText=model->GetRow(row);
    rowText.insert(col,1,text);
    model->SetRow(rowText,row);
}

//Undo of adding a character
void ChangeTextCommand::Undo(){
    std::string rowText=model->GetRow(row);
    rowText.erase(col,1);
    model->SetRow(rowText,row);
}

//Removing a character
void RemoveCharCommand::Execute(){
    std::string rowText=model->GetRow(row);
    //if empty row, remove row
    if(rowText==""){
        if(row<model->GetNumRows()){
            model->RemoveRow(row);
        }
    }
    //if not empty row, remove character
    else if(col>0)
    {
        removedChar=rowText[col-1];
        rowText.erase(col-1,1);
        model->SetRow(rowText,row);
    }
    //if at beginning of row, remove row and append to previous row and scroll up
    else if(row>0){
        std::string temp=model->GetRow(row-1);
        col=temp.length();
        temp+=rowText;
        model->RemoveRow(row);
        model->SetRow(temp,row-1);
        row=row-1;
        
    }
    
}

//Undo of removing a character
void RemoveCharCommand::Undo(){
    //if empty row, add row
    if (removedChar=='\0'){
        std::string rowText=model->GetRow(row);
        std::string newRow=rowText.substr(col);
        rowText.erase(col);
        model->SetRow(rowText,row);
        model->AddRow(newRow,row+1);
        row++;
        col=0;
    }
    //if not empty row, add character
    else{
        std::string rowText=model->GetRow(row);
        rowText.insert(col-1,1,removedChar);
        model->SetRow(rowText,row);
    }
}

//New line
void NewLineCommand::Execute(){
    if(col>=model->GetRow(row).length()){
        model->AddRow("",row+1);
        return;
    }
    std::string rowText=model->GetRow(row);
    std::string newRow=rowText.substr(col);
    rowText.erase(col);
    model->SetRow(rowText,row);
    model->AddRow(newRow,row+1);
}

//Undo of new line
void NewLineCommand::Undo(){
    std::string rowText=model->GetRow(row);
    rowText+=model->GetRow(row+1);
    model->RemoveRow(row+1);
    model->SetRow(rowText,row);
}

