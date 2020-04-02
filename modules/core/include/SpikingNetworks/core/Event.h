#pragma once

#include <algorithm>
#include <memory>
#include <set>

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
		float potential = 0;
	};

	using EventPtr = std::shared_ptr<Event>;
	using SpikeEventPtr = std::shared_ptr<SpikeEvent>;

	bool comp(Event& a, Event& b);
	bool equiv(Event& a, Event& b);

	SpikeEvent operator+(const SpikeEvent& a, const SpikeEvent& b);

	template <typename EventType, typename... EventArgs>
	std::shared_ptr<EventType> EventFactory(EventArgs... arguments)
	{
		return std::make_shared<EventType>(get_simulation_time(), arguments...);
	}

	template <typename EventType>
	std::shared_ptr<EventType> EventFactory()
	{
		return std::make_shared<EventType>(get_simulation_time());
	}

	template <class EventType>
	using EventQueue = std::multiset<EventType>;
}

bool operator<(const SpikingNetworks::core::Event& a, const SpikingNetworks::core::Event& b) noexcept;

SpikingNetworks::core::SpikeEvent operator+(const SpikingNetworks::core::SpikeEvent& a, const SpikingNetworks::core::SpikeEvent& b) noexcept;

constexpr bool std::less<SpikingNetworks::core::SpikeEvent>::operator()(const SpikingNetworks::core::SpikeEvent& a, const SpikingNetworks::core::SpikeEvent& b) const
{
	return a.time < b.time;
}
