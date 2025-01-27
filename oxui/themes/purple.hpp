#ifdef OXUI_THEME_PURPLE_HPP
#ifndef OXUI_THEME_HPP
#define OXUI_THEME_HPP

#include "../types/types.hpp"

namespace oxui {
	struct _theme {
		/*
		int spacing = 20;
		int list_spacing = 0;
		double animation_speed = 0.4;
		color bg = color( 229, 236, 236, 255 );
		color text = color( 0, 0, 0, 255 );
		color container_bg = color( 252, 253, 253, 255 );
		color main = color( 43, 163, 69, 255 );
		color title_text = color( 191, 191, 191, 255 );
		color title_bar = color( 42, 42, 42, 255 );
		*/

		int spacing = 18;
		double animation_speed = 0.4;
		int list_spacing = 0;
		color bg = color( 22, 18, 22, 255 );
		color text = color( 200, 200, 200, 255 );
		color title_text = color( 200, 200, 200, 255 );
		color container_bg = color( 38, 38, 38, 255 );
		color main = color( 156, 155, 255, 255 );
		color title_bar = color( 38, 38, 38, 255 );
	};

	extern _theme theme;
}

#endif // OXUI_THEME_HPP
#endif // OXUI_THEME_PURPLE_HPP