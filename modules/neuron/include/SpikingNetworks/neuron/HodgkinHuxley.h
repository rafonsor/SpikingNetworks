#pragma once

#include <utility>
#include <vector>

#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{

	class HodgkinHuxley : virtual public Soma
	{
	public:
		HodgkinHuxley()
			: Soma(),
			_delta(SpikingNetworks::core::get_time_delta_pointer()),
			_capacitance(1.0),
			_threshold(0.0),
			_reset_to_equilibrium(true),
			_equilibrium_v(0.0),
			_equilibrium_k(0.0),
			_equilibrium_na(0.0),
			_equilibrium_leak(0.0),
			_g_k(1.0),
			_g_na(1.0),
			_g_leak(1.0),
			_v(0.0),
			_h(0.0),
			_m(0.0),
			_n(0.0),
			_alpha_h(0.0),
			_alpha_m(0.0),
			_alpha_n(0.0),
			_beta_h(0.0),
			_beta_m(0.0),
			_beta_n(0.0),
			_v_k(0.0),
			_v_na(0.0),
			_v_leak(0.0)
		{}

		HodgkinHuxley(double* delta, double capacitance, double threshold, double equilibrium_v, double equilibrium_k, double equilibrium_na, double equilibrium_leak, double g_k, double g_na, double g_leak, bool reset_to_equilibrium)
			: Soma(),
			_delta(delta),
			_capacitance(capacitance),
			_threshold(threshold),
			_reset_to_equilibrium(reset_to_equilibrium),
			_equilibrium_v(equilibrium_v),
			_equilibrium_k(equilibrium_k),
			_equilibrium_na(equilibrium_na),
			_equilibrium_leak(equilibrium_leak),
			_g_k(g_k),
			_g_na(g_na),
			_g_leak(g_leak),
			_v(equilibrium_v),
			_h(0.0),
			_m(0.0),
			_n(0.0),
			_alpha_h(0.0),
			_alpha_m(0.0),
			_alpha_n(0.0),
			_beta_h(0.0),
			_beta_m(0.0),
			_beta_n(0.0)
		{
			_v_k = _equilibrium_k - _equilibrium_v;
			_v_na = _equilibrium_na - _equilibrium_v;
			_v_leak = _equilibrium_leak - _equilibrium_v;
		}

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire(float current);

	protected:
		double compute(double current);

		const double* _delta;
		double _capacitance;
		double _threshold;
		bool _reset_to_equilibrium;

		// State variables
		double _v;
		double _h;
		double _m;
		double _n;

		// Equilibrium potentials
		double _equilibrium_v;
		double _equilibrium_k;
		double _equilibrium_na;
		double _equilibrium_leak;
		double _v_k;
		double _v_na;
		double _v_leak;

		// Ionic conductance constants
		double _g_k;
		double _g_na;
		double _g_leak;
		
		// Channels (in)activation rates
		double _alpha_h;
		double _alpha_m;
		double _alpha_n;
		double _beta_h;
		double _beta_m;
		double _beta_n;

	};

}