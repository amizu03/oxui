#ifndef OXUI_LABEL_HPP
#define OXUI_LABEL_HPP

#include <memory>
#include <vector>
#include <functional>
#include "object.hpp"
#include "../types/types.hpp"

namespace oxui {
	class label : public obj {
	public:
		str text;

		label ( const str& text ) {
			this->text = text;
			type = object_label;
		}

		~label ( ) {}

		void think( );
		void draw( ) override;
	};
}

#endif // OXUI_LABEL_HPP