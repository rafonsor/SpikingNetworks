#pragma once

#include <utility>
#include <vector>

#include <SpikingNetworks/core/Connection.h>
#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Exceptions.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/ObjectProperties.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{
	class QuadraticIntegrateAndFireProperties : public SpikingNetworks::core::ObjectProperties
	{
	public:
		QuadraticIntegrateAndFireProperties(const std::string& yaml = "") : SpikingNetworks::core::ObjectProperties(yaml)
		{
			defineOptionalProperty<double>("tau", 1.0, "Temporal decay rate");
			defineOptionalProperty<double>("threshold_i", 0.0, "Input threshold");
			defineOptionalProperty<double>("threshold_x", 0.0, "State variable threshold");
			defineOptionalProperty<double>("reset_x", 0.0, "State variable reset value");
		}
	};

	/*
	Implementation of the Quadratic Integrate-and-Fire model as proposed by Hansel and Mato (2001), DOI: 10.1103/PhysRevLett.86.4175.
	*/
	class QuadraticIntegrateAndFire : virtual public Soma
	{
	SN_CLASS_POINTERS(QuadraticIntegrateAndFire)
	SN_CLASS_CLONE(QuadraticIntegrateAndFire)

	public:
		QuadraticIntegrateAndFire(const double* delta = SpikingNetworks::core::get_time_delta_pointer(), QuadraticIntegrateAndFireProperties properties = QuadraticIntegrateAndFireProperties());

		void update_properties(QuadraticIntegrateAndFireProperties properties);
		QuadraticIntegrateAndFireProperties extract_properties();

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire();
		void fire(double current);

	protected:
		double compute(double current);

		const double* _delta;
		double _tau;
		double _i_threshold;
		double _x_threshold;
		double _x_reset;
		double _x;
	};


	class QuadraticIntegrateAndFireSynapseProperties : public SpikingNetworks::core::ObjectProperties
	{
	public:
		QuadraticIntegrateAndFireSynapseProperties(const std::string& yaml = "") : SpikingNetworks::core::ObjectProperties(yaml)
		{
			defineOptionalProperty<double>("tau1", 6.0, "Temporal decay rate 1");
			defineOptionalProperty<double>("tau2", 3.0, "Temporal decay rate 2");
		}
	};

	class QuadraticIntegrateAndFireSynapse : virtual public SpikingNetworks::core::DirectedConnection2
	{
	SN_CLASS_POINTERS(QuadraticIntegrateAndFireSynapse)
	SN_CLASS_CLONE(QuadraticIntegrateAndFireSynapse)

	public:
		QuadraticIntegrateAndFireSynapse(SpikingNetworks::core::CellPart::Ptr segment_one, SpikingNetworks::core::CellPart::Ptr segment_two, float weight = 1.0, QuadraticIntegrateAndFireSynapseProperties properties = QuadraticIntegrateAndFireSynapseProperties());

		void propagate(SpikingNetworks::core::SpikeEvent spike);

	protected:
		// decay rates
		double _tau1;
		double _tau2;

	};

}