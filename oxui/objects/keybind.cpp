#include "keybind.hpp"
#include "../panels/panel.hpp"
#include "shapes.hpp"
#include "../themes/purple.hpp"

#define make_str(x) case x: as_str = OSTR( #x ); break;

oxui::str key_to_str ( int vk ) {
	if ( vk <= 0 )
		return OSTR ( "-" );

	if ( vk >= '0' && vk <= '9' ) { return oxui::str ( 1, wchar_t ( vk ) ); }
	if ( vk >= 'A' && vk <= 'Z' ) { return oxui::str ( 1, wchar_t ( vk ) ); }

	oxui::str as_str;
	
	switch ( vk ) {
		make_str ( VK_LBUTTON );
		make_str ( VK_RBUTTON );
		make_str ( VK_CANCEL );
		make_str ( VK_MBUTTON );  
		make_str ( VK_XBUTTON1 ); 
		make_str ( VK_XBUTTON2 ); 
		make_str ( VK_BACK );
		make_str ( VK_TAB );
		make_str ( VK_CLEAR );
		make_str ( VK_RETURN );
		make_str ( VK_SHIFT );
		make_str ( VK_CONTROL );
		make_str ( VK_MENU );
		make_str ( VK_PAUSE );
		make_str ( VK_CAPITAL );
		make_str ( VK_KANA );
		make_str ( VK_JUNJA );
		make_str ( VK_FINAL );
		make_str ( VK_KANJI );
		make_str ( VK_ESCAPE );
		make_str ( VK_CONVERT );
		make_str ( VK_NONCONVERT );
		make_str ( VK_ACCEPT );
		make_str ( VK_MODECHANGE );
		make_str ( VK_SPACE );
		make_str ( VK_PRIOR );
		make_str ( VK_NEXT );
		make_str ( VK_END );
		make_str ( VK_HOME );
		make_str ( VK_LEFT );
		make_str ( VK_UP );
		make_str ( VK_RIGHT );
		make_str ( VK_DOWN );
		make_str ( VK_SELECT );
		make_str ( VK_PRINT );
		make_str ( VK_EXECUTE );
		make_str ( VK_SNAPSHOT );
		make_str ( VK_INSERT );
		make_str ( VK_DELETE );
		make_str ( VK_HELP );
		make_str ( VK_LWIN );
		make_str ( VK_RWIN );
		make_str ( VK_APPS );
		make_str ( VK_SLEEP );
		make_str ( VK_NUMPAD0 );
		make_str ( VK_NUMPAD1 );
		make_str ( VK_NUMPAD2 );
		make_str ( VK_NUMPAD3 );
		make_str ( VK_NUMPAD4 );
		make_str ( VK_NUMPAD5 );
		make_str ( VK_NUMPAD6 );
		make_str ( VK_NUMPAD7 );
		make_str ( VK_NUMPAD8 );
		make_str ( VK_NUMPAD9 );
		make_str ( VK_MULTIPLY );
		make_str ( VK_ADD );
		make_str ( VK_SEPARATOR );
		make_str ( VK_SUBTRACT );
		make_str ( VK_DECIMAL );
		make_str ( VK_DIVIDE );
		make_str ( VK_F1 );
		make_str ( VK_F2 );
		make_str ( VK_F3 );
		make_str ( VK_F4 );
		make_str ( VK_F5 );
		make_str ( VK_F6 );
		make_str ( VK_F7 );
		make_str ( VK_F8 );
		make_str ( VK_F9 );
		make_str ( VK_F10 );
		make_str ( VK_F11 );
		make_str ( VK_F12 );
		make_str ( VK_F13 );
		make_str ( VK_F14 );
		make_str ( VK_F15 );
		make_str ( VK_F16 );
		make_str ( VK_F17 );
		make_str ( VK_F18 );
		make_str ( VK_F19 );
		make_str ( VK_F20 );
		make_str ( VK_F21 );
		make_str ( VK_F22 );
		make_str ( VK_F23 );
		make_str ( VK_F24 );
		make_str ( VK_NUMLOCK );
		make_str ( VK_SCROLL );
		make_str ( VK_OEM_NEC_EQUAL );  
		make_str ( VK_OEM_FJ_MASSHOU ); 
		make_str ( VK_OEM_FJ_TOUROKU ); 
		make_str ( VK_OEM_FJ_LOYA );    
		make_str ( VK_OEM_FJ_ROYA );    
		make_str ( VK_LSHIFT );
		make_str ( VK_RSHIFT );
		make_str ( VK_LCONTROL );
		make_str ( VK_RCONTROL );
		make_str ( VK_LMENU );
		make_str ( VK_RMENU );
		make_str ( VK_BROWSER_BACK );
		make_str ( VK_BROWSER_FORWARD );
		make_str ( VK_BROWSER_REFRESH );
		make_str ( VK_BROWSER_STOP );
		make_str ( VK_BROWSER_SEARCH );
		make_str ( VK_BROWSER_FAVORITES );
		make_str ( VK_BROWSER_HOME );
		make_str ( VK_VOLUME_MUTE );
		make_str ( VK_VOLUME_DOWN );
		make_str ( VK_VOLUME_UP );
		make_str ( VK_MEDIA_NEXT_TRACK );
		make_str ( VK_MEDIA_PREV_TRACK );
		make_str ( VK_MEDIA_STOP );
		make_str ( VK_MEDIA_PLAY_PAUSE );
		make_str ( VK_LAUNCH_MAIL );
		make_str ( VK_LAUNCH_MEDIA_SELECT );
		make_str ( VK_LAUNCH_APP1 );
		make_str ( VK_LAUNCH_APP2 );
		make_str ( VK_OEM_1 );     
		make_str ( VK_OEM_PLUS );  
		make_str ( VK_OEM_COMMA ); 
		make_str ( VK_OEM_MINUS ); 
		make_str ( VK_OEM_PERIOD );
		make_str ( VK_OEM_2 ); 
		make_str ( VK_OEM_3 ); 
		make_str ( VK_OEM_4 ); 
		make_str ( VK_OEM_5 ); 
		make_str ( VK_OEM_6 ); 
		make_str ( VK_OEM_7 ); 
		make_str ( VK_OEM_8 );
		make_str ( VK_OEM_AX );  
		make_str ( VK_OEM_102 ); 
		make_str ( VK_ICO_HELP );
		make_str ( VK_ICO_00 );  
		make_str ( VK_PROCESSKEY );
		make_str ( VK_ICO_CLEAR );
		make_str ( VK_PACKET );
		default: return OSTR( "unk" );
	}

	as_str.erase ( 0, 3 );

	for ( auto& character : as_str ) {
		if ( character == '_' ) {
			character = ' ';
			continue;
		}

		if ( character >= 65 && character <= 90 )
			character = std::tolower ( character );
	}

	return as_str;
}

