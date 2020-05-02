
#include <algorithm>
#include <execution>
#include <mutex>

#include <SpikingNetworks/neuron/IntegrateAndFire.h>


namespace SpikingNetworks::neuron
{

	IntegrateAndFire::IntegrateAndFire(const double* delta, IntegrateAndFireProperties properties)
		: Soma(), _delta(delta)
	{
		update_properties(properties);

		_tau = _capacitance * _resistance;
		_v = _equilibrium_v;
	}

	void IntegrateAndFire::update_properties(IntegrateAndFireProperties properties)
	{
		_capacitance = properties.getProperty<double>("capacitance");
		_resistance = properties.getProperty<double>("resistance");
		_threshold = properties.getProperty<double>("threshold");
		_equilibrium_v = properties.getProperty<double>("equilibrium_v");
		_reset_to_equilibrium = properties.getProperty<bool>("reset_to_equilibrium");
		_leaky = properties.getProperty<bool>("leaky");
	}

	IntegrateAndFireProperties IntegrateAndFire::extract_properties()
	{
		IntegrateAndFireProperties properties;
		properties.setProperty<double>("capacitance", _capacitance);
		properties.setProperty<double>("resistance", _resistance);
		properties.setProperty<double>("threshold", _threshold);
		properties.setProperty<double>("equilibrium_v", _equilibrium_v);
		properties.setProperty<bool>("reset_to_equilibrium", _reset_to_equilibrium);
		properties.setProperty<bool>("leaky", _leaky);
		return properties;
	}

	void IntegrateAndFire::integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes)
	{
		// Aggregated Pre-Synaptic Input
		double input = std::reduce(std::execution::par, spikes.begin(), spikes.end()).current;

		std::unique_lock lck(_mutex);

		_v += compute(input);
		if (_v >= _threshold)
		{
			_v = _reset_to_equilibrium ? _equilibrium_v : _v - _threshold;
			lck.unlock();
			SpikingNetworks::core::CellBody::fire();
		}
	}

	double IntegrateAndFire::compute(double input)
	{
		// LIF Neuron: tau (dv/dt) = -(V - Vrest) + RI, with tau = RC.
		// Direct approximation with delta t: dv = dt * ( (Vrest - V)/tau + I/C )
		return (*_delta) * ( (_leaky? (_equilibrium_v - _v) / _tau : 0) + input / _capacitance);
	}

	void IntegrateAndFire::fire(double current)
	{
		SpikingNetworks::core::SpikeEvent spike = SpikingNetworks::core::make_spike(id(), current);
		broadcast(spike);
	}

}