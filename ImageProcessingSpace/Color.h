#pragma once

#include <opencv2/opencv.hpp>
#include <math.h>

#include "ImageProcessingEnums.h"

namespace IP {
    struct Color {
        static cv::Vec4b& RGBAtoHSVA(float r, float g, float b, float a = 255.0f) {
            return Color::RGBAtoHSVA((int) r, (int) g, (int) b, (int) a);
        }
        static cv::Vec4b& RGBAtoHSVA(int r, int g, int b, int a = 255) {
            return Color::RGBAtoHSVA_p(new cv::Vec4b(b, g, r, a));
        };
        static cv::Vec4b& RGBAtoHSVA_p(cv::Vec4b* color) {
            const float r = (*color)[IP::COLOR_RGBA_E::RED];
            const float g = (*color)[IP::COLOR_RGBA_E::GREEN];
            const float b = (*color)[IP::COLOR_RGBA_E::BLUE];
            const float max = fmaxf(r, fmaxf(g, b));
            const float min = fminf(r, fminf(g, b));
            const float diff = max - min;
            float h = 0;
            float s = 0;
            if (diff != 0) {
                if (max == r) {
                    h = 0;
                    h += 60 * (g - b) / diff;
                }
                else if (max == g) {
                    h = 120;
                    h += 60 * (b - r) / diff;
                }
                else {
                    h = 240;
                    h += 60 * (r - g) / diff;
                }
                while (h < 0)
                    h += 360.0;
                h = fmod(h, 360.0);
            }
            if (max > 0)
                s = diff / max * 255;
            float v = max;
            *color = cv::Vec4b(
                (int) h,
                (int) s,
                (int) v,
                (*color)[IP::COLOR_RGBA_E::ALPHA]
            );
            return *color;
        };
        static cv::Vec4b& HSVAtoRGBA(float r, float g, float b, float a = 255.0f) {
            return Color::HSVAtoRGBA((int) r, (int) g, (int) b, (int) a);
        };
        static cv::Vec4b& HSVAtoRGBA(int h, int s, int v, int a = 255) {
            return Color::HSVAtoRGBA_p(new cv::Vec4b(h, s, v, a));
        };
        
