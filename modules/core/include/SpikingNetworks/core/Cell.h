#pragma once

#include <algorithm>
#include <execution>
#include <memory>

#include <SpikingNetworks/core/Concepts.h>
#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Id.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/Propagate.h>
#include <SpikingNetworks/core/Segment.h>


namespace SpikingNetworks::core
{

	class CellPart :
		public Id,
		public Segment<CellPart>,
		public virtual PropagateInterface<SpikeEvent>
	{
	public:
		SN_CLASS_POINTERS(CellPart)

		CellPart() : Id(), Segment()
		{}

		virtual ~CellPart() {}

		//CellPart::Unique virtual clone()
		//{
		//	auto segment = std::make_unique<Segment>();
		//	for (auto& segment : _segments)
		//		other->add_(segment.second.first->clone(), segment.second.second);
		//	for (auto& connection : _connections)
		//		other->add_(connection.second.first->clone(), connection.second.second);
		//	return other;
		//}

		void virtual propagate(SpikeEvent spike);
		void virtual broadcast(SpikeEvent& spike);

	protected:
		void virtual _propagate(SpikeEvent& spike) = 0;
		
	};


	class CellBody : virtual public CellPart
	{
	public:
		SN_CLASS_POINTERS(CellBody)

		CellBody() : CellPart()
		{}

		virtual ~CellBody() {}

		void virtual propagate(SpikeEvent spike);
		void virtual step();

		void virtual integrate(std::vector<SpikeEvent> spikes) = 0;
		
		// TODO: Still uncertain what the best approach would be: assume the conductance information to be available at the pre-synaptic cell and thus send current,
		// or that it's up to the post-synaptic cell to convert the pre-synaptic cell's potential into current. Of course, transmitting current will be simpler to
		// propagate across segments and connections.
		void virtual fire(float current) = 0;
		void virtual fire();

		void stop();

	protected:
		EventQueue<SpikeEvent> _spikes;
		bool _running;
	};

	class Cell : public Id
	{
	public:
		SN_CLASS_POINTERS(Cell)

		Cell() : Id(), _body() {}

		virtual ~Cell() {}

		//template <typename C, typename... Args>
		//typename C::Unique clone(Args... args)
		//{
		//	auto other = std::make_unique<C>(args...);
		//	other->set_body(_body->clone<std::remove_pointer<decltype(_body)>::type>());
		//	return other;
		//}

		void set_body(CellBody::Unique&& body);
		CellBody::Ptr get_body();

		template <typename C> requires concepts::IterableEvents<SpikeEvent, C>
		void input(C& train)
		{
			std::cout << "it a spike train" << std::endl;
			std::for_each(std::execution::par, train.begin(), train.end(), [&](auto& spike) {this->_body->propagate(spike); });
		}

		void input(SpikeEvent& spike);

	protected:
		CellBody::Unique _body;
	};
}