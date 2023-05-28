#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include "ECTextModel.h"
#include "ECTextViewImp.h"

// Command interface
class ECCommand {
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void Undo() = 0;
};

// Concrete Command class that modifies the Model
class ChangeTextCommand : public ECCommand {
public:
    ChangeTextCommand(ECTextModel* model, char text, int row, int col): model(model), text(text), row(row), col(col) {};
    void Execute() override;
    void Undo() override;
private:
    ECTextModel* model;
    int row;
    int col;
    char text;
};

class RemoveCharCommand : public ECCommand {
public:
    RemoveCharCommand(ECTextModel* model, int row, int col): model(model), row(row), col(col) {};
    void Execute() override;
    void Undo() override;
private:
    ECTextModel* model;
    int row;
    int col;
    char removedChar='\0';
};

class NewLineCommand : public ECCommand {
    public:
        NewLineCommand(ECTextModel* model, int row, int col): model(model), row(row), col(col) {};
        void Execute() override;
        void Undo() override;
    private:
        ECTextModel* model;
        int row;
        int col;
};


// Invoker class
class CommandInvoker {
public:
    void executeCommand(ECCommand* command) {
        command->Execute();
        tempCommands.push_back(command);
    }
    //undo's recent set of commands
    void undo() {
        if(m_executedCommands.empty()){
            return;
        }
        std::vector<ECCommand*> latest= m_executedCommands.top();
        m_executedCommands.pop();
        if (!latest.empty()) {
            for(int i=latest.size()-1;i>-1;i--){
                ECCommand* command = latest[i];
                command->Undo();
                tempCommands.push_back(command);
            }     
            m_undoneCommands.push(tempCommands);
            tempCommands.clear();    
        }
    }
    //redo's recent set of undone commands
    void redo() {
        if(m_undoneCommands.empty()){
            return;
        }
        std::vector<ECCommand*> latest= m_undoneCommands.top();
        m_undoneCommands.pop();
        if (!latest.empty()) {
            for(int i=latest.size()-1;i>-1;i--){
                ECCommand* command = latest[i];
                command->Execute();
                tempCommands.push_back(command);
            }        
            m_executedCommands.push(tempCommands);
            tempCommands.clear();
        }
    }

    //loads all commands done during editing to the stack
    void incrementCommandIndex() {
        if(tempCommands.empty()){
            return;
        }
        m_executedCommands.push(tempCommands);
        tempCommands.clear();
    }
private:
    std::stack<std::vector<ECCommand*>> m_executedCommands;
    std::stack<std::vector<ECCommand*>> m_undoneCommands;
    std::vector<ECCommand*> tempCommands;
};
