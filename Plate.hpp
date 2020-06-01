#ifndef Path_hpp
#define Path_hpp

#include <iostream>
#include <sstream>
#include <vector>


using namespace std;

class Plate
{
    private:
        string*  file;
        string*  plate;
        string*  date;
        string*  hour;
        int*     number;
        pair<string,string> dateAndHour;
        vector<string> split(string file, char c);
    public:
                                Plate(string file);
                                ~Plate();

        string                  getFile();
        string                  getPlate();
        string                  getDate();
        string                  getHour();
        int                     getNumber();
        pair< string, string >  getDateAndHour();

        string                  toString();
};

bool isValid(string plate);
bool isLeapYear(int year);

bool operator==(pair<string,string> dateAndHour_A, pair<string,string> dateAndHour_B);
bool operator>(pair<string,string> dateAndHour_A, pair<string,string> dateAndHour_B);

#endif