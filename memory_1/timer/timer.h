#include <cassert>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>
#include <thread>
#include <iostream>

class Timer {
public:
    std::chrono::time_point<std::chrono::steady_clock> begin;
    std::chrono::duration<double> dur;
    Timer(std::chrono::duration<double> time){
        begin = std::chrono::steady_clock::now();
        dur = time;
    }

    bool Expired() const{
        if (std::chrono::steady_clock::now() - begin <= dur) {
            return false;
        }
        return true;
    };
};

class TimeMeasurer {
public:
    std::iostream& outstream;
    std::chrono::time_point<std::chrono::steady_clock> beg;
    TimeMeasurer(std::iostream& output): outstream(output), beg(std::chrono::steady_clock::now()) {}
    ~TimeMeasurer(){
        outstream << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::steady_clock::now() - beg)).count() << std::endl;
    }

};
