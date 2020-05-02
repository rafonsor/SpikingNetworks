
#include <SpikingNetworks/neuron/Neuron.h>


namespace SpikingNetworks::neuron
{
	Neuron::Neuron(Neuron&& other) noexcept
	{
		_segments = std::move(other._segments);
		_connections = std::move(other._connections);
		_body = other._body;
	}

	SpikingNetworks::core::Cell::Unique Neuron::clone()
	{
		auto other = std::make_unique<Neuron>();
		other->set_body(get_body()->clone());
		other->_segments = copy_segments();
		return std::move(other);
	}

	ProgenitorNeuron& ProgenitorNeuron::make_neuron()
	{
		auto neuron = std::make_unique<Neuron>();
		_current = neuron.get();
		_neurons.push_back(std::move(neuron));
		_parent_neurite = nullptr;
		_current_neurite = nullptr;
		return *this;
	}

	ProgenitorNeuron& ProgenitorNeuron::set_connection_point()
	{
		if (_current_neurite == nullptr)
			throw SpikingNetworks::exception::SegmentError("No base neurite set, create an axon or dendrite first.");

		_connection_segments.push_back({ _current_neurite, _current });
		return *this;
	}

	ProgenitorNeuron& ProgenitorNeuron::done()
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

	std::vector<Neuron::Unique> ProgenitorNeuron::all()
	{
		_current = nullptr;
		return std::move(_neurons);
	}
}