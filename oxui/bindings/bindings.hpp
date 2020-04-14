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
		static std::add_pointer_t< void( const oxui::rect&, std::function< void( ) > ) > clip = [ ] ( const oxui::rect& area, std::function< void( ) > func );
		static std::add_pointer_t< void( const str&, int, bool, font& ) > create_font = [ ] ( const str& font_name, int size, bool bold, font& fout );
		static std::add_pointer_t< void( oxui::rect& ) > screen_size = [ ] ( oxui::rect& p );
		static std::add_pointer_t< void( pos& ) > mouse_pos = [ ] ( oxui::pos& p );
		static std::add_pointer_t< void( const oxui::rect&, const color& ) > rect = [ ] ( const oxui::rect& r, const oxui::color& c );
		static std::add_pointer_t< void( const oxui::rect&, const color& ) > fill_rect = [ ] ( const oxui::rect& r, const oxui::color& c );
		static std::add_pointer_t< void( const oxui::rect&, const color&, const color&, bool ) > gradient_rect = [ ] ( const oxui::rect& r, const oxui::color& c, const oxui::color& c1, bool horizontal );
		static std::add_pointer_t< void( const oxui::pos&, const oxui::pos&, const color& ) > line = [ ] ( const oxui::pos& p1, const oxui::pos& p2, const color& c );
		static std::add_pointer_t< void( const font&, const str&, oxui::rect& ) > text_bounds = [ ] ( const font& font, const str& text, oxui::rect& bounds );
		static std::add_pointer_t< void( const pos&, const font&, const str&, const color&, bool ) > text = [ ] ( const pos& point, const font& font, const str& text, const color& c, bool shadow );
	}
}

#endif // OXUI_BINDINGS_HPP