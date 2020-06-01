#include "Plate.hpp"


Plate::Plate(string file)
{
    // 10_209_210_4_29052020_162622_2_IPK5B50.jpg

    vector<string> ss = split(file, '_');

    this->file      = new string(file);
    this->plate     = new string(ss[7]);
    this->date      = new string(ss[4]);
    this->hour      = new string(ss[5]);
    this->number    = new int(stoi(ss[6]));

    dateAndHour.first = *(this->date);
    dateAndHour.second = *(this->hour);

    //cout << toString() << endl;
}

Plate::~Plate()
{
    delete file;
    delete plate;
    delete date;
    delete hour;
    delete number;
}

string  Plate::getFile()    {   return *file;   }
string  Plate::getPlate()   {   return *plate;  }
string  Plate::getDate()    {   return *date;   }
string  Plate::getHour()    {   return *hour;   }
int     Plate::getNumber()  {   return *number; }

vector<string> Plate::split(string file, char c)
{
    vector<string> s;
    string ss;
    for(int i=0; i<file.length(); i++)
    {
        if(file[i]!=c)  ss+=file[i];
        else
        {
            if(ss != "" and ss != " ") s.push_back(ss);
            ss.clear();
        }
    }
    s.push_back(ss);
    return s;
}

string Plate::toString()
{
    stringstream ss;
    ss << "File: " << *this->file << endl;
    ss << "Plate: " << *this->plate << endl;
    ss << "Date: " << *this->date << endl;
    ss << "Hour: " << *this->hour << endl;
    ss << "Number: " << *this->number;

    return ss.str();
}

pair< string, string >  Plate::getDateAndHour()
{
    return dateAndHour;
}

bool isValid(string plate)
{
    if(plate[0] == '0') return false;
    return true;
}

bool isLeapYear(int year)
{
    if(year % 400 == 0) return true;
    else
    {
        if(year % 4 == 0 and year % 100 != 0) return true;
    }
    return false;
}

