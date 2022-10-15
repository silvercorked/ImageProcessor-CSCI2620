#include "ImageProcessor.h"

using namespace IP;

ImageProcessor* ImageProcessor::greyScale() {
	this->updatePixels([](cv::Vec4b& color) {
		int avg = (color[BLUE] + color[GREEN] + color[RED]) / 3;
		color[BLUE] = avg; color[GREEN] = avg; color[RED] = avg;
	});
	return this;
}

cv::Vec4b removeRed(cv::Vec4b color) {
	color[RED] = 0;
	return color;
}
cv::Vec4b removeGreen(cv::Vec4b color) {
	color[GREEN] = 0;
	return color;
}
cv::Vec4b removeBlue(cv::Vec4b color) {
	color[BLUE] = 0;
	return color;
}
ImageProcessor* ImageProcessor::removeColor(COLOR_RGBA_E colorToRemove) {
	int channels = this->image->channels();
	cv::Point* point = new cv::Point();
	auto removeFunc = (colorToRemove == RED
		? removeRed
		: (colorToRemove == GREEN
			? removeGreen
			: removeBlue));
	for (int y = 0; y < this->image->rows; y++) {
		point->y = y;
		for (int x = 0; x < this->image->cols; x++) {
			point->x = x;
			this->image->at<cv::Vec4b>(*point) = removeFunc(this->image->at<cv::Vec4b>(*point));
		}
	}
	delete point;
	return this;
}

ImageProcessor* ImageProcessor::translateForward(int dx, int dy) {
	const int bh = this->image->rows;
	const int bw = this->image->cols;
	cv::Mat* intermediate = new cv::Mat(bh, bw, this->image->type(), Color::magenta);
	cv::Vec4b* color = new cv::Vec4b();
	cv::Point* point = new cv::Point();
	cv::Point* currPoint = new cv::Point();
	for (unsigned int y = 0; y < bh; y++) {
		point->y = y;
		currPoint->y = y + dy;
		for (unsigned int x = 0; x < bw; x++) {
			point->x = x;
			currPoint->x = x + dx;
			*color = this->image->at<cv::Vec4b>(*point);
			if (currPoint->x >= bw || currPoint->y >= bh || currPoint->x < 0 || currPoint->y < 0)
				continue;
			intermediate->at<cv::Vec4b>(*currPoint) = *color;
		}
	}
	delete color;
	delete point;
	delete currPoint;
	delete this->image;
	this->image = intermediate;
	return this;
}
ImageProcessor* ImageProcessor::translate(int dx, int dy) {
	this->updateImage([dx, dy](const cv::Mat& img, const int bw, const int bh, const cv::Point& point, cv::Vec4b* color) {
		int x1 = point.x - dx;
		int y1 = point.y - dy;
		if (x1 >= 0 && y1 >= 0 && x1 <= bw && y1 <= bh)
			*color = img.at<cv::Vec4b>(point);
		else
			*color = cv::Vec4b(Color::magenta);
	});
	return this;
}
ImageProcessor* ImageProcessor::rotate90() {
	const int bh = this->image->rows;
	const int bw = this->image->cols;
	this->updateImageAndSize(bh, bw, [](const cv::Mat& img, const int bw, const int bh, const cv::Point& point, cv::Vec4b* color) {
		int y1 = bw - point.x - 1;
		int x1 = bh - point.y - 1;
		*color = img.at<cv::Vec4b>(y1, x1);
	});
	return this;
}
ImageProcessor* ImageProcessor::rotate180() {
	this->updateImage([](const cv::Mat& img, const int bw, const int bh, const cv::Point& point, cv::Vec4b* color) {
		int y1 = bw - point.x - 1;
		int x1 = bh - point.y - 1;
		*color = img.at<cv::Vec4b>(x1, y1);
	});
	return this;
}
ImageProcessor* ImageProcessor::rotate(const float degrees, const bool linearInterpolation) {
	// Rotate in degrees, about upper-left, down
	const int bh = this->image->rows;
	const int bw = this->image->cols;
	cv::Mat* intermediate = new cv::Mat((int) bh, (int) bw, this->image->type());
	cv::Vec4b* color = new cv::Vec4b();
	cv::Point* point = new cv::Point();
	const float radians = (float) (degrees / 360 * M_PI * 2);
	for (unsigned int y = 0; y < bh; y++) {
		point->y = y;	// doing up here prob avoids saves more time than dropping it into the loop below
		for (unsigned int x = 0; x < bw; x++) {
			point->x = x;

			float r = length(x, y);
			float theta = getAngle(x, y);

			float nAngle = theta - radians;
			float nX = getX(r, nAngle);
			float nY = getY(r, nAngle);

			*color = cv::Vec4b(Color::magenta);
			//std::cout << "nX " << nX << " nY " << nY << " bw, bh " << bw << "," << bh << std::endl;
			if (!(nX >= bw || nY >= bh || nX < 0 || nY < 0)) {
				if (linearInterpolation)
					this->getBilinear(*this->image, nX, nY, color);
				else
					this->getNearestNeighbor(*this->image, nX, nY, color);
			}
			intermediate->at<cv::Vec4b>(*point) = *color;
		}
	}
	delete color;
	delete point;
	delete this->image;
	this->image = intermediate;
	return this;
}

