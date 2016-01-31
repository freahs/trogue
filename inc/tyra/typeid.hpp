#ifndef TYRA_TYPE_ID_H
#define TYRA_TYPE_ID_H

#include "defs.hpp"

#include <cstdint>
#include <type_traits>

namespace tyra {

	template <typename B> class Type {
		static TypeId m_next_type_id;
	public:
		template <typename T> static const TypeId id() {
			static_assert(std::is_base_of<B, T>::value, "TypeId: T must be base of B");
			static const TypeId id = m_next_type_id++;
			return id;
		}
	};

	template <typename B> TypeId Type<B>::m_next_type_id = 0;
}

#endif