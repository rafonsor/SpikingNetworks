
#include <algorithm>
#include <execution>
#include <mutex>

#include <SpikingNetworks/neuron/Izhikevich.h>


namespace SpikingNetworks::neuron
{

	void Izhikevich::integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes)
	{
		// Aggregated Pre-Synaptic Input
		float input = std::reduce(std::execution::par, spikes.begin(), spikes.end()).current;

		std::unique_lock lck(_mutex);

		_v += compute(input);
		if (_v >= _threshold)
		{
			// Izhikevich's reset equations: v <- c; u <- u + d
			_v = _c;
			_u += _d;
			lck.unlock();
			SpikingNetworks::core::CellBody::fire();
		}
	}

	double Izhikevich::compute(double input)
	{
		/* Izhikevich Neuron:
		 - v: dv/dt = 0.04v² + 5v + 140 - u + I
		 - u: du/dt = a(bu - v)
		*/
		double v = _v;
		double u = _u;

		_u += (*_delta) * _a * (_b * u - v);
		return (*_delta) * (0.04*v**2(_leaky ? (_resting_v - _v) / _tau : 0) + input / _capacitance);
	}

	void Izhikevich::fire(float current)
	{
		SpikingNetworks::core::SpikeEvent spike = SpikingNetworks::core::make_spike(id(), current);
		broadcast(spike);
	}

}