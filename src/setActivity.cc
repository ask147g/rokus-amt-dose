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

void SourceActivity::Messenger() {
    std::cout << "Activity: " << activity << " Bq" << std::endl;
}

void SourceActivity::CalculateActivity() {
    activity = activity0 * exp(-log(2)/halfLife*deltaTime); // Ci
    activity *= 3.7*pow(10,10); // Bq
    Messenger();
}

std::tm SourceActivity::SetDate(std::string Date) {
    std::vector<std::string> strs;
    strs = split(Date, '.');
    std::tm date;
    date = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    date.tm_mday = std::stoi(strs[0]);
    date.tm_mon = std::stoi(strs[1]);
    date.tm_year = std::stoi(strs[2]);
    return date;
}

void SourceActivity::CalcDeltaTime(std::pair<std::tm, std::tm> date) {
    deltaTime = (std::difftime(std::mktime(&(date.second)), std::mktime(&(date.first))))/60/60/24/365;
}

void SourceActivity::GetActivityData() {
    rapidxml::xml_document<> doc;
	rapidxml::xml_node<> * root_node;

    std::ifstream theFile ("data/sourceData.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    root_node = doc.first_node("activity0");
    activity0 = std::stof(root_node->value());
    root_node = doc.first_node("half-life");
    halfLife = std::stof(root_node->value());
    std::string initDate, lastDate;
    root_node = doc.first_node("date0");
    initDate = root_node->value();
    root_node = doc.first_node("date");
    lastDate = root_node->value();
    std::tm inDate = SetDate(initDate);
    std::tm lasDate = SetDate(lastDate);
    CalcDeltaTime(std::pair<std::tm,std::tm> (inDate, lasDate));
    root_node = doc.first_node("increasingTimes");
    decreasingActivity = std::stof(root_node->value());
    root_node = doc.first_node("time");
    timeModel = std::stof(root_node->value());


    CalculateActivity();
}