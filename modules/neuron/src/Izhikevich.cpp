
#include <algorithm>
#include <execution>
#include <mutex>

#include <SpikingNetworks/neuron/Izhikevich.h>


namespace SpikingNetworks::neuron
{

	Izhikevich::Izhikevich(const double* delta, IzhikevichProperties properties)
		: Soma(), _delta(delta)
	{
		update_properties(properties);
		_v = _c;
		_u = _d;
	}

	void Izhikevich::update_properties(IzhikevichProperties properties)
	{
		_threshold = properties.getProperty<double>("threshold");
		_a = properties.getProperty<double>("a");
		_b = properties.getProperty<double>("b");
		_c = properties.getProperty<double>("c");
		_d = properties.getProperty<double>("d");
	}

	IzhikevichProperties Izhikevich::extract_properties()
	{
		IzhikevichProperties properties;
		properties.setProperty<double>("threshold", _threshold);
		properties.setProperty<double>("a", _a);
		properties.setProperty<double>("b", _b);
		properties.setProperty<double>("c", _c);
		properties.setProperty<double>("d", _d);
		return properties;
	}

	void Izhikevich::integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes)
	{
		// Aggregated Pre-Synaptic Input
		double input = std::reduce(std::execution::par, spikes.begin(), spikes.end()).current;

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
		 - u: du/dt = a(bv - u)
		*/
		double v = _v;
		double u = _u;

		_u += (*_delta) * _a * (_b * v - u);
		return (*_delta) * ((0.04 * std::pow(v, 2)) + (5 * v) + 140 - u + input);
	}

	void Izhikevich::fire(double current)
	{
		SpikingNetworks::core::SpikeEvent spike = SpikingNetworks::core::make_spike(id(), current);
		broadcast(spike);
	}

}