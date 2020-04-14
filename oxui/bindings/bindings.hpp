#ifndef OXUI_BINDINGS_HPP
#define OXUI_BINDINGS_HPP

#include <memory>
#include <string_view>
#include <functional>
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include "../types/types.hpp"
#include "../../renderer/d3d9.hpp"
#include "../../sdk/sdk.hpp"

namespace oxui {
	namespace binds {
		static std::add_pointer_t< void( const oxui::rect&, std::function< void( ) > ) > clip;
		static std::add_pointer_t< void( const str&, int, bool, font& ) > create_font;
		static std::add_pointer_t< void( oxui::rect& ) > screen_size;
		static std::add_pointer_t< void( pos& ) > mouse_pos;
		static std::add_pointer_t< void( const oxui::rect&, const color& ) > rect;
		static std::add_pointer_t< void( const oxui::rect&, const color& ) > fill_rect;
		static std::add_pointer_t< void( const oxui::rect&, const color&, const color&, bool ) > gradient_rect;
		static std::add_pointer_t< void( const oxui::pos&, const oxui::pos&, const color& ) > line;
		static std::add_pointer_t< void( const font&, const str&, oxui::rect& ) > text_bounds;
		static std::add_pointer_t< void( const pos&, const font&, const str&, const color&, bool ) > text;
	}
}

#endif // OXUI_BINDINGS_HPP