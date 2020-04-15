#pragma once

#include <utility>
#include <vector>

#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{

	class Izhikevich : virtual public Soma
	{
	public:
		Izhikevich()
			: Soma(),
			_delta(SpikingNetworks::core::get_time_delta_pointer()),
			_threshold(30.0),
			_v(-65.0),
			_u(8.0),
			_a(0.02),
			_b(0.25),
			_c(-65.0),
			_d(8.0)
		{}

		Izhikevich(double* delta, double threshold, double a, double b, double c, double d)
			: Soma(),
			_delta(delta),
			_threshold(threshold),
			_v(c),
			_u(d),
			_a(a),
			_b(b),
			_c(c),
			_d(d)
		{}

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire(double current);

	protected:
		double compute(double current);

		const double* _delta;
		double _threshold;

		// State variables
		double _v;
		double _u;

		// Intrinsic parameters
		double _a;
		double _b;
		double _c;
		double _d;

	};

}