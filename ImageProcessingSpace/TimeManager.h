#pragma once

#include <chrono>
#include <iostream>

class TimeManager {
	static void padLeft(std::string& str, const size_t num, const char paddingChar = '0') {
		if (num > str.size()) str.insert(0, num - str.size(), paddingChar);
	}
	private:
		const std::chrono::steady_clock::time_point startOfCode = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point end;
		std::string getTimeBetweenString(std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point);
	public:
		TimeManager();
		~TimeManager();
		void markTime();
		void printTimeSinceStart();
		void printTimeSinceLastMark();
};

