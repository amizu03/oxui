﻿#include "window.hpp"
#include "../panels/panel.hpp"
#include "shapes.hpp"
#include "../themes/purple.hpp"
#include "tab.hpp"
#include "../json/cJSON.h"
#include <string>
#include <fstream>

/* menu objects */
#include "checkbox.hpp"
#include "slider.hpp"
#include "dropdown.hpp"
#include "textbox.hpp"
#include "keybind.hpp"
#include "colorpicker.hpp"
#include <ShlObj.h>

/* resources */
#include "../../resources/images/微信.hpp"

void* oxui::window::find_obj( const str& tab_name, const str& group_name, const str& object_name, object_type type ) {
	for ( auto& _tab : objects ) {
		auto tab = std::static_pointer_cast< oxui::tab >( _tab );

		if ( tab->title == tab_name ) {
			for ( auto& _group : tab->objects ) {
				auto group = std::static_pointer_cast< oxui::group >( _group );

				if ( group->title == group_name ) {
					for ( auto& _control : group->objects ) {
						switch ( _control->type ) {
						case object_checkbox: {
							auto as_checkbox = std::static_pointer_cast< checkbox >( _control );

							if ( as_checkbox->label == object_name )
								return &as_checkbox->checked;

							break;
						}
						case object_slider: {
							auto as_slider = std::static_pointer_cast< slider >( _control );

							if ( as_slider->label == object_name )
								return &as_slider->value;

							break;
						}
						case object_dropdown: {
							auto as_dropdown = std::static_pointer_cast< dropdown >( _control );

							if ( as_dropdown->label == object_name )
								return &as_dropdown->value;

							break;
						}
						case object_textbox: {
							auto as_textbox = std::static_pointer_cast< textbox >( _control );

							if ( as_textbox->label == object_name )
								return &as_textbox->buf;

							break;
						}
						case object_keybind: {
							auto as_keybind = std::static_pointer_cast< keybind >( _control );

							if ( as_keybind->label == object_name )
								return &as_keybind->key;

							break;
						}
						case object_colorpicker: {
							auto as_colorpicker = std::static_pointer_cast< color_picker >( _control );

							if ( as_colorpicker->label == object_name )
								return &as_colorpicker->clr;

							break;
						}
						}
					}
				}
			}
		}
	}

	return nullptr;
}

