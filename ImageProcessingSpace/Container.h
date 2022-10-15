#pragma once

#include <functional>

#include "Layer.h"

namespace IP {
	typedef std::function<ImageProcessor* (ImageProcessor*)> FunctionTypeExecOnImageProcessor;
	typedef std::function<Layer* (Layer*)> FunctionTypeGenerateLayer;
	class Container {
		int width;
		int height;
		cv::Vec4b backgroundColor;
		int currentLayer;
		public:
			std::vector<Layer> layers;
			Container(const std::string& filename)
				: Container(new ImageProcessor(filename)) {};
			Container(const std::string& filename, int offsetX, int offsetY)
				: Container(new Layer(filename, offsetX, offsetY)) {};
			Container(ImageProcessor* imageProcessor)
				: Container(new Layer(imageProcessor)) {};
			Container(Layer* layer) {
				this->currentLayer = 0;
				this->layers.push_back(layer);
				this->width = layer->ip->getWidth();
				this->height = layer->ip->getHeight();
				this->backgroundColor = Color::magenta;
			};
			Container* save(std::string& filename) {
				ImageProcessor* flat = this->flatten();
				flat->save(filename);
				return this;
			};
			Container* addLayer(std::string& filename) {
				return this->addLayer(new Layer(filename));
			};
			Container* addLayer(Layer* layer) {
				layers.push_back(layer);
			};
			Layer* getCurrentLayer() {
				return &this->layers.at(this->currentLayer);
			};
			Container* selectLayer(int index) {
				if (index < 0 || index >= this->layers.size())
					throw std::invalid_argument("selected index is outside the bounds of container!");
				this->currentLayer = index;
				return this;
			};
			Container* exec(FunctionTypeExecOnImageProcessor func) {
				func(this->getCurrentLayer()->ip);
				return this;
			};
			Container* generateLayer(FunctionTypeGenerateLayer func) {
				this->addLayer(
					func(
						this->getCurrentLayer()->clone()
					)
				);
				return this;
			};
			Container* modifyCanvasSize(int dX, int dY) {
				this->width += dX;
				this->height += dY;
				return this;
			};
			Container* translateLayer(int dX, int dY) {
				this->getCurrentLayer()->offsetX += dX;
				this->getCurrentLayer()->offsetY += dY;
				return this;
			}
			ImageProcessor* flatten() {

			};
			// more to make
	};
}

