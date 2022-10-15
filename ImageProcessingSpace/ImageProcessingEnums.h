#pragma once

namespace IP {
	typedef enum COLOR_RGBA_E {
		BLUE = 0,
		GREEN = 1,
		RED = 2,
		ALPHA = 3
	} COLOR_RGBA_TYPE_EMUN;
	typedef enum COLOR_HSV_E {
		HUE = 0,
		SATURATION = 1,
		VALUE = 2
	} COLOR_HSV_TYPE_ENUM;
	typedef enum BLEND_MODE_E {
		NORMAL,
		ADD,
		SUBTRACT,
		DARKEN,
		LIGHTEN,
		DARKER_COLOR,
		LIGHTER_COLOR,
		MULTIPLY,
		DIVIDE
	} BLEND_MODE_TYPE_ENUM;
};
