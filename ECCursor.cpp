#include "ECObserver.h"
#include "ECTextViewImp.h"
#include "ECTextController.h"

//Cursor class inherits from ECObserver
class ECCursor : public ECObserver
{
public:
    ECCursor(ECTextController *instance): instance(instance) {}
    virtual ~ECCursor() {}
    virtual void Update(){
        //Arrow left
        if(instance->GetKey()==ARROW_LEFT){
            //if cursor is not at beginning of row, move cursor left
            if(instance->GetCursorX()>0)
                instance->SetCursorX(instance->GetCursorX()-1);
            //if cursor is at beginning of row, move cursor up and to end of previous row
            if(instance->GetCursorX()==0 && instance->InWrappedView(instance->GetCursorY())){
                CursorUp();
                instance->SetCursorX(instance->GetColNumInView()-1);
            }
        }
        //Arrow right
        else if(instance->GetKey()==ARROW_RIGHT){
            //Calculate last nonwrapped row
            int lastNonwrapped=instance->GetCursorY();
            while(instance->InWrappedView(lastNonwrapped)){
                lastNonwrapped--;
            }
            //If cursor is not at end of row, move cursor right (Calculated by getting the length of the row minus minus the number of columns in view multiplied by number of times its wrapped)
            if(instance->GetCursorX()<instance->GetRowLength(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY()))-((instance->GetColNumInView()-1)*instance->NumWrappedRows(lastNonwrapped,instance->GetCursorY())))
                instance->SetCursorX(instance->GetCursorX()+1);
            //If cursor is at end of row, move cursor down and to beginning of next row
            if(instance->GetCursorX()==instance->GetColNumInView()-1 && instance->InWrappedView(instance->GetCursorY()+1)){
                CursorDown();
                instance->SetCursorX(0);
            }
        }

        else if(instance->GetKey()==ARROW_UP){
            CursorUp();
            //Call cursor up until cursor is not in wrapped view
            while(instance->InWrappedView(instance->GetCursorY())){
                CursorUp();
            }
        }
        else if(instance->GetKey()==ARROW_DOWN){
            CursorDown();
            //while there are still rows below and cursor is in wrapped view, call cursor down
            while(instance->GetCursorY()+instance->GetTopRow()+instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())<instance->GetNumRows() && instance->InWrappedView(instance->GetCursorY())){
                CursorDown();
            }
        } 
    }
    void CursorUp(){
        //If cursor is at top of view and there are still rows above, move view up
        if(instance->GetCursorY()==0 && instance->GetTopRow()>0){
            //Calculate number of rows to move view up by
            int increment=(instance->NumWrappedRows(instance->GetTopRow()-1,instance->GetBottomRow()-1)-instance->NumWrappedRows(instance->GetTopRow(),instance->GetBottomRow()))+1;
            instance->SetTopRow(instance->GetTopRow()-1);
            instance->SetBottomRow(instance->GetBottomRow()-increment);
            if(instance->GetCursorX()>instance->GetRowLength(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())))
                instance->SetCursorX(instance->GetRowLength(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())));
            }
        //If cursor is not at top of view, move cursor up
        else if(instance->GetCursorY()>0){
            instance->SetCursorY(instance->GetCursorY()-1);
            if(instance->GetCursorX()>instance->GetRowLength(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())))
                instance->SetCursorX(instance->GetRowLength(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())));
        }
    }

    void CursorDown(){
        //If cursor is at bottom of view and there are still rows below, move view down
        if((instance->GetBottomRow()<instance->GetNumRows())&&(instance->GetCursorY()==instance->GetRowNumInView()-1)){
            //Calculate number of rows to move view down by
            int increment=(instance->NumWrappedRows(instance->GetTopRow(),instance->GetBottomRow())-instance->NumWrappedRows(instance->GetTopRow()+1,instance->GetBottomRow()+1))+1;
            instance->SetTopRow(instance->GetTopRow()+1);
            instance->SetBottomRow(instance->GetBottomRow()+increment);
            // instance->ReevaluateBottomRow();
            // instance->SetCursorY(instance->GetBottomRow()-instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow()-1,instance->GetBottomRow()-1));
            }
        //If cursor is not at bottom of view, move cursor down
        else if((instance->GetCursorY()+instance->GetTopRow()<instance->GetNumRows()-1+instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())) && (instance->GetCursorY()<instance->GetBottomRow()-1+instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY()))){
            instance->SetCursorY(instance->GetCursorY()+1);
        }
        //If cursor is at bottom of view and there are no rows below, move cursor to end of last row
        if(instance->GetCursorX()>instance->GetRowLength(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())))
            instance->SetCursorX(instance->GetRowLength(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY())));
    }

private:
    ECTextController* instance;
};