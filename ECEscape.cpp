
#include "ECObserver.h"
#include "ECTextViewImp.h"

class ECEscape : public ECObserver
{
public:
    ECEscape(ECTextViewImp *instance): instance(instance) {}
    virtual ~ECEscape() {}
    virtual void Update(){
        if(instance->GetPressedKey()==ESC){
            instance->InitRows();
            instance->ClearColor();
            instance->AddRow("Brian Kunak");
        }
    }
private:
    ECTextViewImp* instance;
};