void oxui::window::save_state( const str& file ) {
	const auto json = cJSON_CreateObject ( );

	auto window_str = std::string( title.begin( ), title.end( ) );

	const auto window = cJSON_CreateObject ( );

	std::for_each ( objects.begin ( ), objects.end ( ), [ & ] ( std::shared_ptr< obj >& tab_obj ) {
		auto tab = std::static_pointer_cast< oxui::tab >( tab_obj );
		auto tab_str = std::string ( tab->title.begin ( ), tab->title.end ( ) );

		const auto jtab = cJSON_CreateObject ( );

		std::for_each ( tab->objects.begin ( ), tab->objects.end ( ), [ & ] ( std::shared_ptr< obj >& group_obj ) {
			auto group = std::static_pointer_cast< oxui::group >( group_obj );
			auto group_str = std::string ( group->title.begin ( ), group->title.end ( ) );

			const auto jgroup = cJSON_CreateObject ( );

			std::for_each ( group->objects.begin ( ), group->objects.end ( ), [ & ] ( std::shared_ptr< obj >& object ) {
				switch ( object->type ) {
				case object_checkbox: {
					auto as_checkbox = ( ( std::shared_ptr< checkbox >& ) object );
					auto obj_str = std::string ( as_checkbox->label.begin ( ), as_checkbox->label.end ( ) );
					cJSON_AddItemToObject ( jgroup, obj_str.c_str ( ), cJSON_CreateBool ( as_checkbox->checked ) );
					break;
				}
				case object_slider: {
					auto as_slider = ( ( std::shared_ptr< slider >& ) object );
					auto obj_str = std::string ( as_slider->label.begin ( ), as_slider->label.end ( ) );
					cJSON_AddItemToObject ( jgroup, obj_str.c_str ( ), cJSON_CreateNumber ( as_slider->value ) );
					break;
				}
				case object_dropdown: {
					auto as_dropdown = ( ( std::shared_ptr< dropdown >& ) object );
					auto obj_str = std::string ( as_dropdown->label.begin ( ), as_dropdown->label.end ( ) );
					cJSON_AddItemToObject ( jgroup, obj_str.c_str ( ), cJSON_CreateNumber ( as_dropdown->value ) );
					break;
				}
									//case object_textbox: {
									//	auto as_textbox = ( ( std::shared_ptr< textbox >& ) object );
									//	auto obj_str = std::string ( as_textbox->label.begin ( ), as_textbox->label.end ( ) );
									//	json [ window_str ][ tab_str ][ group_str ][ obj_str ] = as_textbox->buf;
									//	break;
									//}
				case object_keybind: {
					auto as_keybind = ( ( std::shared_ptr< keybind >& ) object );
					auto obj_str = std::string ( as_keybind->label.begin ( ), as_keybind->label.end ( ) );
					cJSON_AddItemToObject ( jgroup, obj_str.c_str ( ), cJSON_CreateNumber ( as_keybind->key ) );
					break;
				}
				case object_colorpicker: {
					auto as_colorpicker = ( ( std::shared_ptr< color_picker >& ) object );
					auto obj_str = std::string ( as_colorpicker->label.begin ( ), as_colorpicker->label.end ( ) );
					const auto clr_arr = cJSON_CreateArray ( );
					cJSON_AddItemToArray ( clr_arr, cJSON_CreateNumber ( as_colorpicker->clr.r ) );
					cJSON_AddItemToArray ( clr_arr, cJSON_CreateNumber ( as_colorpicker->clr.g ) );
					cJSON_AddItemToArray ( clr_arr, cJSON_CreateNumber ( as_colorpicker->clr.b ) );
					cJSON_AddItemToArray ( clr_arr, cJSON_CreateNumber ( as_colorpicker->clr.a ) );
					cJSON_AddItemToObject ( jgroup, obj_str.c_str ( ), clr_arr );
					break;
				}
				}
			} );

			cJSON_AddItemToObject ( jtab, group_str.c_str ( ), jgroup );
		} );

		cJSON_AddItemToObject ( window, tab_str.c_str ( ), jtab );
	} );

	cJSON_AddItemToObject ( json, window_str.c_str ( ), window );

	const auto dump = cJSON_Print ( json );

	wchar_t appdata [ MAX_PATH ];

	if ( SUCCEEDED ( LI_FN ( SHGetFolderPathW )( nullptr, N ( CSIDL_PERSONAL ), nullptr, N ( int ( SHGFP_TYPE_CURRENT ) ), appdata ) ) ) {
		LI_FN ( CreateDirectoryW )( ( std::wstring ( appdata ) + _ ( L"\\sesame" ) ).data ( ), nullptr );
		LI_FN ( CreateDirectoryW )( ( std::wstring ( appdata ) + _ ( L"\\sesame\\configs" ) ).data ( ), nullptr );

		std::ofstream ofile ( std::wstring ( appdata ) + _ ( L"\\sesame\\configs\\" ) + file );

		if ( ofile.is_open ( ) ) {
			ofile.write ( dump, strlen ( dump ) );
			ofile.close ( );
		}
	}

	cJSON_Delete ( json );
}

