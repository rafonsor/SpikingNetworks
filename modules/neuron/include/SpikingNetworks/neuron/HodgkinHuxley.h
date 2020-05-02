#pragma once

#include <vector>

#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/ObjectProperties.h>
#include <SpikingNetworks/core/Time.h>
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{
	class HodkginHuxleyProperties : public SpikingNetworks::core::ObjectProperties
	{
	public:
		HodkginHuxleyProperties(const std::string& yaml = "") : SpikingNetworks::core::ObjectProperties(yaml)
		{
			defineOptionalProperty<double>("capacitance", 1.0, "Membrane capacitance");
			defineOptionalProperty<double>("threshold", 0.0, "Membrane potential threshold");
			defineOptionalProperty<bool>("reset_to_equilibrium", true, "Reset membrane potential to equilibrium value");
			defineOptionalProperty<double>("equilibrium_v", 0.0, "Membrane potential at rest");
			defineOptionalProperty<double>("equilibrium_k", 0.0, "Equilibrium potential of Potassium ions");
			defineOptionalProperty<double>("equilibrium_na", 0.0, "Equilibrium potential of Sodium ions");
			defineOptionalProperty<double>("equilibrium_leak", 0.0, "Equilibrium potential of other ions");
			defineOptionalProperty<double>("conductance_k", 1.0, "Potassium ionic conductance");
			defineOptionalProperty<double>("conductance_na", 1.0, "Sodium ionic conductance");
			defineOptionalProperty<double>("conductance_leak", 1.0, "Ionic conductance of other ions");
		}
	};

	class HodgkinHuxley : virtual public Soma
	{
	SN_CLASS_POINTERS(HodgkinHuxley)
	SN_CLASS_CLONE(HodgkinHuxley)

	public:
		HodgkinHuxley(const double* delta = SpikingNetworks::core::get_time_delta_pointer(), HodkginHuxleyProperties properties = HodkginHuxleyProperties());

		void update_properties(HodkginHuxleyProperties properties);
		HodkginHuxleyProperties extract_properties();

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire(double current);

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