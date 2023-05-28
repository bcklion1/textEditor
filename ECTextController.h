#ifndef ECTextViewController_h
#define ECTextViewController_h

#include "ECTextViewImp.h"
#include "ECTextModel.h"
#include "ECCommand.h"
#include <set>


//Controller focuses on updating the view and model and moving the cursor after updates
class ECTextController{
    public:
        ECTextController(ECTextModel* model, ECTextViewImp* view): model(model), view(view) {
            //initialize model with empty row
            if(model->GetNumRows()==0){
                model->AddRow("");
            }
            //initialize view size
            if(model->GetNumRows()<=view->GetRowNumInView()){
                bottomRow=model->GetNumRows();
            }
            else{
                bottomRow=view->GetRowNumInView();
            }
        };
        ~ECTextController();
        void AddText(std::string text,int row,int col);
        void AddChar(char text,int row,int col);
        void RemoveChar(int row,int col);
        void NewLine(int row,int col);
        void ClearView(){view->InitRows();}
        void ClearColor(){view->ClearColor();}
        int GetRowLength(int row){return model->GetRow(row).length();}
        int GetNumRows(){return model->GetNumRows();}
        int GetKey(){return view->GetPressedKey();}
        int GetCursorX(){return view->GetCursorX();}
        int GetCursorY(){return view->GetCursorY();}
        void SetCursorX(int x){view->SetCursorX(x);}
        void SetCursorY(int y){view->SetCursorY(y);}
        void UpdateView();
        int GetTopRow(){return topRow;}
        int GetBottomRow(){return bottomRow;}
        void SetTopRow(int row){topRow=row;}
        void SetBottomRow(int row){bottomRow=row;}
        int GetRowNumInView(){return view->GetRowNumInView();}
        bool GetCommandMode(){return commandMode;}
        void SetCommandMode(bool mode){commandMode=mode;}
        void AddStatusRow(std::string left,std::string right,bool background){view->AddStatusRow(left,right,background);}
        void ClearStatusRows(){view->ClearStatusRows();}
        void Undo();
        void Redo();
        void IncrementUndo(){invoker.incrementCommandIndex();}
        void ReevaluateBottomRow();
        void SetRow(std::string text,int row){model->SetRow(text,row);}
        void HighlightRow(int row);
        void HighlightText();
        void AddKeyword(std::string word){keywords.push_back(word);}
        int NumWrappedRows(int top,int bottom);
        bool InWrappedView(int row){return wrappedInView.find(row)!=wrappedInView.end();}
        int GetColNumInView(){return view->GetColNumInView();}
    private:
        ECTextModel* model; //model object
        ECTextViewImp* view;    //view object
        CommandInvoker invoker; //command invoker
        int topRow=0;   //top row of view
        int bottomRow;  //bottom row of view
        bool commandMode=true;  //mode
        std::vector<std::string> keywords;  //list of keywords
        std::vector<std::vector<std::pair<int,int>>> highlighted;   //location of highlighted text
        std::set<int> wrappedInView;    //rows that are wrapped in view
};

#endif