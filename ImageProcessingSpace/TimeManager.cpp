#include "TimeManager.h"

TimeManager::TimeManager() {
    this->start = std::chrono::high_resolution_clock::now();
    this->end = this->start;
}

void TimeManager::markTime() {
    this->start = std::chrono::high_resolution_clock::now();
    this->end = this->start;
}

void TimeManager::printTimeSinceLastMark() {
    this->end = std::chrono::high_resolution_clock::now();
    std::cout
        << "Time since last mark: "
        << this->getTimeBetweenString(this->end, this->start)
    << std::endl;
}

void TimeManager::printTimeSinceStart() {
    this->end = std::chrono::high_resolution_clock::now();
    std::cout
        << "Time since program start: "
        << this->getTimeBetweenString(this->end, this->startOfCode)
    << std::endl;
}

std::string TimeManager::getTimeBetweenString(std::chrono::steady_clock::time_point end, std::chrono::steady_clock::time_point start) {
    const auto between = end - start;
    std::string hours = std::to_string(
        std::chrono::duration_cast<std::chrono::hours>(between).count() % 24
    );
    std::string minutes = std::to_string(
        std::chrono::duration_cast<std::chrono::minutes>(between).count() % 60
    );
    std::string seconds = std::to_string(
        std::chrono::duration_cast<std::chrono::seconds>(between).count() % 60
    );
    std::string millis = std::to_string(
        std::chrono::duration_cast<std::chrono::milliseconds>(between).count() % 1000
    );
    std::string micros = std::to_string(
        std::chrono::duration_cast<std::chrono::microseconds>(between).count() % 1000
    );
    TimeManager::padLeft(hours, 2, '0');
    TimeManager::padLeft(minutes, 2, '0');
    TimeManager::padLeft(seconds, 2, '0');
    TimeManager::padLeft(millis, 3, '0');
    TimeManager::padLeft(micros, 3, '0');
    return hours + ":" + minutes + ":" + seconds + "." + millis + micros;
}

TimeManager::~TimeManager() {}
