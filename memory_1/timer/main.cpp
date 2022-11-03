#include "timer.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

void TestTimer() {
    Timer timer(1s);

    assert(!timer.Expired());

    std::this_thread::sleep_for(100ms);
    assert(!timer.Expired());

    std::this_thread::sleep_for(200ms);
    assert(!timer.Expired());

    std::this_thread::sleep_for(1s);
    assert(timer.Expired());

    std::this_thread::sleep_for(200ms);
    assert(timer.Expired());
}

void TestTimeMeasurer() {
    auto test = [](std::istream& stream, int64_t expected_ms) {
        std::string s;
        stream >> s >> s;
        int64_t real_ms;
        stream >> real_ms;
        assert(std::abs(real_ms - expected_ms) < 15);
    };

    std::stringstream stream;

    {
        TimeMeasurer measurer(stream);
        std::this_thread::sleep_for(150ms);
    }
    test(stream, 150);

    stream.str("");
    {
        TimeMeasurer measurer(stream);
        std::this_thread::sleep_for(703ms);
    }
    test(stream, 703);
}

int main() {
    TestTimer();
    TestTimeMeasurer();
    return 0;
}
