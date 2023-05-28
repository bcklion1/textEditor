#include "ECObserver.h"
#include "ECTextViewImp.h"
#include "ECTextController.h"

class ECTextInterstion : public ECObserver
{
public:
    ECTextInterstion(ECTextController *instance): instance(instance) {}
    virtual void Update(){
        //only insert if not in command mode
        if(!instance->GetCommandMode()){
            //Character insertion
            if(instance->GetKey()>=32 && instance->GetKey()<=126){
                int lastNonwrapped=instance->GetCursorY();
                while(instance->InWrappedView(lastNonwrapped)){
                    lastNonwrapped--;
                }
                instance->AddChar(instance->GetKey(),instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY()),instance->GetCursorX()+((instance->GetColNumInView()-1)*instance->NumWrappedRows(lastNonwrapped,instance->GetCursorY())));
                instance->SetCursorX(instance->GetCursorX()+1);
            }
            //Newline insertion
            if(instance->GetKey()==13){
                int lastNonwrapped=instance->GetCursorY();
                while(instance->InWrappedView(lastNonwrapped)){
                    lastNonwrapped--;
                }
                instance->NewLine(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY()),instance->GetCursorX()+((instance->GetColNumInView()-1)*instance->NumWrappedRows(lastNonwrapped,instance->GetCursorY())));
            }
            //Backspace
            if(instance->GetKey()==127){
                int lastNonwrapped=instance->GetCursorY();
                while(instance->InWrappedView(lastNonwrapped)){
                    lastNonwrapped--;
                }
                instance->RemoveChar(instance->GetCursorY()+instance->GetTopRow()-instance->NumWrappedRows(instance->GetTopRow(),instance->GetCursorY()),instance->GetCursorX()+((instance->GetColNumInView()-1)*instance->NumWrappedRows(lastNonwrapped,instance->GetCursorY())));
            }

            

        }  
        //Command mode
        else{
            if(instance->GetKey()==CTRL_Z){
                instance->Undo();
            }
            else if(instance->GetKey()==CTRL_Y){
                instance->Redo();
            }
        }     
        //Update view after key press
        instance->UpdateView(); 
    }
private:
    ECTextController* instance;
};