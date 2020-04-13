#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include <SpikingNetworks/core/Cell.h>
#include <SpikingNetworks/core/Exceptions.h>
#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Macros.h>
#include <iostream>


namespace SpikingNetworks::neuron
{
	class Neurite : virtual public SpikingNetworks::core::CellPart
	{
	public:
		SN_CLASS_POINTERS(Neurite)

	protected:
		Neurite() : SpikingNetworks::core::CellPart()
		{}
	};

	class Dendrite : virtual public Neurite
	{
	public:
		SN_CLASS_POINTERS(Dendrite)

		Dendrite() : Neurite()
		{}

	protected:
		void virtual _propagate(SpikingNetworks::core::SpikeEvent & spike) {}
	};

	class Axon : virtual public Neurite
	{
	public:
		SN_CLASS_POINTERS(Axon)

		Axon() : Neurite()
		{}

	protected:
		void virtual _propagate(SpikingNetworks::core::SpikeEvent& spike) {}
	};

	class Spine : virtual public Neurite
	{
	public:
		SN_CLASS_POINTERS(Spine)
	};



	class Neuron : virtual public SpikingNetworks::core::Cell
	{
	public:
		SN_CLASS_POINTERS(Neuron)

			Neuron() : SpikingNetworks::core::Cell(), _dendrites(), _axons() { std::cout << "I am being constructed" << std::endl; }

		Neuron(Neuron&& other) noexcept
		{
			this->_body = std::move(other._body);
			this->_dendrites = other._dendrites;
			this->_axons = other._axons;
		}

		//Neuron::Unique clone()
		//{
		//	auto other = SpikingNetworks::core::Cell::clone<Neuron>();
		//	/*for (auto segment : _dendrites)
		//	{
		//		auto clone = segment->clone<std::remove_pointer<decltype(segment)>::type>();
		//		auto id = clone->id();
		//		other->get_body()->add_segment(std::move(clone));
		//		other->add_dendrite(other->get_body()->get_segment(id));
		//	}
		//	for (auto segment : _axons)
		//		other->add_axon(segment->clone<std::remove_pointer<decltype(segment)>::type>().get());*/
		//	return other;
		//}

		void add_axon(Axon::Ptr axon)
		{
			_axons.push_back(axon);
		}
		void add_dendrite(Dendrite::Ptr dendrite)
		{
			_dendrites.push_back(dendrite);
		}

	private:
		std::vector<SpikingNetworks::core::CellPart::Ptr> _dendrites;
		std::vector<SpikingNetworks::core::CellPart::Ptr> _axons;
	};
	using NeuronPtr = std::unique_ptr<Neuron>;

	template <typename SomaType, typename ... SomaArgs>
	NeuronPtr NeuronFactory(SomaArgs... args)
	{
		NeuronPtr neuron = std::make_unique<Neuron>();
		neuron->set_body(std::make_unique<SomaType>(std::forward<SomaArgs>(args)...));
		return std::move(neuron);
	}

	class Soma : virtual public SpikingNetworks::core::CellBody
	{
	public:
		SN_CLASS_POINTERS(Neuron)

		Soma() : SpikingNetworks::core::CellBody()
		{}

	protected:
		// Return Spike Event untouched
		void virtual _propagate(SpikingNetworks::core::SpikeEvent&) {}
	};


	class ProgenitorNeuron
	{
	public:
		ProgenitorNeuron() : _current(nullptr), _neurons() {}

		ProgenitorNeuron& make_neuron()
		{
			auto neuron = std::make_unique<Neuron>();
			_current = neuron.get();
			_neurons.push_back(std::move(neuron));
			_parent_neurite = nullptr;
			_current_neurite = nullptr;
			return *this;
		}

		template <typename Model> requires std::is_base_of_v<SpikingNetworks::core::CellBody, Model>
		ProgenitorNeuron& soma()
		{
			if (_current == nullptr)
				make_neuron();
			auto soma = std::make_unique<Model>();
			_current->set_body(std::move(soma));
			return *this;
		}

