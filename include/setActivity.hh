#ifndef setActivity_h
#define setActivity_h 1

#include <ctime>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <utility>

#include <rapidxml/rapidxml.hpp>

class SourceActivity {
private:
    float activity, activity0 = 8200, halfLife = 5.27;
    double deltaTime = 0;
    void CalculateActivity();
    void GetActivityData();
    void Messenger();
    std::tm SetDate(std::string Date);
    void CalcDeltaTime(std::pair<std::tm, std::tm> date);
    double decreasingActivity = 1;
    double timeModel = 1;
public:
    SourceActivity();
    ~SourceActivity();
    float GetActivity() {return activity;};
    double GetIncreasing() {return decreasingActivity;};
    double GetTime() {return timeModel;};
};

#endif