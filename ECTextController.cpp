#include "ECTextController.h"
#include <iostream>

//add strings of text to model
void ECTextController::AddText(std::string text,int row,int col){
    if(row>=model->GetNumRows()){
        model->AddRow("");
    }
    std::string rowText=model->GetRow(row);
    rowText.insert(col,text);
    model->SetRow(rowText,row);
    UpdateView();
    ReevaluateBottomRow();
}

//add single character of text to model
void ECTextController::AddChar(char text,int row,int col){
    if(col%(GetColNumInView()-1)==0 && col!=0){
        view->SetCursorX(0);
        view->SetCursorY(GetCursorY()+1);
    }
    ECCommand *command = new ChangeTextCommand(model,text,row,col);
    invoker.executeCommand(command);
}

//remove character or row from model
void ECTextController::RemoveChar(int row,int col){
    if((row>0 && col==0)||(col%GetColNumInView()==0 && col!=0)){
        if(InWrappedView(GetCursorY())){
            view->SetCursorX(GetColNumInView()-1);
            view->SetCursorY(GetCursorY()-1);
            bottomRow++;
        }
        else{
            view->SetCursorX(model->GetRow(row-1).length());
            view->SetCursorY(GetCursorY()-1);
            if(bottomRow==model->GetNumRows()+NumWrappedRows(topRow,bottomRow-1)){
                bottomRow--;
            }
            if(topRow>0){
                view->SetCursorY(row-topRow);
                topRow--;
            }
        }
    }  
    else if(row>=0 && col>0){
        view->SetCursorX(GetCursorX()-1);
    }
    if(col==0 && row==0){
        return;
    }
    ECCommand *command = new RemoveCharCommand(model,row,col);
    invoker.executeCommand(command);
    ReevaluateBottomRow();
}

//new line command
void ECTextController::NewLine(int row,int col){
    ECCommand *command = new NewLineCommand(model,row,col);
    invoker.executeCommand(command);
    bottomRow++;
    //Move cursor if screen not filled
    if(view->GetRowNumInView()>=topRow+bottomRow+NumWrappedRows(topRow,bottomRow)){
        SetCursorY(GetCursorY()+1);
    }
    //scroll screen otherwise
    if(view->GetRowNumInView()<bottomRow-topRow+NumWrappedRows(topRow,bottomRow)){
        topRow++;
    }
    view->SetCursorX(0);
}

//refesh view
void ECTextController::UpdateView(){

    view->InitRows();
    int offset = 0;
    wrappedInView.clear();
    for(int i=topRow; i<bottomRow-offset; i++){
        std::vector<std::string> substrings;
        int start = 0;
        //splits row into substrings of length of columns in view
        while (start < model->GetRow(i).length()) {
            std::string substr = model->GetRow(i).substr(start, view->GetColNumInView()-1);
            substrings.push_back(substr);
            start += view->GetColNumInView()-1;
        }
        if(substrings.size()==0){
            substrings.push_back("");
        }
        int j=0;
        offset--;
        //adds substrings to view offset increments if row is wrapped
        while (j<substrings.size() && bottomRow-offset>0) {
            
            view->AddRow(substrings[j]);
            if(j>0){
                wrappedInView.insert(i+j+offset);
                if(bottomRow<view->GetRowNumInView()){
                    offset--;
                }
                
            }
            j++;
            offset++;
        }
        
    }


    // view->ClearStatusRows();
    // std::string right="Num Col"+std::to_string(GetColNumInView())+" X:"+std::to_string(GetCursorX())+" Y:"+ std::to_string(GetCursorY())+" Num Rows:"+std::to_string(GetNumRows())+" Rows in view:"+std::to_string(GetRowNumInView())+" Bot:"+std::to_string(GetBottomRow())+" Top:"+std::to_string(GetTopRow())+" Wrapped:";
    // for(auto i:wrappedInView){
    //     right+=std::to_string(i)+",";
    // }
    // right+=" NumWrapped:"+std::to_string(NumWrappedRows(GetTopRow(),GetCursorY()));
    // view->AddStatusRow("Command Mode",right,true);

    //highlighting text from keywords
    view->ClearColor();
    HighlightText();
    std::cout<<"Z"<<std::endl;
    for(int i = topRow;i<bottomRow;i++){
        for(auto j:highlighted[i]){
            view->SetColor(i-topRow+NumWrappedRows(topRow,i),j.first,j.second-1,TEXT_COLOR_BLUE);
        }
    }
    
}


//Undo commands
void ECTextController::Undo(){
    invoker.undo();
    //puts cursor in correct position after undo
    if(GetCursorY()+topRow>GetNumRows()-1){
        SetBottomRow(GetNumRows());
        (GetBottomRow()-GetRowNumInView()>0)?SetTopRow(GetBottomRow()-GetRowNumInView()):SetTopRow(0);
        SetCursorY(bottomRow-topRow-1);
        SetCursorX(GetRowLength(GetCursorY()+topRow));
    }
    else if(GetCursorX()>GetRowLength(GetCursorY()+topRow)){
        SetCursorX(GetRowLength(GetCursorY()+topRow-NumWrappedRows(topRow,GetCursorY())));
    }
    
    ReevaluateBottomRow();
}

//Redo commands
void ECTextController::Redo(){
    invoker.redo();
    if(GetCursorY()+topRow>GetNumRows()-1){
        SetBottomRow(GetNumRows());
        (GetBottomRow()-GetRowNumInView()>0)?SetTopRow(GetBottomRow()-GetRowNumInView()):SetTopRow(0);
        SetCursorY(bottomRow-topRow-1);
        SetCursorX(GetRowLength(GetCursorY()+topRow));
    }
    else if(GetCursorX()>GetRowLength(GetCursorY()+topRow)){
        SetCursorX(GetRowLength(GetCursorY()+topRow));
    }
    ReevaluateBottomRow();
}

//Finds valid bottom row
void ECTextController::ReevaluateBottomRow(){
    if(model->GetNumRows()+NumWrappedRows(topRow,bottomRow)<=view->GetRowNumInView()){
        bottomRow=model->GetNumRows();
    }
}

void::ECTextController::HighlightRow(int row){
    highlighted[row].clear();
    for(auto search:keywords){
        size_t word = model->GetRow(row).find(search);
        while(word!=std::string::npos){
            std::pair<int,int> temp=std::make_pair(word,word+search.length());
            highlighted[row].push_back(temp);
            word = model->GetRow(row).find(search,word+search.length()-1);
        }
    }   
}

void ECTextController::HighlightText(){
    highlighted.clear();
    for(int i=0;i<model->GetNumRows();i++){
        highlighted.push_back(std::vector<std::pair<int,int>>());
        HighlightRow(i);
    }
}

int ECTextController::NumWrappedRows(int top,int bottom){
    int numWrappedRows=0;
    // std::cout<<model->GetNumRows()<<std::endl;
    for(int i=top;i<bottom;i++){
        numWrappedRows+=((model->GetRow(i).length() ) / view->GetColNumInView());
    }
    // std::cout<<"x"<<std::endl;

    return numWrappedRows;
}