		template <typename Model> requires std::is_base_of_v<Axon, Model>
		ProgenitorNeuron& axon()
		{
			if (_current == nullptr)
				throw SpikingNetworks::exception::CellError("No neuron specified.");
			if (_current->get_body() == nullptr)
				throw SpikingNetworks::exception::SegmentError("Neuron without a soma, add a cell body first.");

			auto neurite = std::make_unique<Model>();
			auto id = neurite->id();
			_current->get_body()->add_segment(std::move(neurite));
			auto ptr = dynamic_cast<typename Model::Ptr>(_current->get_body()->get_segment(id));
			_current->add_axon(ptr);

			_parent_neurite = nullptr;
			_current_neurite = ptr;
			return *this;
		}

		template <typename Model> requires std::is_base_of_v<Dendrite, Model>
		ProgenitorNeuron& dendrite()
		{
			if (_current == nullptr)
				throw SpikingNetworks::exception::CellError("No neuron specified.");
			if (_current->get_body() == nullptr)
				throw SpikingNetworks::exception::SegmentError("Neuron without a soma, add a cell body first.");

			auto neurite = std::make_unique<Model>();
			auto id = neurite->id();
			_current->get_body()->add_segment(std::move(neurite));
			auto ptr = dynamic_cast<typename Model::Ptr>(_current->get_body()->get_segment(id));
			_current->add_dendrite(ptr);

			_parent_neurite = nullptr;
			_current_neurite = ptr;
			return *this;
		}

		template <typename Model> requires std::is_base_of_v<Neurite, Model>
		ProgenitorNeuron& segment()
		{
			if (_current_neurite == nullptr)
				throw SpikingNetworks::exception::SegmentError("No base neurite set, create an axon or dendrite first.");

			auto neurite = std::make_unique<Model>();
			auto id = neurite->id();
			_current_neurite->add_segment(std::move(neurite));

			_parent_neurite = _current_neurite;
			_current_neurite = dynamic_cast<typename Model::Ptr>(_current_neurite->get_segment(id));
			return *this;
		}

		ProgenitorNeuron& done()
		{
			if (_parent_neurite != nullptr)
			{
				_current_neurite = _parent_neurite;
				_parent_neurite = _current_neurite->get_parent();
			}
			else
				_current_neurite = nullptr;
			return *this;
		}

		std::vector<Neuron::Unique> all()
		{
			_current = nullptr;
			return std::move(_neurons);
		}


	private:
		Neuron::Ptr _current;
		SpikingNetworks::core::CellPart::Ptr _parent_neurite;
		SpikingNetworks::core::CellPart::Ptr _current_neurite;
		std::vector<Neuron::Unique> _neurons;
	};

	/*
	Purely code-based:
	Progenitor.make_neuron()
		.set_soma<Leaky>({props})
		.set_axon<Segment>{props})
			.add_segment(...)
				.add_segment(...)
				.done()
			.add_segment(...)
			.done()
		.set_dendrite<Segment>(...)
		.get()

		*where {props} is an object that could be shared
	Or
	Progenitor.make_neuron()
		.set_soma<Leaky>({props})
		.set_axon<Axon>(graph<from, to, prop_id>, map<prop_id>{props})
		.set_dendrite<Dendrite>(graph<from, to, prop_id>, map<prop_id>{props})
		.get()
	
	Using a JSON/YAML/XML config file

	Progenitor.make_neuron_from_file(config)
	where file has structure:
	- structure (graph):
		[(from, to)]
	- elements (dict):
		- id: prop_id
	- properties (dict):
		- id: 
			- type: IntegrateAndFire/Dendrite/...
			- config (dict)

	This is a nice versatile approach, but keep in mind that for now, less is needed.
	For example, a standard LIF neuron only requires defining the soma.
	*/
}