
#include <algorithm>
#include <cmath>
#include <execution>
#include <mutex>

#include <SpikingNetworks/neuron/QuadraticIntegrateAndFire.h>


namespace SpikingNetworks::neuron
{

	void QuadraticIntegrateAndFire::integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes)
	{
		// Aggregated Pre-Synaptic Input
		double input = std::reduce(std::execution::par, spikes.begin(), spikes.end()).current;

		std::unique_lock lck(_mutex);

		_x += compute(input);
		if (_x > _x_threshold)
		{
			_x = _x_reset;
			lck.unlock();
			SpikingNetworks::core::CellBody::fire();
		}
	}

	double QuadraticIntegrateAndFire::compute(double input)
	{
		// QIF Neuron: tau (dv/dt) = X² - I_th + I.
		return (*_delta) * (std::pow(_x, 2) - _i_threshold + input) / _tau;
	}

	void QuadraticIntegrateAndFire::fire(float current)
	{
		SpikingNetworks::core::SpikeEvent spike = SpikingNetworks::core::make_spike(id(), current);
		broadcast(spike);
	}


	void QuadraticIntegrateAndFireSynapse::propagate(SpikingNetworks::core::SpikeEvent spike)
	{
		/*
		The total pre-synaptic current, Isyn, is determined over a time horizon [0, t] by summing all spikes prior to time t:
		Isyn = w.sum(f(t-t_spike)) with f(t) = (1/(tau1 - tau2)) * (exp(t/tau1) - exp(t/tau2)) * heavyside(t)

		As we're only interested in the instantaneous pre-synaptic current for a single spike , this can be reduced to:
		Isyn = w.f(0) = w if delay is null (t=t_spike), otherwise Isyn = w.f(delay)
		*/
		spike.current = _weight;
		if (spike.delay > 0)
			spike.current *= std::exp(spike.delay) / (_tau1 - _tau2);

		_segment_two->propagate(spike);
	}

}