ImageProcessor* ImageProcessor::rotateLinear(const float deg) {
	return this->rotate(deg);
}
ImageProcessor* ImageProcessor::rotateNearestNeighbor(const float deg) {
	return this->rotate(deg, false);
}

ImageProcessor* ImageProcessor::scale(const float xs, const float ys, bool linearInterpolation) {
	const int bh = this->image->rows;
	const int bw = this->image->cols;
	const int nh = bh * ys + 0.5;
	const int nw = bw * xs + 0.5;
	this->updateImageAndSize(nh, nw, [&](const cv::Mat& img, const int bw, const int bh, const cv::Point& point, cv::Vec4b* color) {
		float oldX = (point.x / xs);
		float oldY = (point.y / ys);
		*color = Color::magenta; // default color
		if (oldX >= bw || oldY >= bh || oldX < 0 || oldY < 0) {
			if (linearInterpolation)
				this->getBilinear(*this->image, oldX, oldY, color);
			else
				this->getNearestNeighbor(*this->image, oldX, oldY, color);
		}
	});
	return this;
}

ImageProcessor* ImageProcessor::scaleLinear(const float xs, const float ys) {
	return this->scale(xs, ys);
}
ImageProcessor* ImageProcessor::scaleLinear(const float s) {
	return this->scale(s, s);
}
ImageProcessor* ImageProcessor::scaleNearestNeighbor(const float xs, const float ys) {
	return this->scale(xs, ys, false);
}
ImageProcessor* ImageProcessor::scaleNearestNeighbor(const float s) {
	return this->scale(s, s, false);
}

ImageProcessor* ImageProcessor::toHistogram() {
	const unsigned int bh = this->image->rows;
	const unsigned int bw = this->image->cols;
	int h = 100; int w = 255;
	cv::Mat* intermediate = new cv::Mat(h, w + 1, CV_8UC4);
	std::vector<float> histogram(w + 1); // create vector of length w + 1
	for (auto& elem : histogram) {
		elem = 0;
	}
	cv::Point* curr = new cv::Point();
	cv::Vec4b* pixel = new cv::Vec4b();
	for (unsigned int y = 0; y < bh; y++) {
		curr->y = y;
		for (unsigned int x = 0; x < bw; x++) {
			curr->x = x;
			*pixel = this->image->at<cv::Vec4b>(*curr);
			Color::RGBAtoHSVA_p(pixel);
			histogram[(int) (*pixel)[IP::COLOR_HSV_TYPE_ENUM::VALUE]]++;
		}
	} // add pixels HSV values to get frequency
	float max = 0;
	for (auto& elem : histogram) {
		if (elem > max)
			max = elem;
	}
	for (auto& elem : histogram) {
		elem /= max;
	} // normalize [0, 1]

	for (unsigned int x = 0; x < w + 1; x++) {
		curr->y = 0;
		curr->x = x;
		while (curr->y < h) {
			if (curr->y < (1.0f - histogram[curr->x]) * h)
				*pixel = Color::black;
			else
				*pixel = Color::white;
			intermediate->at<cv::Vec4b>(*curr) = *pixel;
			curr->y++;
		}
	} // set colors to black and white in intermediate
	delete this->image;
	this->image = intermediate;
	delete curr;
	delete pixel;
	return this;
}

int ImageProcessor::getWidth() {
	return this->image->cols;
}
int ImageProcessor::getHeight() {
	return this->image->rows;
}

ImageProcessor* ImageProcessor::clone() {
	return new ImageProcessor(*this->image);
}
