
#include <algorithm>
#include <cmath>
#include <memory>

#include <SpikingNetworks/core/Event.h>


namespace SpikingNetworks::core
{

	SpikeEvent make_spike(UUID origin, double current, unsigned long delay)
	{
		return EventFactory<SpikeEvent>(origin, current, delay);
	}

	SpikeEvent operator+(const SpikeEvent& a, const SpikeEvent& b) noexcept
	{
		return SpikeEvent{ std::max(a.time, b.time), a.origin, a.current + b.current, a.delay + b.delay };
	}

}