        static cv::Vec4b& HSVAtoRGBA_p(cv::Vec4b* color) {
            const float sat = (*color)[IP::COLOR_HSV_E::SATURATION] / 255.0f;
            float val = (*color)[IP::COLOR_HSV_E::VALUE] / 255.0f;
            int h = (*color)[IP::COLOR_HSV_E::HUE] / 60.0f;
            float f = (*color)[IP::COLOR_HSV_E::HUE] / 60.0f - h;
            float p = val * (1 - sat);
            float q = val * (1 - f * sat);
            float t = val * (1 - (1 - f) * sat);

            val *= 255.0f;
            f *= 255.0f;
            p *= 255.0f;
            q *= 255.0f;
            t *= 255.0f;
            (*color)[IP::COLOR_RGBA_E::ALPHA] = 255; // default alpha to 255
            switch (h) {
                case 0:
                    (*color)[IP::RED] = (int) val;
                    (*color)[IP::GREEN] = (int) t;
                    (*color)[IP::BLUE] = (int) p;
                    return *color;
                case 1:
                    (*color)[IP::RED] = (int) q;
                    (*color)[IP::GREEN] = (int) val;
                    (*color)[IP::BLUE] = (int) p;
                    return *color;
                case 2:
                    (*color)[IP::RED] = (int) p;
                    (*color)[IP::GREEN] = (int) val;
                    (*color)[IP::BLUE] = (int) t;
                    return *color;
                case 3:
                    (*color)[IP::RED] = (int) p;
                    (*color)[IP::GREEN] = (int) q;
                    (*color)[IP::BLUE] = (int) val;
                    return *color;
                case 4:
                    (*color)[IP::RED] = (int) t;
                    (*color)[IP::GREEN] = (int) p;
                    (*color)[IP::BLUE] = (int) val;
                    return *color;
                case 5:
                    (*color)[IP::RED] = (int) val;
                    (*color)[IP::GREEN] = (int) p;
                    (*color)[IP::BLUE] = (int) q;
                    return *color;
                default:
                    std::cout << "invalid case of hue greater than 360." << std::endl;
                    return *color;
            }
        };
        static cv::Scalar aliceblue;
        static cv::Scalar antiquewhite;
        static cv::Scalar aqua;
        static cv::Scalar aquamarine;
        static cv::Scalar azure;
        static cv::Scalar beige;
        static cv::Scalar bisque;
        static cv::Scalar black;
        static cv::Scalar blanchedalmond;
        static cv::Scalar blue;
        static cv::Scalar blueviolet;
        static cv::Scalar brown;
        static cv::Scalar burlywood;
        static cv::Scalar cadetblue;
        static cv::Scalar chartreuse;
        static cv::Scalar chocolate;
        static cv::Scalar coral;
        static cv::Scalar cornflowerblue;
        static cv::Scalar cornsilk;
        static cv::Scalar crimson;
        static cv::Scalar cyan;
        static cv::Scalar darkblue;
        static cv::Scalar darkcyan;
        static cv::Scalar darkgoldenrod;
        static cv::Scalar darkgray;
        static cv::Scalar darkgreen;
        static cv::Scalar darkgrey;
        static cv::Scalar darkkhaki;
        static cv::Scalar darkmagenta;
        static cv::Scalar darkolivegreen;
        static cv::Scalar darkorange;
        static cv::Scalar darkorchid;
        static cv::Scalar darkred;
        static cv::Scalar darksalmon;
        static cv::Scalar darkseagreen;
        static cv::Scalar darkslateblue;
        static cv::Scalar darkslategray;
        static cv::Scalar darkslategrey;
        static cv::Scalar darkturquoise;
        static cv::Scalar darkviolet;
        static cv::Scalar deeppink;
        static cv::Scalar deepskyblue;
        static cv::Scalar dimgray;
        static cv::Scalar dimgrey;
        static cv::Scalar dodgerblue;
        static cv::Scalar firebrick;
        static cv::Scalar floralwhite;
        static cv::Scalar forestgreen;
        static cv::Scalar fuchsia;
        static cv::Scalar gainsboro;
        static cv::Scalar ghostwhite;
        static cv::Scalar gold;
        static cv::Scalar goldenrod;
        static cv::Scalar gray;
        static cv::Scalar green;
        static cv::Scalar greenyellow;
        static cv::Scalar grey;
        static cv::Scalar honeydew;
        static cv::Scalar hotpink;
        static cv::Scalar indianred;
        static cv::Scalar indigo;
        static cv::Scalar ivory;
        static cv::Scalar khaki;
        static cv::Scalar lavender;
        static cv::Scalar lavenderblush;
        static cv::Scalar lawngreen;
        static cv::Scalar lemonchiffon;
        static cv::Scalar lightblue;
        static cv::Scalar lightcoral;
        static cv::Scalar lightcyan;
        static cv::Scalar lightgoldenrodyellow;
        static cv::Scalar lightgray;
        static cv::Scalar lightgreen;
        static cv::Scalar lightgrey;
        static cv::Scalar lightpink;
        static cv::Scalar lightsalmon;
        static cv::Scalar lightseagreen;
        static cv::Scalar lightskyblue;
        static cv::Scalar lightslategray;
        static cv::Scalar lightslategrey;
        static cv::Scalar lightsteelblue;
        static cv::Scalar lightyellow;
        static cv::Scalar lime;
        static cv::Scalar limegreen;
        static cv::Scalar linen;
        static cv::Scalar magenta;
        static cv::Scalar maroon;
        static cv::Scalar mediumaquamarine;
        static cv::Scalar mediumblue;
        static cv::Scalar mediumorchid;
        static cv::Scalar mediumpurple;
        static cv::Scalar mediumseagreen;
        static cv::Scalar mediumslateblue;
        static cv::Scalar mediumspringgreen;
        static cv::Scalar mediumturquoise;
        static cv::Scalar mediumvioletred;
        static cv::Scalar midnightblue;
        static cv::Scalar mintcream;
        static cv::Scalar mistyrose;
        static cv::Scalar moccasin;
        static cv::Scalar navajowhite;
        static cv::Scalar navy;
        static cv::Scalar oldlace;
        static cv::Scalar olive;
        static cv::Scalar olivedrab;
        static cv::Scalar orange;
        static cv::Scalar orangered;
        static cv::Scalar orchid;
        static cv::Scalar palegoldenrod;
        static cv::Scalar palegreen;
        static cv::Scalar paleturquoise;
        static cv::Scalar palevioletred;
        static cv::Scalar papayawhip;
        static cv::Scalar peachpuff;
        static cv::Scalar peru;
        static cv::Scalar pink;
        static cv::Scalar plum;
        static cv::Scalar powderblue;
        static cv::Scalar purple;
        static cv::Scalar red;
        static cv::Scalar rosybrown;
        static cv::Scalar royalblue;
        static cv::Scalar saddlebrown;
        static cv::Scalar salmon;
        static cv::Scalar sandybrown;
        static cv::Scalar seagreen;
        static cv::Scalar seashell;
        static cv::Scalar sienna;
        static cv::Scalar silver;
        static cv::Scalar skyblue;
        static cv::Scalar slateblue;
        static cv::Scalar slategray;
        static cv::Scalar slategrey;
        static cv::Scalar snow;
        static cv::Scalar springgreen;
        static cv::Scalar steelblue;
        static cv::Scalar tan;
        static cv::Scalar teal;
        static cv::Scalar thistle;
        static cv::Scalar tomato;
        static cv::Scalar turquoise;
        static cv::Scalar violet;
        static cv::Scalar wheat;
        static cv::Scalar white;
        static cv::Scalar whitesmoke;
        static cv::Scalar yellow;
        static cv::Scalar yellowgreen;
        static cv::Scalar transparent;
    };
};
