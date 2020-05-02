#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include <SpikingNetworks/core/Cell.h>
#include <SpikingNetworks/core/Exceptions.h>
#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Macros.h>


namespace SpikingNetworks::neuron
{
	class Neurite : virtual public SpikingNetworks::core::CellPart
	{
	SN_CLASS_POINTERS(Neurite)
	SN_CLASS_CLONE_ABSTRACT(Neurite)

	protected:
		Neurite() : SpikingNetworks::core::CellPart()
		{}
	};

	class Dendrite : virtual public Neurite
	{
	SN_CLASS_POINTERS(Dendrite)
	SN_CLASS_CLONE(Dendrite)

	public:
		Dendrite() : Neurite()
		{}

	protected:
		void virtual _propagate(SpikingNetworks::core::SpikeEvent& spike) {}
	};

	class Axon : virtual public Neurite
	{
	SN_CLASS_POINTERS(Axon)
	SN_CLASS_CLONE(Axon)

	public:
		Axon() : Neurite()
		{}

	protected:
		void virtual _propagate(SpikingNetworks::core::SpikeEvent& spike) {}
	};

	class Spine : virtual public Neurite
	{
	SN_CLASS_POINTERS(Spine)
	};

	class Soma : virtual public SpikingNetworks::core::CellBody
	{
	SN_CLASS_POINTERS(Soma)
	SN_CLASS_CLONE_ABSTRACT(Soma)

	public:
		Soma() : SpikingNetworks::core::CellBody(), _dendrites(), _axons()
		{}

		void add_axon(Axon::Ptr axon)
		{
			_axons.push_back(axon);
		}
		void add_dendrite(Dendrite::Ptr dendrite)
		{
			_dendrites.push_back(dendrite);
		}

	protected:
		// Return Spike Event untouched
		void virtual _propagate(SpikingNetworks::core::SpikeEvent&) {}

	private:
		std::vector<SpikingNetworks::neuron::Dendrite::Ptr> _dendrites;
		std::vector<SpikingNetworks::neuron::Axon::Ptr> _axons;
	};


	// What about excitation/inhibition?
	class Neuron : virtual public SpikingNetworks::core::Cell
	{
	SN_CLASS_POINTERS(Neuron)

	public:
		Neuron() : SpikingNetworks::core::Cell()
		{}

		Neuron(Neuron&& other) noexcept;

		SpikingNetworks::core::Cell::Unique clone();
	};
	using NeuronPtr = std::unique_ptr<Neuron>;


	class ProgenitorNeuron
	{
	public:
		ProgenitorNeuron() : _current(nullptr), _parent_neurite(nullptr), _current_neurite(nullptr), _neurons(), _connection_segments()
		{}

		ProgenitorNeuron& make_neuron();

		template <typename Model> requires std::is_base_of_v<SpikingNetworks::core::CellBody, Model>
		ProgenitorNeuron& soma()
		{
			if (_current == nullptr)
				make_neuron();
			_current->set_body(std::make_unique<Model>());
			_current_neurite = _current->get_body();
			return *this;
		}

		template <typename Model, typename ModelProperties> requires std::is_base_of_v<SpikingNetworks::core::CellBody, Model> && std::is_base_of_v<SpikingNetworks::core::ObjectProperties, ModelProperties>
		ProgenitorNeuron& soma(ModelProperties properties)
		{
			if (_current == nullptr)
				make_neuron();
			_current->set_body(std::make_unique<Model>(properties));
			_current_neurite = _current->get_body();
			return *this;
		}

		template <typename Model> requires std::is_base_of_v<Axon, Model>
		ProgenitorNeuron& axon()
		{
			if (_current == nullptr)
				throw SpikingNetworks::exception::CellError("No neuron specified.");
			if (_current->get_body() == nullptr)
				throw SpikingNetworks::exception::SegmentError("Neuron without a soma, add a cell body first.");

			Soma::Ptr body = dynamic_cast<Soma::Ptr>(_current->get_body());
			auto neurite = std::make_unique<Model>();
			auto id = neurite->id();
			_current->add_segment(std::move(neurite), body->id());
			auto ptr = dynamic_cast<typename Model::Ptr>(_current->get_segment(id));
			body->add_axon(ptr);

			_parent_neurite = _current_neurite;
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

			Soma::Ptr body = dynamic_cast<Soma::Ptr>(_current->get_body());
			auto neurite = std::make_unique<Model>();
			auto id = neurite->id();
			_current->add_segment(std::move(neurite), body->id());
			auto ptr = dynamic_cast<typename Model::Ptr>(_current->get_segment(id));
			body->add_dendrite(ptr);

			_parent_neurite = _current_neurite;
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
			_current->add_segment(std::move(neurite), _current_neurite->id());

			_parent_neurite = _current_neurite;
			_current_neurite = dynamic_cast<typename Model::Ptr>(_current->get_segment(id));
			return *this;
		}

		ProgenitorNeuron& set_connection_point();

		template <typename ConnectionT> requires std::is_base_of_v<SpikingNetworks::core::ConnectionBase, ConnectionT>
		ProgenitorNeuron& create_connection2(float weight = 1.0)
		{
			if (_connection_segments.size() != 2)
				throw SpikingNetworks::exception::SegmentError("Cannot instantiate connection, wrong number of segments provided (" + std::to_string(_connection_segments.size()) + "): expected 2.");

			std::shared_ptr<ConnectionT> connection;
			connection = std::make_shared<ConnectionT>(_connection_segments[0].first, _connection_segments[1].first, weight);
			for (auto segment : _connection_segments)
				segment.second->add_connection(connection, segment.first->id());
			_connection_segments.clear();
			return *this;
		}

		template <typename ConnectionT> requires std::is_base_of_v<SpikingNetworks::core::ConnectionBase, ConnectionT>
		ProgenitorNeuron& create_connection3(float weight = 1.0)
		{
			if (_connection_segments.size() != 3)
				throw SpikingNetworks::exception::SegmentError("Cannot instantiate connection, wrong number of segments provided (" + std::to_string(_connection_segments.size()) + "): expected 3.");

			std::shared_ptr<ConnectionT> connection;
			connection = std::make_shared<ConnectionT>(_connection_segments[0].first, _connection_segments[1].first, _connection_segments[2].first,  weight);
			for (auto segment : _connection_segments)
				segment.second->add_connection(connection, segment.first->id());
			_connection_segments.clear();
			return *this;
		}

		template <typename ConnectionT> requires std::is_base_of_v<SpikingNetworks::core::ConnectionBase, ConnectionT>
		ProgenitorNeuron& create_connection4(float weight = 1.0)
		{
			if (_connection_segments.size() != 4)
				throw SpikingNetworks::exception::SegmentError("Cannot instantiate connection, wrong number of segments provided (" + std::to_string(_connection_segments.size()) + "): expected 4.");

			std::shared_ptr<ConnectionT> connection;
			connection = std::make_shared<ConnectionT>(_connection_segments[0].first, _connection_segments[1].first, _connection_segments[2].first, _connection_segments[3].first,  weight);
			for (auto segment : _connection_segments)
				segment.second->add_connection(connection, segment.first->id());
			_connection_segments.clear();
			return *this;
		}

		ProgenitorNeuron& done();

		std::vector<Neuron::Unique> all();


	private:
		Neuron::Ptr _current;
		std::vector<Neuron::Unique> _neurons;
		SpikingNetworks::core::CellPart::Ptr _parent_neurite;
		SpikingNetworks::core::CellPart::Ptr _current_neurite;
		std::vector<std::pair<SpikingNetworks::core::CellPart::Ptr, Neuron::Ptr>> _connection_segments;
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