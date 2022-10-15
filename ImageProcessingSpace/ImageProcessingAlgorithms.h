#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include "curry.hpp"

/*typedef enum COLOR_E_TYPE {
	BLUE = 0,
	GREEN = 1,
	RED = 2,
	ALPHA = 3
} color_e;

typedef enum STEGANOGRAPHY_ALGORITHM_E_TYPE {
	STEGANOGRAPHY_ALGORITHM_BASIC = 0,
	STEGANOGRAPHY_ALGORITHM_BASIC_REDONLY = 1,
	STEGANOGRAPHY_ALGORITHM_BLUEONLY = 2,
	STEGANOGRAPHY_ALGORITHM_GREENONLY = 3,
	STEGANOGRAPHY_ALGORITHM_REDONLY = 4,
	STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT = 5,
	STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT_BLUEONLY = 6,
	STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT_GREENONLY = 7,
	STEGANOGRAPHY_ALGORITHM_LEASTSIGNIGIFCANTBIT_REDONLY = 8,
	STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT = 9,
	STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT_BLUEONLY = 10,
	STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT_GREENONLY = 11,
	STEGANOGRAPHY_ALGORITHM_MOSTSIGNIFICANTBIT_REDONLY = 12
} steganograpgy_algorithm_e;

typedef enum ROTATE_DEGREE_E_TYPE {
	R90,
	R180,
	R270
} rotate_degree_e;

bool toGreyScale(cv::Mat* image) {
	int channels = image->channels();
	cv::Point* point = new cv::Point();
	if (channels == 3) {
		cv::Vec4b* color = new cv::Vec4b();
		for (int y = 0; y < image->rows; y++) {
			point->y = y;
			for (int x = 0; x < image->cols; x++) {
				point->x = x;
				*color = image->at<cv::Vec4b>(*point);
				unsigned char b = (*color)[BLUE];
				unsigned char g = (*color)[GREEN];
				unsigned char r = (*color)[RED];
				int avg = b; avg += g; avg += r; avg /= 3;
				image->at<cv::Vec4b>(*point) = cv::Vec4b(
					avg,
					avg,
					avg
				);
				//std::cout << std::endl << "colors: " << image->channels() << " " << +r << " " << +g << " " << +b << std::endl;
			}
		}
		delete color;
	}
	delete point;
	return true;
}

unsigned char* maskOutBlue(unsigned char b, unsigned char g, unsigned char r) {
	unsigned char* color = new unsigned char[3];
	color[0] = 0; color[1] = g; color[2] = r;
	return color;
}
unsigned char* maskOutGreen(unsigned char b, unsigned char g, unsigned char r) {
	unsigned char* color = new unsigned char[3];
	color[0] = b; color[1] = 0; color[2] = r;
	return color;
}
unsigned char* maskOutRed(unsigned char b, unsigned char g, unsigned char r) {
	unsigned char* color = new unsigned char[3];
	color[0] = b; color[1] = g; color[2] = 0;
	return color;
}
unsigned char* maskOutNothing(unsigned char b, unsigned char g, unsigned char r) {
	unsigned char* color = new unsigned char[3];
	color[0] = b; color[1] = g; color[2] = r;
	return color;
}
auto removeColor(color_e c) {
	auto alg = c == BLUE ? maskOutBlue : (c == GREEN ? maskOutGreen : (c == RED ? maskOutRed : maskOutNothing));
	return [alg](cv::Mat* image) {
		int channels = image->channels();
		cv::Point* point = new cv::Point();
		if (channels == 3) {
			cv::Vec4b* color = new cv::Vec4b();
			for (int y = 0; y < image->rows; y++) {
				point->y = y;
				for (int x = 0; x < image->cols; x++) {
					point->x = x;
					*color = image->at<cv::Vec4b>(*point);
					unsigned char b = (*color)[BLUE];
					unsigned char g = (*color)[GREEN];
					unsigned char r = (*color)[RED];
					unsigned char* nColor = alg(b, g, r);
					image->at<cv::Vec4b>(*point) = cv::Vec4b(
						nColor[BLUE],
						nColor[GREEN],
						nColor[RED]
					);
					delete nColor;
					//std::cout << std::endl << "colors: " << image->channels() << " " << +r << " " << +g << " " << +b << std::endl;
				}
			}
			delete color;
		}
		delete point;
		return true;
	};
}

unsigned char getCharFromStringOrFallback(std::string s, int index, unsigned char fallback) {
	size_t size = s.size();
	if (index < size) return s.at(index);
	else if (index == size) return '\0';
	else return fallback;
}
auto hideString(std::string s) {
	return [s](steganograpgy_algorithm_e alg) {
		if (alg == STEGANOGRAPHY_ALGORITHM_BASIC) {
			return [s](cv::Mat* image) {
				int stringIndex = 0;
				int channels = image->channels();
				cv::Point* point = new cv::Point();
				if (channels == 3) {
					cv::Vec4b* color = new cv::Vec4b();
					for (int y = 0; y < image->rows; y++) {
						point->y = y;
						for (int x = 0; x < image->cols; x++) {
							point->x = x;
							*color = image->at<cv::Vec4b>(*point);
							unsigned char r = getCharFromStringOrFallback(s, stringIndex++, (*color)[RED]);
							unsigned char g = getCharFromStringOrFallback(s, stringIndex++, (*color)[GREEN]);
							unsigned char b = getCharFromStringOrFallback(s, stringIndex++, (*color)[BLUE]);
							image->at<cv::Vec4b>(*point) = cv::Vec4b(
								b, // these are reversed because bits are
								g, // aligned from left to right, so r (bits 16-23)
								r  // come before b (bits 0-7)
							);
							//std::cout << std::endl << "colors: " << image->channels() << " " << +r << " " << +g << " " << +b << std::endl;
						}
					}
					delete color;
				}
				delete point;
				return true;
			};
		}
	};
}

auto findStringInImage(steganograpgy_algorithm_e alg) {
	if (alg == STEGANOGRAPHY_ALGORITHM_BASIC)
		return [](cv::Mat* image) {
			std::string* s = new std::string();
			int stringIndex = 0;
			int channels = image->channels();
			cv::Point* point = new cv::Point();
			if (channels == 3) {
				cv::Vec4b* color = new cv::Vec4b();
				for (int y = 0; y < image->rows; y++) {
					point->y = y;
					for (int x = 0; x < image->cols; x++) {
						point->x = x;
						*color = image->at<cv::Vec4b>(*point);
						unsigned char r = (*color)[RED];
						unsigned char g = (*color)[GREEN];
						unsigned char b = (*color)[BLUE];
						//if (stringIndex < 100) {
						//	std::cout << b << g << r << std::endl;
						//	if (b == '\0') std::cout << "newline found" << std::endl;
						//	if (g == '\0') std::cout << "newline found" << std::endl;
						//	if (r == '\0') std::cout << "newline found" << std::endl;
						//}
						//stringIndex += 3;
						if (r == '\0') goto findStringInImageBreakOutOfDoubleLoop;
						*s += (char) r;
						if (g == '\0') goto findStringInImageBreakOutOfDoubleLoop;
						*s += (char) g;
						if (b == '\0') goto findStringInImageBreakOutOfDoubleLoop;
						*s += (char) b;
						
						//std::cout << std::endl << "colors: " << image->channels() << " " << +r << " " << +g << " " << +b << std::endl;
					}
				}
				findStringInImageBreakOutOfDoubleLoop: delete color;
			}
			delete point;
			return s;
		};
}

auto rotate(rotate_degree_e rotation) {
	if (rotation == R180) {
		return [](cv::Mat* image) {
			int channels = image->channels();
			cv::Point* point = new cv::Point();
			cv::Point* pointTemp = new cv::Point();
			if (channels == 3) {
				cv::Vec4b* color = new cv::Vec4b();
				cv::Vec4b* colorOther = new cv::Vec4b();
				int halfHeight = image->rows / 2;
				int halfWidth = image->cols / 2;
				for (int y = 0; y < halfHeight; y++) {
					point->y = y;
					for (int x = 0; x < halfWidth; x++) {
						point->x = x;
						pointTemp->x = image->cols - x;
						pointTemp->y = image->rows - y;
						*color = image->at<cv::Vec4b>(*point);
						unsigned char b = (*color)[BLUE];
						unsigned char g = (*color)[GREEN];
						unsigned char r = (*color)[RED];
						*colorOther = image->at<cv::Vec4b>(*pointTemp);
						unsigned char b2 = (*colorOther)[BLUE];
						unsigned char g2 = (*colorOther)[GREEN];
						unsigned char r2 = (*colorOther)[RED];
						image->at<cv::Vec4b>(*point) = cv::Vec4b(
							b2,
							g2,
							r2
						);
						image->at<cv::Vec4b>(*pointTemp) = cv::Vec4b(
							b,
							g,
							r
						);
					}
				}
				delete color;
				delete colorOther;
			}
			delete point;
			delete pointTemp;
			return true;
		};
	}
}
*/
