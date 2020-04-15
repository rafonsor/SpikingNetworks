#pragma once

#include <utility>
#include <vector>

#include <SpikingNetworks/core/Connection.h>
#include <SpikingNetworks/core/Exceptions.h>
#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{
	/*
	Implementation of the Quadratic Integrate-and-Fire model as proposed by Hansel and Mato (2001), DOI: 10.1103/PhysRevLett.86.4175.
	*/
	class QuadraticIntegrateAndFire : virtual public Soma
	{
	public:
		QuadraticIntegrateAndFire()
			: Soma(), _delta(SpikingNetworks::core::get_time_delta_pointer()), _tau(1.0), _i_threshold(0.0), _x_threshold(0.0), _x_reset(0.0), _x(0.0)
		{}

		QuadraticIntegrateAndFire(double* delta, double tau, double i_threshold, double x_threshold, double x_reset)
			: Soma(), _delta(delta), _tau(tau), _i_threshold(i_threshold), _x_threshold(x_threshold), _x_reset(x_reset), _x(x_reset)
		{}

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire();
		void fire(float current);

	protected:
		double compute(double current);

		const double* _delta;
		double _tau;
		double _i_threshold;
		double _x_threshold;
		double _x_reset;
		double _x;
	};

	class QuadraticIntegrateAndFireSynapse : virtual public SpikingNetworks::core::DirectedConnection2
	{
		//QuadraticIntegrateAndFireSynapse()
		//	: SpikingNetworks::core::DirectedConnection2(),
		//	_tau1(6.0),
		//	_tau2(3.0)
		//{}

		QuadraticIntegrateAndFireSynapse(SpikingNetworks::core::CellPart::Ptr segment_one, SpikingNetworks::core::CellPart::Ptr segment_two, float weight, double tau1, double tau2)
			: SpikingNetworks::core::DirectedConnection2(segment_one, segment_two, weight),
			_tau1(tau1),
			_tau2(tau2)
		{
			if (tau1 == tau2)
				throw SpikingNetworks::exception::InvalidParameter("decay rates tau1 and tau2 cannot be equal, will cause a division by 0.");
		}

		void propagate(SpikingNetworks::core::SpikeEvent spike);

	protected:
		// decay rates
		double _tau1;
		double _tau2;

	};

}