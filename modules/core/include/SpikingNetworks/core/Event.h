#pragma once

#include <algorithm>
#include <memory>
#include <set>

#include <SpikingNetworks/core/Concepts.h>
#include <SpikingNetworks/core/Id.h>
#include <SpikingNetworks/core/Time.h>


namespace SpikingNetworks::core
{
	struct Event
	{
		// Time in milliseconds since start of simulation (_SIMULATION_START_POINT)
		unsigned long time;
	};

	struct SpikeEvent : Event
	{
		UUID origin;
		double current = 0;
		unsigned long delay = 0;
	};

	using EventPtr = std::shared_ptr<Event>;
	using SpikeEventPtr = std::shared_ptr<SpikeEvent>;

	template <class EventType>
	using EventQueue = std::multiset<EventType>;

	template <typename EventType, typename... EventArgs>
	std::shared_ptr<EventType> EventPtrFactory(EventArgs... arguments)
	{
		return std::make_shared<EventType>(get_simulation_time(), arguments...);
	}

	template <typename EventType>
	std::shared_ptr<EventType> EventPtrFactory()
	{
		return std::make_shared<EventType>(get_simulation_time());
	}

	template <typename EventType, typename... EventArgs>
	EventType EventFactory(EventArgs... arguments)
	{
		return EventType{ get_simulation_time(), arguments... };
	}

	template <typename EventType>
	EventType EventFactory()
	{
		return EventType{ get_simulation_time() };
	}

	SpikeEvent make_spike(UUID origin, double current, unsigned long delay = 0);

	constexpr bool operator<(const Event& a, const Event& b) noexcept
	{
		return a.time < b.time;
	}

	constexpr bool operator<(const SpikeEvent& a, const SpikeEvent& b) noexcept
	{
		return (a.time + a.delay) < (b.time + b.delay) && a.current < b.current;
	}

	SpikeEvent operator+(const SpikeEvent& a, const SpikeEvent& b) noexcept;


	namespace concepts
	{
		template <typename EventType>
		concept IsEvent = _IsDerived<SpikingNetworks::core::Event, EventType>;
	}
}
