#ifndef ECTextModel_h
#define ECTextModel_h

#include <vector>
#include <string>

class ECTextModel{
    public:
        ECTextModel(){};
        void AddRow(std::string row);
        void AddRow(std::string row, int index);
        void RemoveRow(int index);
        void SetRow(std::string row, int index);
        std::string GetRow(int index);
        int GetNumRows();
    private:
        std::vector<std::string> text;  //represents the text as a vector of strings
};

#endif