#include "IPBase.h"

using namespace IP;

void IPBase::appendSystemPath(std::string* filename) {
	filename->assign(
		std::filesystem::current_path().string()
		+ "\\inImages\\"
		+ filename->c_str()
	); // append curr location
}

bool IPBase::checkFileExistance(const std::string* path) {
	std::wstring wideCharTest(path->begin(), path->end());
	return !(GetFileAttributes(wideCharTest.c_str()) == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
}

bool IPBase::loadFile() {
	std::cout << "loading image" << std::endl;
	if (this->image != NULL)
		delete this->image;
	this->image = new cv::Mat();
	(*this->image) = cv::imread(*(this->imagePath), cv::IMREAD_UNCHANGED); // unchanged includes alpha channel (or may crop image a bit)
	std::cout << "channels " << this->image->channels() << std::endl;
	if (this->image->channels() != 4) {
		cv::Mat* temp = new cv::Mat(this->image->rows, this->image->cols, CV_8UC4);
		cv::cvtColor(*this->image, *temp, cv::COLOR_BGR2BGRA); // X channel -> 4 channel
		delete this->image;
		this->image = temp;
	}
	std::cout << "after" << std::endl;
	return !this->image->empty();
}

float IPBase::length(const float x, const float y) {
	return sqrtf(x * x + y * y);
}
float IPBase::getAngle(const float x, const float y) {
	return atan2f(y, x);
}
float IPBase::getX(const float r, const float theta) {
	return cosf(theta) * r;
}
float IPBase::getY(const float r, const float theta) {
	return sinf(theta) * r;
}
float IPBase::fInterpolate(const float v0, const float v1, const float t) {
	return (1 - t) * v0 + t * v1;
}
cv::Vec4b* IPBase::vInterpolate(const cv::Vec4b& one, const cv::Vec4b& two, const float percent) {
	return new cv::Vec4b(
		(1 - percent) * one[IP::BLUE] + (percent * two[IP::BLUE]),
		(1 - percent) * one[IP::GREEN] + (percent * two[IP::GREEN]),
		(1 - percent) * one[IP::RED] + (percent * two[IP::RED]),
		(1 - percent) * one[IP::ALPHA] + (percent * two[IP::ALPHA])
	);
}

void IPBase::updatePixels(FunctionTypeUpdatePixels func) {
	cv::Vec4b* color = new cv::Vec4b();
	cv::Point* point = new cv::Point(0, 0);
	for (; point->y < this->image->rows; point->y++) {
		for (point->x = 0; point->x < this->image->cols; point->x++) {
			*color = this->image->at<cv::Vec4b>(*point);
			func(*color);
			this->image->at<cv::Vec4b>(*point) = *color;
		}
	}
	delete point;
	delete color;
}
void IPBase::updateImage(FunctionTypeUpdateImage func) {
	const int bh = this->image->rows;
	const int bw = this->image->cols;
	cv::Vec4b* color = new cv::Vec4b();
	cv::Mat* intermediate = new cv::Mat((int) bh, (int) bw, this->image->type(), Color::magenta);
	cv::Point* point = new cv::Point(0, 0);
	for (; point->y < bh; point->y++) {
		for (point->x = 0; point->x < bw; point->x++) {
			func(*this->image, bw, bh, *point, color);
			intermediate->at<cv::Vec4b>(*point) = *color;
		}
	}
	delete point;
	delete color;
	delete this->image;
	this->image = intermediate;
}
void IPBase::updateImageAndSize(int nw, int nh, FunctionTypeUpdateImage func) {
	cv::Vec4b* color = new cv::Vec4b();
	cv::Mat* intermediate = new cv::Mat((int) nh, (int) nw, this->image->type(), Color::magenta);
	cv::Point* point = new cv::Point(0, 0);
	for (; point->y < nh; point->y++) {
		for (point->x = 0; point->x < nw; point->x++) {
			func(*this->image, nw, nh, *point, color);
			intermediate->at<cv::Vec4b>(*point) = *color;
		}
	}
	delete point;
	delete color;
	delete this->image;
	this->image = intermediate;
}

void IPBase::getBilinear(cv::Mat& img, const float x, const float y, cv::Vec4b* ret) {
	const int xInt = (int)x; const int yInt = (int) y;
	const int bh = img.rows;
	const int bw = img.cols;
	cv::Vec4b* color00 = new cv::Vec4b(this->image->at<cv::Vec4b>(yInt, xInt));
	cv::Vec4b color01, color10, color11;
	if (xInt + 1 < bw) color10 = cv::Vec4b(this->image->at<cv::Vec4b>(yInt, xInt + 1));
	else color10 = cv::Vec4b(Color::magenta);
	if (yInt + 1 < bh) color01 = cv::Vec4b(this->image->at<cv::Vec4b>(yInt + 1, xInt));
	else color01 = cv::Vec4b(Color::magenta);
	if (xInt + 1 < bw && yInt + 1 < bh) color11 = cv::Vec4b(this->image->at<cv::Vec4b>(yInt + 1, xInt + 1));
	else color11 = cv::Vec4b(Color::magenta);
	float percentX = x - xInt;
	float percentY = y - yInt;
	cv::Vec4b* interpolate1 = this->vInterpolate(*color00, color10, percentX);
	delete color00;
	cv::Vec4b* interpolate2 = this->vInterpolate(color01, color11, percentY);
	cv::Vec4b* temp = this->vInterpolate(*interpolate1, *interpolate2, percentY);
	*ret = *temp;
	delete temp;
	delete interpolate1;
	delete interpolate2;
}
void IPBase::getNearestNeighbor(cv::Mat& img, const int x, const int y, cv::Vec4b* ret) {
	if (x >= 0 && y >= 0 && x < img.cols && y < img.rows)
		(*ret) = img.at<cv::Vec4b>(y, x);
}
void IPBase::getNearestNeighbor(cv::Mat& img, const float x, const float y, cv::Vec4b* ret) {
	this->getNearestNeighbor(img, (int) x, (int) y, ret);
}

/*
	rgb to hsv conversions
	value = max(r,g,b) or max(r,g,b)
	saturation = ((max(r,g,b) - min(r,g,b)) / max(r,g,b)) * 255
	hue = x + 60 * (y - z) / (max(r,g,b) - min(r,g,b))
		if (max(r,g,b) == g)
			x = 120, y = b, z = r
		if (max(r,g,b) == r)
			x = 240, y = g, z = b
		if (max(r,g,b) == b)
			x = 0, y = r, z = g

	hsv to rgb conversions
	is hue closest to 0, 120, or 240?
	if 0, blue = value
	if 120, green = value
	if 240, red = value
	
	min = value - value * saturation
	which side of value color are we on?
	if value set to green, red or blue side
		if red side
			blue = min
			red = ...
		if blue side
			red = min
			blue = ...
	if value set to red, blue and green side
		if blue side,
			green = min
			blue = ...
		if green side,
			blue = min
			green = ...
	if value set to blue, green or red side
		if green side
			red = min
			green = ...
		if red side
			green = min
			red = ...

	hue = base(0,120,240) + 60 *(min - mid)/ (max-min)
		

*/

bool IPBase::save(const std::string& saveFileName) {
	unsigned int i = 0;
	std::string outputFileName = std::string(saveFileName);
	std::string outputFileDirectory = std::filesystem::current_path().string() + "\\outImages\\";
	std::string testOutputPath = outputFileDirectory + outputFileName + std::to_string(i) + ".png";
	while (this->checkFileExistance(&testOutputPath)) { // don't want to overwrite
		i++;
		testOutputPath = outputFileDirectory + outputFileName + "-" + std::to_string(i) + ".png";
	}
	return cv::imwrite(testOutputPath, *this->image);
}
