#include "colorpicker.hpp"
#include "../panels/panel.hpp"
#include "shapes.hpp"
#include "../themes/purple.hpp"
#include "../themes/pink.hpp"

void oxui::color_picker::calculate_phase( const pos& offset, color& color_out ) {
	constexpr auto _w = 233, _h = 230;
	auto w = _w - 15 - 20;
	auto h = _h - 15 - 20;

	auto div = w / 6;
	auto phase = offset.x / div;
	auto t = ( offset.x % div ) / static_cast< float >( div );
	auto r = 0, g = 0, b = 0;

	switch ( phase ) {
	case 0:
		r = 255;
		g = 255 * t;
		b = 0;
		break;
	case 1:
		r = 255 * ( 1.f - t );
		g = 255;
		b = 0;
		break;
	case 2:
		r = 0;
		g = 255;
		b = 255 * t;
		break;
	case 3:
		r = 0;
		g = 255 * ( 1.f - t );
		b = 255;
		break;
	case 4:
		r = 255 * t;
		g = 0;
		b = 255;
		break;
	case 5:
		r = 255;
		g = 0;
		b = 255 * ( 1.f - t );
		break;
	}

	auto sat = offset.y / static_cast< float >( h );
	color_out = color( r + sat * ( 128 - r ), g + sat * ( 128 - g ), b + sat * ( 128 - b ), color_out.a );
}

void oxui::color_picker::think( ) {
	auto& parent_window = find_parent< window >( object_window );
	auto& cursor_pos = parent_window.cursor_pos;
	animate( rect( cursor_pos.x, cursor_pos.y, area.w, area.h ) );

	if ( shapes::clicking( rect( cursor_pos.x, cursor_pos.y, area.w, area.h ), false, opened ) ) {
		g_input = !( opened = !opened );
		mcursor_pos = pos( 0, 0 );
		mcursor_pos1 = pos( 0, 0 );
		mcursor_pos2 = pos( 0, 0 );
	}

	/* handle */
	if ( opened ) {
		constexpr auto w = 233, h = 230;

		rect picker_area{ cursor_pos.x + area.w, cursor_pos.y + 26, w, h };
		rect picker_frame{ picker_area.x + 10, picker_area.y + 10, w - 20, h - 20 };
		rect picker_rect{ picker_frame.x, picker_frame.y, picker_frame.w - 15, picker_frame.h - 15 };
		rect brightness_bar{ picker_frame.x + picker_frame.w - 10, picker_frame.y, 10, picker_frame.h - 15 };
		rect alpha_bar{ picker_frame.x, picker_frame.y + picker_frame.h - 10, picker_frame.w - 15, 10 };
		rect preview_rect{ picker_frame.x + picker_frame.w - 10, picker_frame.y + picker_frame.h - 10, 11, 11 };

		if ( GetAsyncKeyState( VK_LBUTTON ) ) {
			pos mouse_pos;
			binds::mouse_pos( mouse_pos );

			const auto hovered = mouse_pos.x >= picker_rect.x && mouse_pos.y >= picker_rect.y && mouse_pos.x <= picker_rect.x + picker_rect.w - 1 && mouse_pos.y <= picker_rect.y + picker_rect.h;

			/* color picker color calc */
			if ( hovered ) {
				calculate_phase( pos( mouse_pos.x - picker_rect.x, mouse_pos.y - picker_rect.y ), clr_raw );
				mcursor_pos = mouse_pos;
			}

			const auto hovered_brightness_bar = mouse_pos.x >= brightness_bar.x && mouse_pos.y >= brightness_bar.y && mouse_pos.x <= brightness_bar.x + brightness_bar.w - 1 && mouse_pos.y <= brightness_bar.y + brightness_bar.h;

			/* brightness calc */
			if ( hovered_brightness_bar ) {
				brightness_coeff = ( 255.0 - ( ( 255.0 / brightness_bar.h ) * ( mouse_pos.y - picker_rect.y ) ) ) / ( 255.0 / 2.0 );
				mcursor_pos1 = mouse_pos;

				if ( brightness_coeff > 1.0 )
					clr = color( clr_raw.r + ( brightness_coeff - 1.0 ) * ( 255 - clr_raw.r ), clr_raw.g + ( brightness_coeff - 1.0 ) * ( 255 - clr_raw.g ), clr_raw.b + ( brightness_coeff - 1.0 ) * ( 255 - clr_raw.b ), clr.a );
				else
					clr = color( clr_raw.r * brightness_coeff, clr_raw.g * brightness_coeff, clr_raw.b * brightness_coeff, clr.a );
			}

			const auto hovered_alpha_bar = mouse_pos.x >= alpha_bar.x && mouse_pos.y >= alpha_bar.y && mouse_pos.x <= alpha_bar.x + alpha_bar.w - 1 && mouse_pos.y <= alpha_bar.y + alpha_bar.h;

			/* alpha calc */
			if ( hovered_alpha_bar ) {
				clr.a = 255.0 - ( ( 255.0 / alpha_bar.w ) * ( mouse_pos.x - picker_rect.x ) );
				mcursor_pos2 = mouse_pos;
			}

			/* we clicked outside the color picker, let's close */
			rect picker_title_bar( picker_area.x, picker_area.y - 26, picker_area.w, 26 );
			const auto hovered_area = mouse_pos.x >= picker_area.x && mouse_pos.y >= picker_area.y && mouse_pos.x <= picker_area.x + picker_area.w - 1 && mouse_pos.y <= picker_area.y + picker_area.h;
			const auto hovered_open = mouse_pos.x >= cursor_pos.x && mouse_pos.y >= cursor_pos.y && mouse_pos.x <= cursor_pos.x + area.w - 1 && mouse_pos.y <= cursor_pos.y + area.h;
			const auto hovered_tbar = mouse_pos.x >= picker_title_bar.x && mouse_pos.y >= picker_title_bar.y && mouse_pos.x <= picker_title_bar.x + picker_title_bar.w - 1 && mouse_pos.y <= picker_title_bar.y + picker_title_bar.h;
			if ( !hovered && !hovered_brightness_bar && !hovered_alpha_bar && !hovered_area && !hovered_open && !hovered_tbar ) {
				shapes::finished_input_frame = true;
				shapes::click_start = pos( 0, 0 );
				g_input = true;
				opened = false;
			}

		}
	}
}

