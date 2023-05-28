// Test code for editor
#include "ECTextViewImp.h"
#include <iostream>
#include "ECEscape.cpp"
#include "ECCursor.cpp"
#include "ECTextInsertion.cpp"
#include "ECMode.cpp"
#include <fstream>
#include <vector>

using namespace  std;

int myCounter = 0;

int main(int argc, char *argv[])
{
    //
    ECTextViewImp wndTest;
    ECTextModel *model=new ECTextModel();
    ECTextController *controller=new ECTextController(model,&wndTest);

    // add a status bar
    wndTest.AddStatusRow("Editor", "For demo only", true);
    

    //Add text from file
    std::ifstream file(argv[1]);
    if (file.is_open()) {
        std::string line;
        int row=0;
        while (std::getline(file, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }
            controller->AddText(line,row,0);
            row++;
        }
        file.close();
    }
    
    //Add Keywords
    std::ifstream keywords("keywords.txt");
    if (keywords.is_open()) {
        std::string line;
        while (std::getline(keywords, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }
            controller->AddKeyword(line);
        }
        keywords.close();
    }


    //refresh the view
    controller->UpdateView();

    //mount cursor
    ECCursor *cursor=new ECCursor(controller);
    wndTest.Attach(cursor);
    //mount insertion
    ECTextInterstion *insertion=new ECTextInterstion(controller);
    wndTest.Attach(insertion);
    //mount mode control
    ECMode *mode=new ECMode(controller);
    wndTest.Attach(mode);
    wndTest.Show();
    
    return 0;
}
