// ImageProcessingExperimentationSpace.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ImageProcessingSpace.h"

#include "ImageProcessor.h"
#include "Container.h"

using namespace IP;

int main() {
    
    /*
   Container* container = new Container("beach-small.jpg");
   container->generateLayer([](Layer* layer) {
       return layer->ip->toHistogram();
   })->modifyCanvasSize()->flatten()->save("beach-small-histogram");
   */

    ImageProcessor* ip = new ImageProcessor("beach-small.jpg");
    ip->toHistogram()->save("beach-small-histogram");
    //ip->greyScale()->save("cat-attempt");

    /*
    string command;
    int commandNum = -1;
    markTime(false);
    cout << "Available Commands: (enter number)" << endl
        << "\t1) viewSelectableImages" << endl
        << "\t2) removeColor" << endl
        << "\t3) hideString" << endl
        << "\t4) findStringInImage" << endl
        << "\t5) rotate 180 degrees" << endl;
    getline(cin, command);
    commandNum = stoi(command);
    if (commandNum == VIEWSELECTABLEIMAGES) return handleViewSelectableImages() ? -1 : 0;
    else if (commandNum == REMOVECOLOR) return handleRemoveColor() ? -1 : 0;
    else if (commandNum == HIDESTRING) return handleHideString() ? -1 : 0;
    else if (commandNum == FINDSTRINGINIMAGE) return handleFindStringInImage() ? -1 : 0;
    else if (commandNum == ROTATEIMAGE) return handleRotate() ? -1 : 0;
    cout << "Command number: " << commandNum << " not recognized." << endl;
    return -1; */
    return 0;
}

/*
bool handleViewSelectableImages() {
    string path = std::filesystem::current_path().string() + "\\inImages";
    for (const auto& entry : filesystem::directory_iterator(path)) {
        string path = entry.path().string();
        cout << path.substr(path.find_last_of('\\') + 1) << endl;
    }
    return true;
}

bool handleRemoveColor() {
    string imageFileName;
    string outputFileName;
    string colorToRemove;
    markTime(false);
    cout << "Enter an image file path: ";
    getline(cin, imageFileName);
    cout << endl << "Finding and validating image at: " << imageFileName << endl;
    outputFileName = imageFileName.substr(0, imageFileName.find_last_of('.')); // file name w/o extension
    
    markTime(true);
    if (!validateImagePath(&imageFileName)) {
        cout << "Validation failed for image: " << imageFileName << endl;
        return false;
    }
    cout << "string after valid: " << imageFileName << endl;
    
    markTime(true);
    cv::Mat image = getAndAllocateImage(imageFileName);
    if (image.empty()) return false;

    markTime(true);
    cout << "Which color would you like to remove (red, green, or blue)?" << endl;
    getline(cin, colorToRemove);
    auto colorBeingRemoved = BLUE;
    if (colorToRemove == "red") colorBeingRemoved = RED;
    else if (colorToRemove == "green") colorBeingRemoved == GREEN;
    else if (colorToRemove != "blue") {
        cout << "invalid option." << endl;
        return false;
    }

    markTime(true);
    auto curriedRemoveColor = curry(removeColor);
    processImage(&image, curriedRemoveColor(colorBeingRemoved));
    
    markTime(true);
    unsigned int i = 0;
    string outputFileDirectory = filesystem::current_path().string() + "\\outImages\\";
    string testOutputPath = outputFileDirectory + outputFileName + to_string(i) + ".png";
    while (checkFileExistance(&testOutputPath)) { // don't want to overwrite
        i++;
        testOutputPath = outputFileDirectory + outputFileName + to_string(i) + ".png";
    }
    cv::imwrite(testOutputPath, image);
   
    markTime(true);
    return true;
}

bool handleHideString() {
    string imageFileName;
    string outputFileName;
    string toHide;
    markTime(false);
    cout << "Enter an image file path: ";
    getline(cin, imageFileName);
    cout << endl << "Finding and validating image at: " << imageFileName << endl;
    outputFileName = imageFileName.substr(0, imageFileName.find_last_of('.')); // file name w/o extension

    markTime(true);
    if (!validateImagePath(&imageFileName)) {
        cout << "Validation failed for image: " << imageFileName << endl;
        return false;
    }
    cout << "string after valid: " << imageFileName << endl;

    markTime(true);
    cv::Mat image = getAndAllocateImage(imageFileName);
    if (image.empty()) return false;

    markTime(true);
    cout << "What secret message would you like to hide?" << endl;
    getline(cin, toHide);

    markTime(true);
    auto algorithm = getHideStringAlgorithm();

    markTime(true);
    processImage(&image, curry(hideString)(toHide)(algorithm));

    markTime(true);
    bool writeSuccess = attemptSaveImage(&image, outputFileName);

    markTime(true);
    return writeSuccess ? 0 : -1;
}

bool handleFindStringInImage() {
    string imageFileName;
    string outputFileName;
    string* fromImage;
    markTime(false);
    cout << "Enter an image file path: ";
    getline(cin, imageFileName);
    cout << endl << "Finding and validating image at: " << imageFileName << endl;
    outputFileName = imageFileName.substr(0, imageFileName.find_last_of('.')); // file name w/o extension

    markTime(true);
    if (!validateImagePath(&imageFileName)) {
        cout << "Validation failed for image: " << imageFileName << endl;
        return false;
    }
    cout << "string after valid: " << imageFileName << endl;

    markTime(true);
    cv::Mat image = getAndAllocateImage(imageFileName);
    if (image.empty()) return false;

    markTime(true);
    auto algorithm = getHideStringAlgorithm();

    markTime(true);
    fromImage = curry(findStringInImage)(algorithm)(&image);

    markTime(true);
    cout << "Message found in image: " << *fromImage << endl;
    return 0;
}

bool handleRotate() {
    string imageFileName;
    string outputFileName;
    markTime(false);
    cout << "Enter an image file path: ";
    getline(cin, imageFileName);
    cout << endl << "Finding and validating image at: " << imageFileName << endl;
    outputFileName = imageFileName.substr(0, imageFileName.find_last_of('.')); // file name w/o extension

    markTime(true);
    if (!validateImagePath(&imageFileName)) {
        cout << "Validation failed for image: " << imageFileName << endl;
        return false;
    }
    cout << "string after valid: " << imageFileName << endl;

    markTime(true);
    cv::Mat image = getAndAllocateImage(imageFileName);
    if (image.empty()) return false;

    markTime(true);
    //processImage(&image, curry(rotate)(R180));

    markTime(true);
    bool writeSuccess = attemptSaveImage(&image, outputFileName);

    markTime(true);
    return writeSuccess ? 0 : -1;
}
*/
