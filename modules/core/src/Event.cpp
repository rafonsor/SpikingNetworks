
#include <algorithm>
#include <memory>

#include <SpikingNetworks/core/Event.h>


namespace SpikingNetworks::core
{

	SpikeEvent make_spike(UUID origin, float current)
	{
		return EventFactory<SpikeEvent>(origin, current);
	}

	SpikeEvent operator+(const SpikeEvent& a, const SpikeEvent& b) noexcept
	{
		return SpikeEvent{ std::max(a.time, b.time), a.origin, a.current + b.current };
	}

}
