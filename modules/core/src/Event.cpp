
#include <algorithm>
#include <memory>

#include <SpikingNetworks/core/Event.h>


namespace SpikingNetworks::core
{

	bool comp(Event& a, Event& b)
	{
		return a.time <= b.time;
	}

	bool equiv(Event& a, Event& b)
	{
		return a.time == b.time;
	}

}

bool operator<(const SpikingNetworks::core::Event& a, const SpikingNetworks::core::Event& b) noexcept
{
	return a.time < b.time;
}

SpikingNetworks::core::SpikeEvent operator+(const SpikingNetworks::core::SpikeEvent& a, const SpikingNetworks::core::SpikeEvent& b) noexcept
{
	return { std::max(a.time, b.time), a.potential + b.potential };
}

bool operator<(const SpikingNetworks::core::SpikeEvent& a, const SpikingNetworks::core::SpikeEvent& b) noexcept
{
	return a.time < b.time;
}
