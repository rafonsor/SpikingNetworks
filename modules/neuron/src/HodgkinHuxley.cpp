
#include <algorithm>
#include <execution>
#include <mutex>

#include <SpikingNetworks/neuron/HodgkinHuxley.h>


namespace SpikingNetworks::neuron
{

	void HodgkinHuxley::integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes)
	{
		// Aggregated Pre-Synaptic Input
		float input = std::reduce(std::execution::par, spikes.begin(), spikes.end()).current;

		std::unique_lock lck(_mutex);

		_v += compute(input);
		if (_v >= _threshold)
		{
			_v = _reset_to_equilibrium ? _equilibrium_v : _v - _threshold;
			lck.unlock();
			SpikingNetworks::core::CellBody::fire();
		}
	}

	double HodgkinHuxley::compute(double input)
	{
		/* HH Neuron:
		 - v: C.dv/dt = I - g_K.n^4.(V - VK) - g_Na.m^3.h.(V - VNa) - g_leak.(V - Vleak).
		 - h: dh/dt = alpha_h(1 - h) - beta_h.h, with alpha_h = 0.007exp(V/20) and beta_h = 1/(exp((V+30)/10) + 1)
		 - m: dm/dt = alpha_m(1 - m) - beta_m.m, with alpha_m = 0.01(V + 25)/(exp((V+25)/10) - 1) and beta_m = 4exp(V/18)
		 - n: dn/dt = alpha_n(1 - n) - beta_n.n, with alpha_n = 0.01(V + 10)/(exp((V+10)/10) - 1) and beta_n = 0.125.exp(V/80)
		*/
		double v = _v;

		// update channels activation rates
		_alpha_h = 0.007 * std::exp(v / 20.0);
		_alpha_m = (0.01 * v + 0.25) / (std::exp((v + 25) / 10) - 1);
		_alpha_n = (0.01 * v + 0.1) / (std::exp((v + 10) / 10) - 1);

		// update channels inactivation rates
		_beta_h = 1 / (std::exp((v + 30) / 10) + 1);
		_beta_m = 4 * std::exp(v / 18);
		_beta_n = 0.125 * std::exp(v / 80);

		// compute channels state
		_h += (*_delta) * (_alpha_h - ((_alpha_h + _beta_h) * _h));
		_m += (*_delta) * (_alpha_m - ((_alpha_m + _beta_m) * _m));
		_n += (*_delta) * (_alpha_n - ((_alpha_n + _beta_n) * _n));

		// compute ionic currents
		double i_k = _g_k * std::pow(_n, 4) * (v - _v_k);
		double i_na = _g_na * std::pow(_m, 3) * _h * (v - _v_na);
		double i_leak = _g_leak * (v - _v_leak);

		// compute potential delta
		return (*_delta) * (input - i_k - i_na - i_leak) / _capacitance;
	}

	void HodgkinHuxley::fire(float current)
	{
		SpikingNetworks::core::SpikeEvent spike = SpikingNetworks::core::make_spike(id(), current);
		broadcast(spike);
	}

}