bool  operator==(pair<string,string> dateAndHour_A, pair<string,string> dateAndHour_B)
{
    int yearA, yearB, monA, monB, dayA, dayB, hourA, hourB, minA, minB, secA, secB;
    stringstream  ssYearA, ssYearB, ssMonA, ssMonB, ssDayA, ssDayB, ssHourA, ssHourB, ssMinA, ssMinB, ssSecA, ssSecB;
    string sDateA, sHourA, sDateB, sHourB;
    try
    {
        sDateA = dateAndHour_A.first;
        sHourA = dateAndHour_A.second;
        sDateB = dateAndHour_B.first;
        sHourB = dateAndHour_B.second;    
    }
    catch(const std::exception& e)
    {
        //cout << "exception 1" << endl;
        return  false;
    }
    
    //01 23 4567
    //DD MM AAAA

    ssYearA << sDateA[4] << sDateA[5] << sDateA[6] << sDateA[7];
    ssMonA  << sDateA[2] << sDateA[3];
    ssDayA  << sDateA[0] << sDateA[1];

    ssHourA << sHourA[0] << sHourA[1];
    ssMinA  << sHourA[2] << sHourA[3];
    ssSecA  << sHourA[4] << sHourA[5];

    ssYearB << sDateB[4] << sDateB[5] << sDateB[6] << sDateB[7];
    ssMonB  << sDateB[2] << sDateB[3];
    ssDayB  << sDateB[0] << sDateB[1];

    ssHourB << sHourB[0] << sHourB[1];
    ssMinB  << sHourB[2] << sHourB[3];
    ssSecB  << sHourB[4] << sHourB[5];
    try
    {
        yearA = stoi(ssYearA.str());
        yearB = stoi(ssYearB.str());
        monA = stoi(ssMonA.str());
        monB = stoi(ssMonB.str());
        dayA = stoi(ssDayA.str());
        dayB = stoi(ssDayB.str());

        hourA = stoi(ssHourA.str());
        hourB = stoi(ssHourB.str());
        minA = stoi(ssMinA.str());
        minB = stoi(ssMinB.str());
        secA = stoi(ssSecA.str());
        secB = stoi(ssSecB.str());
    }
    catch(const std::exception& e)
    {
        //cout << "exception 2" << endl;
        return false;
    }
    


   //  cout << "A: " << dayA << "/" << monA << "/" << yearA << " " << hourA << ";" << minA << ";" << secA << endl;
   //  cout << "B: " << dayB << "/" << monB << "/" << yearB << " " << hourB << ";" << minB << ";" << secB << endl;

    int calendar[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if(secA == secB and minA == minB and hourA == hourB and dayA == dayB and monA == monB and yearA == yearB) return true;
    
    bool flag = 0; // 0 se A > B , 1 se B > A

    if(yearB > yearA) flag = true;
    else if(yearB < yearA) flag = false;
    else // yearA == yearB
    {
        //cout << "year iquals" << endl;
        if(monB > monA) flag = true;
        else if(monB < monA) flag = false;
        else // monA == monB
        {
            if(dayB > dayA) flag = true;
            else if(dayB < dayA) flag = false;
            else // dayA == dayB
            {
                if(hourB > hourA) flag = true;
                else if(hourB < hourA) flag = false;
                else // hourA == hourB
                {
                    if(minB > minA) flag = true;
                    else if(minB < minA) flag = false;
                    else // minA == minB
                    {
                        if(secB > secA) flag = true;
                        else if(secB < secA) flag = false;
                        else // secA == secB
                        {
                            cout << "Error 111" << endl;
                        }
                    }
                }
            }
        }      
    }

    // cout << (flag ? "B > A" : "A > B") << endl;

    int& _year = !flag ? yearA : yearB;
    int& _mon = !flag ? monA : monB;
    int& _day = !flag ? dayA : dayB;
    int& _hour = !flag ? hourA : hourB;
    int& _min = !flag ? minA : minB;
    int& _sec = !flag ? secA : secB;

    int& year_ = flag ? yearA : yearB;
    int& mon_ = flag ? monA : monB;
    int& day_ = flag ? dayA : dayB;
    int& hour_ = flag ? hourA : hourB;
    int& min_ = flag ? minA : minB;
    int& sec_ = flag ? secA : secB;
    


    if(--_sec == -1)
    {
        _sec = 59;
        if(--_min == -1)
        {
            _min = 59;
            if(--_hour == -1)
            {
                _hour = 23;
                if(--_day == 0)
                {
                    if(--_mon == 0)
                    {
                        _mon = 12;
                        if(isLeapYear(--_year)) calendar[1] = 29;
                    }
                    _day = calendar[_mon - 1];   
                }
            }
        }
    }

    if(_sec == sec_ and _min == min_ and _hour == hour_ and _day == day_ and _mon == mon_ and _year == year_) return true;
     // cout << "false 1" << endl;
    return false;

}

bool operator>(pair<string,string> dateAndHour_A, pair<string,string> dateAndHour_B)
{
    int yearA, yearB, monA, monB, dayA, dayB, hourA, hourB, minA, minB, secA, secB;
    stringstream  ssYearA, ssYearB, ssMonA, ssMonB, ssDayA, ssDayB, ssHourA, ssHourB, ssMinA, ssMinB, ssSecA, ssSecB;
    string sDateA, sHourA, sDateB, sHourB;
    try
    {
        sDateA = dateAndHour_A.first;
        sHourA = dateAndHour_A.second;
        sDateB = dateAndHour_B.first;
        sHourB = dateAndHour_B.second;    
    }
    catch(const std::exception& e)
    {
        return  false;
    }
    //01 23 4567
    //DD MM AAAA

    ssYearA << sDateA[4] << sDateA[5] << sDateA[6] << sDateA[7];
    ssMonA  << sDateA[2] << sDateA[3];
    ssDayA  << sDateA[0] << sDateA[1];

    ssHourA << sHourA[0] << sHourA[1];
    ssMinA  << sHourA[2] << sHourA[3];
    ssSecA  << sHourA[4] << sHourA[5];

    ssYearB << sDateB[4] << sDateB[5] << sDateB[6] << sDateB[7];
    ssMonB  << sDateB[2] << sDateB[3];
    ssDayB  << sDateB[0] << sDateB[1];

    ssHourB << sHourB[0] << sHourB[1];
    ssMinB  << sHourB[2] << sHourB[3];
    ssSecB  << sHourB[4] << sHourB[5];

    try
    {
        yearA = stoi(ssYearA.str());
        yearB = stoi(ssYearB.str());
        monA = stoi(ssMonA.str());
        monB = stoi(ssMonB.str());
        dayA = stoi(ssDayA.str());
        dayB = stoi(ssDayB.str());

        hourA = stoi(ssHourA.str());
        hourB = stoi(ssHourB.str());
        minA = stoi(ssMinA.str());
        minB = stoi(ssMinB.str());
        secA = stoi(ssSecA.str());
        secB = stoi(ssSecB.str());
    }
    catch(const std::exception& e)
    {
        return false;
    }
    


    if(yearA > yearB)       return true;
    else if(yearB > yearA)  return false;
    if(monA > monB)         return true;
    else if(monB > monA)    return false;
    if(dayA > dayB)         return true;
    else if(dayB > dayA)    return false;
    if(hourA > hourB)       return true;
    else if(hourB > hourA ) return false;
    if(minA > minB)         return true;
    else if(minB > minA)    return false;
    if(secA > secB)         return true;
                            return false;
}