
#include <algorithm>
#include <execution>
#include <mutex>

#include <SpikingNetworks/neuron/IntegrateAndFire.h>


namespace SpikingNetworks::neuron
{

	void IntegrateAndFire::integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes)
	{
		// Aggregated Pre-Synaptic Input
		double input = std::reduce(std::execution::par, spikes.begin(), spikes.end()).current;

		std::unique_lock lck(_mutex);

		_v += compute(input);
		if (_v >= _threshold)
		{
			_v = _reset_to_rest ? _resting_v : _v - _threshold;
			lck.unlock();
			SpikingNetworks::core::CellBody::fire();
		}
	}

	double IntegrateAndFire::compute(double input)
	{
		// LIF Neuron: tau (dv/dt) = -(V - Vrest) + RI, with tau = RC.
		// Direct approximation with delta t: dv = dt * ( (Vrest - V)/tau + I/C )
		return (*_delta) * ( (_leaky? (_resting_v - _v) / _tau : 0) + input / _capacitance);
	}

	void IntegrateAndFire::fire(double current)
	{
		SpikingNetworks::core::SpikeEvent spike = SpikingNetworks::core::make_spike(id(), current);
		broadcast(spike);
	}

}