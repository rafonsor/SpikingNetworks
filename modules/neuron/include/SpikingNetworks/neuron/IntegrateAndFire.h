#pragma once

#include <utility>
#include <vector>

#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{

	class IntegrateAndFire : virtual public Soma
	{
	public:
		IntegrateAndFire()
			: Soma(), _delta(SpikingNetworks::core::get_time_delta_pointer()), _capacitance(1), _resistance(1), _tau(1), _threshold(0), _resting_v(-60), _v(-60), _reset_to_rest(true), _leaky(false)
		{}

		IntegrateAndFire(double* delta, double threshold, double capacitance, double resistance, double resting_v, bool reset_to_rest, bool leaky)
			: Soma(), _delta(delta), _capacitance(capacitance), _resistance(resistance), _tau(capacitance * resistance), _threshold(threshold), _resting_v(resting_v), _v(resting_v), _reset_to_rest(reset_to_rest)
		{}

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire(float current);

	protected:
		double compute(double current);

		const double* _delta;
		double _capacitance;
		double _resistance;
		double _tau;
		double _threshold;
		double _resting_v;
		double _v;
		bool _reset_to_rest;
		bool _leaky;
	};

}