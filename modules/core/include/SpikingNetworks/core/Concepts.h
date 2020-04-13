#pragma once

#include <concepts>
#include <type_traits>

namespace SpikingNetworks::core::concepts
{

	template <typename C>
	concept Iterable = requires(C a, typename C::iterator it) {
		a.begin();
		a.end();
		++it;
	};

	template <typename EventType, typename C>
	concept IterableEvents = std::is_same<typename C::value_type, EventType>::value && requires(C container, typename C::iterator it) {
		container.begin();
		container.end();
		++it;
	};

	template <typename Base, typename Derived>
	concept _IsDerived = std::is_base_of_v<Base, Derived>;

}