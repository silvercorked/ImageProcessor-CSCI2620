#pragma once

#include <functional>

#include "ImageProcessingEnums.h"
#include "Color.h"
#include "ImageProcessor.h"

namespace IP {
	typedef std::function<IP::ImageProcessor* (IP::ImageProcessor*)> FunctionTypeLayerExec;

	struct Layer {
		ImageProcessor* ip;
		int offsetX;
		int offsetY;
		float alpha;
		BLEND_MODE_E blendMode;
		cv::Vec4b backgroundColor;
		Layer(std::string filename)
			: Layer(new ImageProcessor(filename)) {};
		Layer(std::string filename, int offsetX, int offsetY)
			: Layer(new ImageProcessor(filename), offsetX, offsetY) {};
		Layer(cv::Mat& image, int offsetX, int offsetY)
			: Layer(new ImageProcessor(image), offsetX, offsetY, 1, cv::Vec4b(Color::magenta)) {};
		Layer(IP::ImageProcessor* ip)
			: Layer(ip, 0, 0, 1.0f, cv::Vec4b(Color::magenta)) {};
		Layer(IP::ImageProcessor* ip, int offsetX, int offset)
			: Layer(ip, offsetX, offsetY, 1, cv::Vec4b(Color::magenta)) {};
		Layer(Layer* l)
			: Layer(l->ip->clone(), l->offsetX, l->offsetY, l->alpha, l->backgroundColor) {};
		Layer(IP::ImageProcessor* ip, int offsetX, int offsetY, float alpha, const cv::Vec4b& backgroundColor) {
			this->ip = ip;
			this->offsetX = offsetX;
			this->offsetY = offsetY;
			this->alpha = alpha;
			this->backgroundColor = backgroundColor;
			this->blendMode = IP::BLEND_MODE_TYPE_ENUM::NORMAL;
		};
		Layer* clone() {
			return new Layer(this);
		};
		Layer* exec(IP::ImageProcessor* func(IP::ImageProcessor*)) {
			func(this->ip);
			return this;
		}
		~Layer();
	};
};


