#include "groupbox.hpp"
#include "../panels/panel.hpp"
#include "shapes.hpp"
#include "../themes/purple.hpp"

void oxui::group::think( ) {
	auto& parent_window = find_parent< window >( object_window );
	animate( rect( parent_window.cursor_pos.x, parent_window.cursor_pos.y, area.w, area.h ) );

	/* calculate how much room we have for new objects */
	max_height = 26 + theme.spacing * 2;

	std::for_each (
		objects.begin ( ),
		objects.end ( ),
		[ & ] ( std::shared_ptr< obj >& child ) {
		max_height += child->calc_height ( );
	} );

	if ( area.h > max_height )
		max_height = area.h;

	/* manage scrolling */
	if ( shapes::hovering ( rect ( parent_window.cursor_pos.x, parent_window.cursor_pos.y, area.w, area.h ) ) && parent_window.scroll_delta != 0.0 ) {
		scroll_offset += parent_window.scroll_delta * 10.0;
		scroll_offset = std::clamp< double > ( scroll_offset, 0, static_cast< double >( max_height - area.h ) );
	}
}

void oxui::group::draw( ) {
	think( );

	auto& parent_panel = find_parent< panel >( object_panel );
	auto& parent_window = find_parent< window >( object_window );

	auto& tfont = parent_panel.fonts [ OSTR( "title" ) ];
	auto& font = parent_panel.fonts [ OSTR( "object" ) ];

	/* reset draw cursor pos */
	auto& cursor_pos = parent_window.cursor_pos;

	const auto og_cursor_pos = cursor_pos;

	shapes::box( rect( cursor_pos.x, cursor_pos.y, area.w, area.h ), fade_timer, true, true, true, true, true );
	binds::fill_rect( rect( cursor_pos.x, cursor_pos.y, area.w, 26 ), theme.title_bar );
	shapes::box( rect( cursor_pos.x, cursor_pos.y, area.w, 26 ), fade_timer, true, true, false, true, true, false );
	shapes::box( rect( cursor_pos.x, cursor_pos.y, area.w, 26 ), 0.0, false, false, true, false, false, false );
	binds::text( pos( cursor_pos.x + 6, cursor_pos.y + 4 ), tfont, title, theme.title_text, false );

	/* move all objects inside group */
	cursor_pos.x += theme.spacing;
	cursor_pos.y += theme.spacing * 2;

	/* draw group objects */
	binds::clip( rect( og_cursor_pos.x, og_cursor_pos.y + 26 + 1, area.w, area.h - 26 - 1 ), [ & ] ( ) {
		cursor_pos.y -= scroll_offset;

		std::for_each(
			objects.begin( ),
			objects.end( ),
			[ & ] ( std::shared_ptr< obj >& child ) {
				child->area = rect( 0, 0, area.w - theme.spacing * 2, theme.spacing );

				child->draw_ex( );
			}
		);
	} );

	/* too many items; we need to be able to scroll down on this groupbox */
	if ( max_height - area.h > 0 ) {
		/* let's draw a gradient next to the top to let people know that there are more items on the way on the top */
		if ( scroll_offset > 0 ) {
			binds::gradient_rect ( rect( og_cursor_pos.x + 1, og_cursor_pos.y + 26 + 1, area.w - 2, theme.spacing / 2 ), color ( 0, 0, 0, 100 ), color ( 0, 0, 0, 0 ), false );
		}

		/* let's draw a gradient next to the bottom to let people know that there are more items on the way on the bottom */
		if ( scroll_offset < static_cast< double >( max_height - area.h ) ) {
			binds::gradient_rect ( rect ( og_cursor_pos.x + 1, og_cursor_pos.y + area.h - theme.spacing / 2 - 1, area.w - 2, theme.spacing / 2 ), color ( 0, 0, 0, 0 ), color ( 0, 0, 0, 100 ), false );
		}
	}
}