void oxui::window::load_state( const str& file ) {
	std::string dump;

	wchar_t appdata [ MAX_PATH ];

	if ( SUCCEEDED ( LI_FN ( SHGetFolderPathW )( nullptr, N ( CSIDL_PERSONAL ), nullptr, N ( int ( SHGFP_TYPE_CURRENT ) ), appdata ) ) ) {
		LI_FN ( CreateDirectoryW )( ( std::wstring ( appdata ) + _ ( L"\\sesame" ) ).data ( ), nullptr );
		LI_FN ( CreateDirectoryW )( ( std::wstring ( appdata ) + _ ( L"\\sesame\\configs" ) ).data ( ), nullptr );
	}

	std::ifstream ofile ( std::wstring ( appdata ) + _ ( L"\\sesame\\configs\\" ) + file );

	if ( !ofile.is_open ( ) )
		return;

	ofile.seekg ( 0, std::ios::end );
	const auto fsize = ofile.tellg ( );
	ofile.seekg ( 0, std::ios::beg );
	char* str = new char [ fsize ];
	ofile.read ( str, fsize );
	dump = str;
	delete [ ] str;
	ofile.close ( );

	if ( dump.empty ( ) )
		return;

	const auto json = cJSON_Parse ( dump.c_str ( ) );

	if ( !json ) {
		dbg_print ( cJSON_GetErrorPtr ( ) );
		cJSON_Delete ( json );
		return;
	}

	auto window_str = std::string ( title.begin ( ), title.end ( ) );

	const auto jwindow = cJSON_GetObjectItemCaseSensitive ( json, window_str.c_str ( ) );

	if ( !jwindow ) {
		dbg_print ( cJSON_GetErrorPtr ( ) );
		cJSON_Delete ( json );
		return;
	}

	std::for_each ( objects.begin ( ), objects.end ( ), [ & ] ( std::shared_ptr< obj > tab_obj ) {
		auto tab = std::static_pointer_cast< oxui::tab >( tab_obj );
		auto tab_str = std::string ( tab->title.begin ( ), tab->title.end ( ) );

		const auto jtab = cJSON_GetObjectItemCaseSensitive ( jwindow, tab_str.c_str ( ) );

		if ( !jtab ) {
			dbg_print ( cJSON_GetErrorPtr ( ) );
			return;
		}

		std::for_each ( tab->objects.begin ( ), tab->objects.end ( ), [ & ] ( std::shared_ptr< obj > group_obj ) {
			auto group = std::static_pointer_cast< oxui::group >( group_obj );
			auto group_str = std::string ( group->title.begin ( ), group->title.end ( ) );

			const auto jgroup = cJSON_GetObjectItemCaseSensitive ( jtab, group_str.c_str ( ) );

			if ( !jgroup ) {
				dbg_print ( cJSON_GetErrorPtr ( ) );
				return;
			}

			std::for_each ( group->objects.begin ( ), group->objects.end ( ), [ & ] ( std::shared_ptr< obj >& object ) {
				switch ( object->type ) {
				case object_checkbox: {
					auto as_checkbox = ( checkbox* ) object.get ( );
					auto obj_str = std::string ( as_checkbox->label.begin ( ), as_checkbox->label.end ( ) );

					const auto jitem = cJSON_GetObjectItemCaseSensitive ( jgroup, obj_str.c_str ( ) );

					if ( !jitem || !cJSON_IsBool ( jitem ) ) {
						dbg_print ( cJSON_GetErrorPtr ( ) );
						return;
					}

					as_checkbox->checked = jitem->valueint;
					break;
				}
				case object_slider: {
					auto as_slider = ( slider* ) object.get ( );
					auto obj_str = std::string ( as_slider->label.begin ( ), as_slider->label.end ( ) );

					const auto jitem = cJSON_GetObjectItemCaseSensitive ( jgroup, obj_str.c_str ( ) );

					if ( !jitem || !cJSON_IsNumber ( jitem ) ) {
						dbg_print ( cJSON_GetErrorPtr ( ) );
						return;
					}

					as_slider->value = jitem->valuedouble;
					break;
				}
				case object_dropdown: {
					auto as_dropdown = ( dropdown* ) object.get ( );
					auto obj_str = std::string ( as_dropdown->label.begin ( ), as_dropdown->label.end ( ) );

					const auto jitem = cJSON_GetObjectItemCaseSensitive ( jgroup, obj_str.c_str ( ) );

					if ( !jitem || !cJSON_IsNumber ( jitem ) ) {
						dbg_print ( cJSON_GetErrorPtr ( ) );
						return;
					}

					as_dropdown->value = jitem->valueint;
					break;
				}
									//case object_textbox: {
									//	auto as_textbox = ( textbox* ) object.get ( );
									//	auto obj_str = std::string ( as_textbox->label.begin ( ), as_textbox->label.end ( ) );
									//
									//	/* control doesn't exist */
									//	if ( !json [ window_str ][ tab_str ][ group_str ].contains ( obj_str ) )
									//		return;
									//
									//	const auto as_str = json [ window_str ][ tab_str ][ group_str ][ obj_str ].get< std::string > ( );
									//	as_textbox->buf = std::wstring ( as_str.begin ( ), as_str.end ( ) );
									//	break;
									//}
				case object_keybind: {
					auto as_keybind = ( keybind* ) object.get ( );
					auto obj_str = std::string ( as_keybind->label.begin ( ), as_keybind->label.end ( ) );

					const auto jitem = cJSON_GetObjectItemCaseSensitive ( jgroup, obj_str.c_str ( ) );

					if ( !jitem || !cJSON_IsNumber ( jitem ) ) {
						dbg_print ( cJSON_GetErrorPtr ( ) );
						return;
					}

					as_keybind->key = jitem->valueint;
					break;
				}
				case object_colorpicker: {
					auto as_colorpicker = ( color_picker* ) object.get ( );
					auto obj_str = std::string ( as_colorpicker->label.begin ( ), as_colorpicker->label.end ( ) );

					const auto jitem = cJSON_GetObjectItemCaseSensitive ( jgroup, obj_str.c_str ( ) );

					if ( !jitem || !cJSON_IsArray ( jitem ) ) {
						dbg_print ( cJSON_GetErrorPtr ( ) );
						return;
					}

					const cJSON* clr_channel = nullptr;
					int clr_channel_num = 0;

					cJSON_ArrayForEach ( clr_channel, jitem ) {
						if ( !clr_channel || !cJSON_IsNumber ( clr_channel ) ) {
							dbg_print ( cJSON_GetErrorPtr ( ) );
							clr_channel_num++;
							continue;
						}

						switch ( clr_channel_num ) {
						case 0: as_colorpicker->clr.r = clr_channel->valueint; break;
						case 1: as_colorpicker->clr.g = clr_channel->valueint; break;
						case 2: as_colorpicker->clr.b = clr_channel->valueint; break;
						case 3: as_colorpicker->clr.a = clr_channel->valueint; break;
						}

						clr_channel_num++;
					}

					break;
				}
				}
			} );
		} );
	} );

	cJSON_Delete ( json );
}

