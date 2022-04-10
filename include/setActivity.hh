#ifndef SETACTIVITY_HH
#define SETACTIVITY_HH

#include <ctime>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <utility>

class SourceActivity {
private:
    float activity, activity0 = 8200, halfLife = 5.27;
    double deltaTime = 0;
    void CalculateActivity();
    void GetActivityData();
    void Messanger();
    std::tm SetDate(std::string Date);
    void CalcDeltaTime(std::pair<std::tm, std::tm>);
public:
    SourceActivity();
    ~SourceActivity();
    float GetActivity() {return activity;};
};

#endif