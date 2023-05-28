#include "ECObserver.h"
#include "ECTextViewImp.h"
#include "ECTextController.h"

class ECMode : public ECObserver
{
public:
    ECMode(ECTextController *instance): instance(instance) {}
    virtual void Update(){
        //If in command mode, check for i key to enter insertion mode
        if(instance->GetCommandMode()){
            if(instance->GetKey()==105){
                instance->ClearStatusRows();
                instance->AddStatusRow("Editor Insertion Mode","For demo only",true);
                instance->SetCommandMode(false);
            }
        }
        //If in insertion mode, check for escape key to enter command mode
        else if(instance->GetKey()==27 || instance->GetKey()==CTRL_A){
            instance->SetCommandMode(true);
            instance->ClearStatusRows();
            instance->AddStatusRow("Editor","For demo only",true);
            instance->IncrementUndo();
        }
    }
private:
    ECTextController* instance;
};