void oxui::window::think( ) {
	if ( !pressing_move_key && GetAsyncKeyState( VK_LBUTTON ) && shapes::hovering( rect( area.x, area.y - 26, area.w, 26 ) ) ) {
		pressing_move_key = true;
		pos mouse_mpos;
		binds::mouse_pos( mouse_mpos );
		click_offset = pos( mouse_mpos.x - area.x, mouse_mpos.y - area.y );
	}
	else if ( pressing_move_key && GetAsyncKeyState( VK_LBUTTON ) ) {
		pos mouse_mpos;
		binds::mouse_pos( mouse_mpos );

		area.x = mouse_mpos.x - click_offset.x;
		area.y = mouse_mpos.y - click_offset.y;
	}
	else {
		pressing_move_key = false;
	}
}

void oxui::window::draw( ) {
	shapes::finished_input_frame = false;

	render_overlay = false;

	if ( toggle_bind ) {
		if ( !pressing_open_key && GetAsyncKeyState( toggle_bind ) ) {
			pressing_open_key = true;
		}
		else if ( pressing_open_key && !GetAsyncKeyState( toggle_bind ) ) {
			open = !open;
			pressing_open_key = false;

			if ( open )
				g_input = true;
		}
	}

	if ( !open ) {
		render_overlay = false;
		handle_keyboard = false;
		g_input = false;
		return;
	}

	auto& parent_panel = find_parent< panel >( object_panel );

	/* menu physics */
	think( );

	cursor_pos = pos( area.x + theme.spacing, area.y + theme.spacing );

	/* draw window rect */
	binds::fill_rect( area, theme.bg );

	/* title bar */
	binds::fill_rect( rect( area.x, area.y - 26, area.w, 26 ), theme.title_bar );
	shapes::box( rect( area.x, area.y - 26, area.w, 26 ), 0.0, false, false, true, false, false, false );
	// render::texture( rsc::images::weixin_logo, area.x + 4, area.y - 28 + 6, 118, 100, 0.133f );
	binds::text( pos( area.x + 4 /* + 24 */, area.y - 28 + 6 ), parent_panel.fonts [ OSTR( "title" ) ], title, theme.title_text, false );
	binds::rect ( rect ( area.x - 1, area.y - 1 - 26, area.w + 1, area.h + 1 + 26 ), color ( 0, 0, 0, 255 ) );

	std::vector< std::pair< std::shared_ptr< tab >, int > > tab_list;
	auto total_tabs_w = 0;
	std::for_each( objects.begin( ), objects.end( ), [ & ] ( std::shared_ptr< obj > object ) {
		if ( object->type == object_tab ) {
			auto as_tab = std::static_pointer_cast< tab >( object );

			rect bounds;
			binds::text_bounds( parent_panel.fonts [ OSTR( "object" ) ], as_tab->title, bounds );
			total_tabs_w += bounds.w + 4;

			tab_list.push_back( std::pair< std::shared_ptr< tab >, int >( as_tab, bounds.w ) );
		}
		} );
	auto last_tab_pos = pos( area.x + area.w - total_tabs_w - 6, area.y - 28 + 6 );
	std::for_each( tab_list.begin( ), tab_list.end( ), [ & ] ( std::pair< std::shared_ptr< tab >, int > object ) {
		if ( shapes::clicking( rect( last_tab_pos.x, last_tab_pos.y, object.second, 16 ) ) ) {
			/* update click timer */
			object.first->time = parent_panel.time;

			/* deselect all */
			std::for_each(
				tab_list.begin( ),
				tab_list.end( ),
				[ & ] ( std::pair< std::shared_ptr< tab >, int > child ) {
					/* re-run animation */
					if ( child.first->selected )
						child.first->time = parent_panel.time;

					child.first->selected = false;
				}
			);

			/* select clicked tab */
			object.first->selected = true;
		}

		auto time_since_click = std::clamp( parent_panel.time - object.first->time, 0.0, theme.animation_speed );
		auto bar_width = object.second - object.second * ( time_since_click * ( 1.0 / theme.animation_speed ) );
		auto text_height = 0;
		auto alpha = 255 - int( time_since_click * ( 1.0 / theme.animation_speed ) * 80.0 );

		if ( object.first->selected ) {
			alpha = 175 + int( time_since_click * ( 1.0 / theme.animation_speed ) * 80.0 );
			bar_width = object.second * ( time_since_click * ( 1.0 / theme.animation_speed ) );
			text_height = 2 * ( time_since_click * ( 1.0 / theme.animation_speed ) );
		}

		binds::text( pos( last_tab_pos.x, area.y - 28 + 6 - text_height ), parent_panel.fonts [ OSTR( "object" ) ], object.first->title, color( theme.title_text.r, theme.title_text.g, theme.title_text.b, alpha ), false );
		binds::line( pos( last_tab_pos.x + object.second / 2 - bar_width / 2, area.y - 28 + 6 + 16 ), pos( last_tab_pos.x + object.second / 2 + bar_width / 2, area.y - 28 + 6 + 16 ), theme.main );

		last_tab_pos.x += object.second + 4;
		} );

	/* draw window objects */
	binds::clip( area, [ & ] ( ) {
		std::for_each(
			objects.begin( ),
			objects.end( ),
			[ ] ( std::shared_ptr< obj >& child ) {
				child->draw( );
			}
		);
		} );

	if ( render_overlay )
		overlay_func( );

	shapes::click_switch = false;

	scroll_delta = 0.0;
}