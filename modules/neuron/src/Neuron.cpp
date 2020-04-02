
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{

	void LeakyIntegrateAndFire::integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes)
	{
		// Aggregated Pre-Synaptic Potential
		float potential = std::reduce(std::execution::par, ++spikes.begin(), spikes.end()).potential;

		std::unique_lock lck(_mutex);
		// LIF Neuron: tau (dv/dt) = -v + R(potential), with tau = RC.
		_v += *_delta * (-_v / _tau + potential / _capacitance);
		if (_v >= _threshold)
		{
			_v = _reset_to_zero ? 0 : _v - _threshold;
			lck.unlock();
			Soma::fire();
		}
	}
	void LeakyIntegrateAndFire::fire(float potential)
	{

	}

}