void oxui::keybind::think ( ) {
	window& parent_window = find_parent< window > ( object_window );
	pos& cursor_pos = parent_window.cursor_pos;
	animate ( rect ( cursor_pos.x, cursor_pos.y, area.w, area.h ) );

	if ( shapes::clicking ( rect ( cursor_pos.x, cursor_pos.y, area.w, area.h ), false, true ) )
		searching = true;

	if ( searching ) {
		for ( int i = 0; i < 255; i++ ) {
			if ( GetAsyncKeyState ( i ) ) {

				/* we can't bind left/right click */
				if ( i == VK_LBUTTON || i == VK_RBUTTON ) {
					return;
				}

				/* bind other mouse binds */
				if ( i == VK_MBUTTON ) {
					key = VK_MBUTTON;
					shapes::finished_input_frame = searching = false;
					return;
				}

				if ( i == VK_XBUTTON1 ) {
					key = VK_XBUTTON1;
					shapes::finished_input_frame = searching = false;
					return;
				}

				if ( i == VK_XBUTTON2 ) {
					key = VK_XBUTTON2;
					shapes::finished_input_frame = searching = false;
					return;
				}

				/* bind */
				key = i;

				/* invalidate if pressing ESCAPE */
				if ( i == VK_ESCAPE ) {
					key = -1;
					shapes::finished_input_frame = searching = false;
					return;
				}

				/* stop searching */
				shapes::finished_input_frame = searching = false;

			}
		}

	}
}

void oxui::keybind::draw( ) {
	think( );

	auto& parent_panel = find_parent< panel >( object_panel );
	auto& parent_window = find_parent< window >( object_window );

	auto& font = parent_panel.fonts [ OSTR( "object" ) ];

	auto& cursor_pos = parent_window.cursor_pos;

	/* button box */
	shapes::box( rect( cursor_pos.x, cursor_pos.y, area.w, area.h ), fade_timer, true, true, true, true, true, false );

	auto text = label + OSTR ( " [ " ) + key_to_str ( key ) + OSTR ( " ]" );

	if ( searching )
		text = OSTR ( "[ ... ]" );

	rect text_size;
	binds::text_bounds( font, text, text_size );

	auto area_center_x = cursor_pos.x + area.w / 2;
	auto area_center_y = cursor_pos.y + theme.spacing / 2;
	auto check_dimensions = rect( 0, 0, 10, 6 );

	/* centered text */
	binds::text( pos( area_center_x - text_size.w / 2 - 1, area_center_y - text_size.h / 2 - 1 ), font, text, theme.text, false );
}