void oxui::color_picker::draw( ) {
	think( );

	auto& parent_panel = find_parent< panel >( object_panel );
	auto& parent_window = find_parent< window >( object_window );
	auto font = parent_panel.fonts[ OSTR( "object" ) ];
	auto win_font = parent_panel.fonts[ OSTR( "title" ) ];
	auto cursor_pos = parent_window.cursor_pos;

	/* highlighted selection */
	shapes::box( rect( cursor_pos.x, cursor_pos.y, area.w, area.h ), fade_timer, true, true, true, false, false, false );

	rect text_size;
	binds::text_bounds( font, label, text_size );

	auto area_center_y = cursor_pos.y + theme.spacing / 2;
	auto clr_box_dimensions = rect( 0, 0, 10, 10 );

	/* centered text */
	binds::text( pos( cursor_pos.x + 6, area_center_y - text_size.h / 2 - 1 ), font, label, theme.text, false );

	/* color box */
	shapes::box( rect( cursor_pos.x + area.w - clr_box_dimensions.w - 6 - 1, area_center_y - clr_box_dimensions.h / 2 - 1, clr_box_dimensions.w + 1, clr_box_dimensions.h + 1 ), fade_timer, true, true, true, true, true, false );

	/* alpha bg */
	binds::fill_rect( rect( cursor_pos.x + area.w - clr_box_dimensions.w - 6, area_center_y - clr_box_dimensions.h / 2, clr_box_dimensions.w / 2, clr_box_dimensions.h / 2 ), color( 255, 255, 255, 255 ) );
	binds::fill_rect( rect( cursor_pos.x + area.w - clr_box_dimensions.w - 6 + clr_box_dimensions.h / 2, area_center_y - clr_box_dimensions.h / 2, clr_box_dimensions.w / 2, clr_box_dimensions.h / 2 ), color( 175, 175, 175, 255 ) );
	binds::fill_rect( rect( cursor_pos.x + area.w - clr_box_dimensions.w - 6, area_center_y, clr_box_dimensions.w / 2, clr_box_dimensions.h / 2 ), color( 175, 175, 175, 255 ) );
	binds::fill_rect( rect( cursor_pos.x + area.w - clr_box_dimensions.w - 6 + clr_box_dimensions.h / 2, area_center_y, clr_box_dimensions.w / 2, clr_box_dimensions.h / 2 ), color( 255, 255, 255, 255 ) );

	/* color preview */
	binds::fill_rect( rect( cursor_pos.x + area.w - clr_box_dimensions.w - 6, area_center_y - clr_box_dimensions.h / 2, clr_box_dimensions.w, clr_box_dimensions.h ), clr );

	/* draw overlay window */
	if ( opened ) {
		g_input = false;

		parent_window.draw_overlay( [ = ] ( ) {
			constexpr auto w = 233, h = 230;

			rect picker_area{ cursor_pos.x + area.w, cursor_pos.y + 26, w, h };
			rect picker_frame{ picker_area.x + 10, picker_area.y + 10, w - 20, h - 20 };
			rect picker_rect{ picker_frame.x, picker_frame.y, picker_frame.w - 15, picker_frame.h - 15 };
			rect brightness_bar{ picker_frame.x + picker_frame.w - 10, picker_frame.y, 10, picker_frame.h - 15 };
			rect alpha_bar{ picker_frame.x, picker_frame.y + picker_frame.h - 10, picker_frame.w - 15, 10 };
			rect preview_rect{ picker_frame.x + picker_frame.w - 10, picker_frame.y + picker_frame.h - 10, 11, 11 };

			/* draw window rect */
			binds::fill_rect( picker_area, theme.bg );

			/* title bar */
			binds::fill_rect( rect( picker_area.x, picker_area.y - 26, picker_area.w, 26 ), theme.title_bar );
			shapes::box( rect( picker_area.x, picker_area.y - 26, picker_area.w, 26 ), 0.0, false, false, true, false, false, false );
			binds::text( pos( picker_area.x + 4 /* + 24 */, picker_area.y - 28 + 6 ), win_font, OSTR( "color picker" ), theme.title_text, false );
			binds::rect( rect( picker_area.x - 1, picker_area.y - 1 - 26, picker_area.w + 1, picker_area.h + 1 + 26 ), color( 0, 0, 0, 255 ) );

			/* picker */ {
				color phase_clr = color( 255, 255, 255, 255 );

				for ( auto i = 0; i < picker_rect.w; i++ ) {
					for ( auto j = 0; j < picker_rect.h; j++ ) {
						calculate_phase( pos( i, j ), phase_clr );
						binds::line( pos( picker_rect.x + i, picker_rect.y + j ), pos( picker_rect.x + i + 1, picker_rect.y + j + 1 ), phase_clr );
					}
				}
			}

			pos mouse_pos;
			binds::mouse_pos( mouse_pos );

			const auto hovered = mouse_pos.x >= picker_rect.x && mouse_pos.y >= picker_rect.y && mouse_pos.x <= picker_rect.x + picker_rect.w - 1 && mouse_pos.y <= picker_rect.y + picker_rect.h;

			if ( GetAsyncKeyState( VK_LBUTTON ) && hovered )
				animate( picker_area );

			auto calc_alpha = static_cast< int >( std::sin( parent_panel.time * 2.0 * 3.141f ) * 80.0 + 175.0 );
			auto clr_inverted = color( std::abs( clr_raw.r - 255 ), std::abs( clr_raw.g - 255 ), std::abs( clr_raw.b - 255 ), calc_alpha );

			binds::gradient_rect( rect( brightness_bar.x, brightness_bar.y, brightness_bar.w, brightness_bar.h / 2 ), color( 255, 255, 255, 255 ), color( clr_raw.r, clr_raw.g, clr_raw.b, 255 ), false );
			binds::gradient_rect( rect( brightness_bar.x, brightness_bar.y + brightness_bar.h / 2, brightness_bar.w, brightness_bar.h / 2 ), color( clr_raw.r, clr_raw.g, clr_raw.b, 255 ), color( 0, 0, 0, 255 ), false );

			binds::fill_rect( alpha_bar, color( 255, 255, 255, 255 ) );

			binds::clip( rect( alpha_bar.x, alpha_bar.y, alpha_bar.w + 1, alpha_bar.h + 1 ), [ & ] ( ) {
				for ( auto i = 0; i <= alpha_bar.w / 5; i++ ) {
					binds::fill_rect( rect( alpha_bar.x + i * 5, ( i % 2 ) ? alpha_bar.y : ( alpha_bar.y + 5 ), 5, 5 ), color( 255, 255, 255, 255 ) );
					binds::fill_rect( rect( alpha_bar.x + i * 5, ( i % 2 ) ? ( alpha_bar.y + 5 ) : alpha_bar.y, 5, 5 ), color( 175, 175, 175, 255 ) );
				}
				} );

			binds::gradient_rect( alpha_bar, color( clr.r, clr.g, clr.b, 255 ), color( clr.r, clr.g, clr.b, 0 ), true );

			/* alpha bg */
			binds::fill_rect( rect( preview_rect.x, preview_rect.y, 5, 5 ), color( 255, 255, 255, 255 ) );
			binds::fill_rect( rect( preview_rect.x + 5, preview_rect.y, 5, 5 ), color( 175, 175, 175, 255 ) );
			binds::fill_rect( rect( preview_rect.x, preview_rect.y + 5, 5, 5 ), color( 175, 175, 175, 255 ) );
			binds::fill_rect( rect( preview_rect.x + 5, preview_rect.y + 5, 5, 5 ), color( 255, 255, 255, 255 ) );

			if ( mcursor_pos.x != 0 && mcursor_pos.y != 0 ) {
				binds::rect( rect( mcursor_pos.x - 3, mcursor_pos.y - 3, 6, 6 ), color( 0, 0, 0, calc_alpha ) );
				binds::rect( rect( mcursor_pos.x - 2, mcursor_pos.y - 2, 4, 4 ), clr_inverted );
			}

			if ( mcursor_pos1.x != 0 && mcursor_pos1.y != 0 ) {
				binds::rect( rect( mcursor_pos1.x - 3, mcursor_pos1.y - 3, 6, 6 ), color( 0, 0, 0, calc_alpha ) );
				binds::rect( rect( mcursor_pos1.x - 2, mcursor_pos1.y - 2, 4, 4 ), clr_inverted );
			}

			if ( mcursor_pos2.x != 0 && mcursor_pos2.y != 0 ) {
				binds::rect( rect( mcursor_pos2.x - 3, mcursor_pos2.y - 3, 6, 6 ), color( 0, 0, 0, calc_alpha ) );
				binds::rect( rect( mcursor_pos2.x - 2, mcursor_pos2.y - 2, 4, 4 ), clr_inverted );
			}

			/* color preview */
			binds::fill_rect( rect( preview_rect.x, preview_rect.y, preview_rect.w - 1, preview_rect.h - 1 ), clr );
			} );
	}
}