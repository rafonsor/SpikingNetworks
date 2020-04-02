#pragma once

#include <algorithm>
#include <execution>
#include <future>
#include <mutex>

#include <SpikingNetworks/core/Cell.h>
#include <SpikingNetworks/core/Event.h>


namespace SpikingNetworks::neuron
{
	class Neurite : virtual public SpikingNetworks::core::Segment
	{

	};

	class Dendrite : virtual public Neurite
	{

	};

	class Axon : virtual public Neurite
	{

	};

	class Spline : virtual public Neurite
	{

	};




	class Neuron : virtual public SpikingNetworks::core::Cell
	{
		Neuron() {}
		Neuron(Neuron&& other)
		{
			this->_body = std::move(other._body);
			this->_dendrites = other._dendrites;
			this->_axons = other._axons;
		}

	private:
		std::vector<SpikingNetworks::core::SegmentPtr> _dendrites;
		std::vector<SpikingNetworks::core::SegmentPtr> _axons;
	};
	using NeuronPtr = std::unique_ptr<Neuron>;

	template <typename SomaType, typename ... SomaArgs>
	NeuronPtr make_neuron(SomaArgs... args)
	{
		NeuronPtr neuron = std::make_shared<Neuron>();
		neuron->set_body(std::make_unique<SomaType>(std::forward<SomaArgs>(args)...));
		return neuron;
	}

	using Soma = SpikingNetworks::core::CellBody;
	class LeakyIntegrateAndFire : virtual public Soma
	{
	public:
		LeakyIntegrateAndFire(double* delta, double threshold, double capacitance, double resistance, double resting_v, bool reset_to_zero)
			: _delta(delta), _capacitance(capacitance), _resistance(resistance), _tau(capacitance*resistance), _threshold(threshold), _resting_v(resting_v), _v(resting_v), _reset_to_zero(reset_to_zero)
		{}

		void integrate(std::vector<SpikingNetworks::core::SpikeEvent> spikes);
		void fire(float potential);

	private:
		std::mutex _mutex;
		const double* _delta;
		double _capacitance;
		double _resistance;
		double _tau;
		double _threshold;
		double _resting_v;
		double _v;
		bool _reset_to_zero;
	};
}