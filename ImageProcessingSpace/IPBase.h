#pragma once

#include <opencv2/opencv.hpp>

#include <filesystem>
#include <windows.h>

#include <functional>

#include "ImageProcessingEnums.h"
#include "Color.h"

namespace IP {
	typedef std::function<void (cv::Vec4b&)> FunctionTypeUpdatePixels;
	typedef std::function<void (const cv::Mat&, const int, const int, const cv::Point&, cv::Vec4b*)> FunctionTypeUpdateImage;

	class IPBase {
		protected:
			cv::Mat* image;
			std::string* imagePath;

			virtual void appendSystemPath(std::string*);
			virtual bool checkFileExistance(const std::string*);
			virtual bool loadFile();

			float length(const float, const float);
			float getAngle(const float, const float);
			float getX(const float, const float);
			float getY(const float, const float);
			float fInterpolate(const float, const float, const float);
			cv::Vec4b* vInterpolate(const cv::Vec4b&, const cv::Vec4b&, const float);

			void updatePixels(FunctionTypeUpdatePixels);
			void updateImage(FunctionTypeUpdateImage);
			void updateImageAndSize(int, int, FunctionTypeUpdateImage);

			void getBilinear(cv::Mat&, const float, const float, cv::Vec4b*);
			void getNearestNeighbor(cv::Mat& img, const int, const int, cv::Vec4b*);
			void getNearestNeighbor(cv::Mat&, const float, const float, cv::Vec4b*);
		public:
			IPBase(const std::string& filename) {
				this->image = NULL; // need to initialize
				this->imagePath = new std::string(filename);
				this->appendSystemPath(this->imagePath);
				bool valid = this->checkFileExistance(this->imagePath);
				if (valid) {
					valid = this->loadFile();
					if (!valid)
						std::cout << "Image Failed To Load: " << *this->imagePath << std::endl;
				}
				else std::cout << "Image Path Invalid: " << *this->imagePath << std::endl;
			};
			IPBase(cv::Mat& image) {
				cv::Mat cloned = image.clone();
				cv::Mat* p = &cloned;
				this->image = p;
				this->imagePath = NULL;
			}
			virtual IPBase* clone() = 0;
			virtual bool save(const std::string&);
			~IPBase() {};
	};
};

