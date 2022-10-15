#pragma once

#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <windows.h>
#include <filesystem>

//#include "ImageProcessingAlgorithms.h"

using namespace std;
/*
bool handleViewSelectableImages();
bool handleRemoveColor();
bool handleHideString();
bool handleFindStringInImage();
bool handleRotate();

void padLeft(string& str, const size_t num, const char paddingChar = '0') {
    if (num > str.size()) str.insert(0, num - str.size(), paddingChar);
}

bool markTime(bool print = false) {
    static auto start = chrono::high_resolution_clock::now();
    static auto end = chrono::high_resolution_clock::now();
    static bool first = true;
    end = chrono::high_resolution_clock::now();
    if (print && !first) {
        auto between = end - start;
        string hours = to_string(chrono::duration_cast<chrono::hours>(between).count() % 24);
        string minutes = to_string(chrono::duration_cast<chrono::minutes>(between).count() % 60);
        string seconds = to_string(chrono::duration_cast<chrono::seconds>(between).count() % 60);
        string millis = to_string(chrono::duration_cast<chrono::milliseconds>(between).count() % 1000);
        string micros = to_string(chrono::duration_cast<chrono::microseconds>(between).count() % 1000);
        padLeft(hours, 2, '0');
        padLeft(minutes, 2, '0');
        padLeft(seconds, 2, '0');
        padLeft(millis, 3, '0');
        padLeft(micros, 3, '0');
        cout
            << endl
            << "Time since last mark: "
            << hours
            << ":"
            << minutes
            << ":"
            << seconds
            << "."
            << millis
            << micros
            << endl;
    }
    first = false;
    start = chrono::high_resolution_clock::now();
    return !(print && first); // true if print false or if print true and first false. False otherwise
}

bool checkFileExistance(const string* fileName) {
    std::wstring wideCharTest (fileName->begin(), fileName->end());
    return !(GetFileAttributes(wideCharTest.c_str()) == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
}

bool validateImagePath(string* s) {
    s->assign(std::filesystem::current_path().string() + "\\inImages\\" + s->c_str()); // append curr location
    cout << s->c_str() << endl;
    return checkFileExistance(s);
}

cv::Mat getAndAllocateImage(string imagePath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED); // unchanged includes alpha channel (or may crop image a bit)
    if (image.empty()) {
        cout << "Image Not Found!!!" << endl;
    }
    return image;
}

template<typename Lambda>
void processImage(cv::Mat* img, Lambda algorithm) {
    algorithm(img);
}

bool attemptSaveImage(cv::Mat* image, string outputFileName) {
    unsigned int i = 0;
    string outputFileDirectory = filesystem::current_path().string() + "\\outImages\\";
    string testOutputPath = outputFileDirectory + outputFileName + to_string(i) + ".png";
    while (checkFileExistance(&testOutputPath)) { // don't want to overwrite
        i++;
        testOutputPath = outputFileDirectory + outputFileName + to_string(i) + ".png";
    }
    return cv::imwrite(testOutputPath, *image);
}

// hideString specific
steganograpgy_algorithm_e getHideStringAlgorithm() {
    string algorithm;
    int algorithmNum;
    std::cout << "What algorithm would you like to use to encrypt the message?" << endl
        << "\t1) use all bits of image" << endl
        << "\t2) use only least significant bits" << endl
        << "\t3) use only most significant bits" << endl
        << "\t4) use only blue color bits" << endl
        << "\t5) use only green color bits" << endl
        << "\t6) use only red color bits" << endl
        << "\t7) use least sig bits but only blue" << endl
        << "\t8) use least sig bits but only green" << endl
        << "\t9) use least sig bits but only red" << endl
        << "\t10) use most sig bits but only blue" << endl
        << "\t11) use most sig bits but only green" << endl
        << "\t12) use most sig bits but only red" << endl;
    std::getline(std::cin, algorithm);
    algorithmNum = std::stoi(algorithm);
    if (algorithmNum == 1) return STEGANOGRAPHY_ALGORITHM_BASIC;
    if (algorithmNum == 2) return STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT;
    if (algorithmNum == 3) return STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT;
    if (algorithmNum == 4) return STEGANOGRAPHY_ALGORITHM_BLUEONLY;
    if (algorithmNum == 5) return STEGANOGRAPHY_ALGORITHM_GREENONLY;
    if (algorithmNum == 6) return STEGANOGRAPHY_ALGORITHM_REDONLY;
    if (algorithmNum == 7) return STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT_BLUEONLY;
    if (algorithmNum == 8) return STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT_GREENONLY;
    if (algorithmNum == 9) return STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT_REDONLY;
    if (algorithmNum == 10) return STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT_BLUEONLY;
    if (algorithmNum == 11) return STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT_GREENONLY;
    if (algorithmNum == 12) return STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT_REDONLY;
    return STEGANOGRAPHY_ALGORITHM_BASIC; // return basic if invalid selection
}
*/
