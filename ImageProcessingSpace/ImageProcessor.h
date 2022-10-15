#pragma once

#define _USE_MATH_DEFINES // allows access to M_PI (PI in c++)
#include <math.h>
#include <stdbool.h>

#include "ImageProcessingEnums.h"
#include "IPBase.h"
#include "TimeManager.h"


using namespace cv;

namespace IP {
	struct ImageProcessor : public IPBase {
		ImageProcessor(const std::string& filepath) : IPBase(filepath) {
			this->tm = new TimeManager();
		};
		ImageProcessor(cv::Mat& image) : IPBase(image) {
			this->tm = new TimeManager();
		};
		ImageProcessor* greyScale();
		ImageProcessor* removeColor(const COLOR_RGBA_TYPE_EMUN);
		ImageProcessor* translate(const int, const int);
		ImageProcessor* translateForward(const int, const int);
		ImageProcessor* rotate(const float, const bool = true);
		ImageProcessor* rotateLinear(const float);
		ImageProcessor* rotateNearestNeighbor(const float);
		ImageProcessor* rotate90();
		ImageProcessor* rotate180();
		ImageProcessor* scale(const float, const float, const bool = true);
		ImageProcessor* scaleLinear(const float, const float);
		ImageProcessor* scaleLinear(const float);
		ImageProcessor* scaleNearestNeighbor(const float, const float);
		ImageProcessor* scaleNearestNeighbor(const float);
		ImageProcessor* toHistogram();
		
		ImageProcessor* hideString(const std::string*);
		ImageProcessor* findString(std::string*);

		int getWidth();
		int getHeight();
		ImageProcessor* clone() override;
		~ImageProcessor() {};

		private:
			TimeManager* tm;
	};
};
