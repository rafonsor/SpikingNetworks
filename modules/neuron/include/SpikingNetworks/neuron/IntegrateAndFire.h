#pragma once

#include <vector>

#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/ObjectProperties.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{
	class IntegrateAndFireProperties : public SpikingNetworks::core::ObjectProperties
	{
	public:
		IntegrateAndFireProperties(const std::string& yaml = "") : SpikingNetworks::core::ObjectProperties(yaml)
		{
			defineOptionalProperty<double>("capacitance", 1.0, "Membrane capacitance");
			defineOptionalProperty<double>("resistance", 1.0, "Channels resistance");
			defineOptionalProperty<double>("threshold", 0.0, "Membrane potential threshold");
			defineOptionalProperty<bool>("reset_to_equilibrium", true, "Reset membrane potential to equilibrium value");
			defineOptionalProperty<double>("equilibrium_v", -60.0, "Membrane potential at rest");
			defineOptionalProperty<bool>("leaky", true, "Membrane potential leakage");
		}
	};

	class IntegrateAndFire : virtual public Soma
	{
	SN_CLASS_POINTERS(IntegrateAndFire)
	SN_CLASS_CLONE(IntegrateAndFire)

	public:
		IntegrateAndFire(const double* delta = SpikingNetworks::core::get_time_delta_pointer(), IntegrateAndFireProperties properties = IntegrateAndFireProperties());

		void update_properties(IntegrateAndFireProperties properties);
		IntegrateAndFireProperties extract_properties();

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire(double current);

	protected:
		double compute(double current);

		const double* _delta;
		double _capacitance;
		double _resistance;
		double _tau;
		double _threshold;
		double _equilibrium_v;
		double _v;
		bool _reset_to_equilibrium;
		bool _leaky;
	};

}