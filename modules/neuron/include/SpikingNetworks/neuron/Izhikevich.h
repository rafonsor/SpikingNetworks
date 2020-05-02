#pragma once

#include <vector>

#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/ObjectProperties.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{
	class IzhikevichProperties : public SpikingNetworks::core::ObjectProperties
	{
	public:
		IzhikevichProperties(const std::string& yaml = "") : SpikingNetworks::core::ObjectProperties(yaml)
		{
			defineOptionalProperty<double>("threshold", 30.0, "Membrane potential threshold");
			defineOptionalProperty<double>("a", 0.02, "Recovery time scale constant");
			defineOptionalProperty<double>("a", 0.25, "Recovery sensitivity constant");
			defineOptionalProperty<double>("c", -60.0, "Membrane potential at rest");
			defineOptionalProperty<double>("d", 8.0, "Membrane potential recovery constant");
		}
	};

	class Izhikevich : virtual public Soma
	{
	SN_CLASS_POINTERS(Izhikevich)
	SN_CLASS_CLONE(Izhikevich)

	public:
		Izhikevich(const double* delta = SpikingNetworks::core::get_time_delta_pointer(), IzhikevichProperties properties = IzhikevichProperties());

		void update_properties(IzhikevichProperties properties);
		IzhikevichProperties extract_properties();

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