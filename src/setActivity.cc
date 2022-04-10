#include "setActivity.hh"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

SourceActivity::SourceActivity() {GetActivityData();}

SourceActivity::~SourceActivity() {}

void SourceActivity::Messanger() {
    std::cout << "Activity: " << activity << " Bq" << std::endl;
}

void SourceActivity::CalculateActivity() {
    activity = activity0 * exp(-log(2)/halfLife*deltaTime); // Ci
    activity *= 3.7*pow(10,10); // Bq
    Messanger();
}

std::tm SourceActivity::SetDate(std::string Date) {
    std::vector<std::string> strs;
    strs = split(Date, '.');
    std::tm date;
    date = {0};
    date.tm_mday = std::stoi(strs[0]);
    date.tm_mon = std::stoi(strs[1]);
    date.tm_year = std::stoi(strs[2]);
    return date;
}

void SourceActivity::CalcDeltaTime(std::pair<std::tm, std::tm> date) {
    deltaTime = (std::difftime(std::mktime(&(date.second)), std::mktime(&(date.first))))/60/60/24/365;
}

void SourceActivity::GetActivityData() {
    std::ifstream inActivity("data/sourceData.txt", std::ios_base::in);
    if (inActivity.is_open()) {
        std::string act; std::getline(inActivity,act);
        activity0 = std::stof(act);
        std::getline(inActivity,act); halfLife = std::stof(act);
        std::string initDate, lastDate;
        std::getline(inActivity,initDate);
        std::getline(inActivity,lastDate);
        std::tm inDate = SetDate(initDate);
        std::tm lasDate = SetDate(lastDate);
        CalcDeltaTime(std::pair<std::tm,std::tm> (inDate, lasDate));
    }
    inActivity.close();
    CalculateActivity();
}