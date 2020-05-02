#pragma once

#include <algorithm>
#include <execution>

#include <SpikingNetworks/core/CellPart.h>
#include <SpikingNetworks/core/Concepts.h>
#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Id.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/Registry.h>


namespace SpikingNetworks::core
{

	class Cell :
		public Id,
		public CellContentRegistry
	{
	SN_CLASS_POINTERS(Cell)

	public:
		
		Cell() : Id(), CellContentRegistry(), _body(nullptr)
		{}

		virtual ~Cell() {}

		Cell::Unique virtual clone();

		void set_body(CellPart::Unique&& body);
		const CellBody::Ptr get_body() const;

		void input(SpikeEvent& spike);
		template <typename C> requires concepts::IterableEvents<SpikeEvent, C>
		void input(C& train)
		{
			std::for_each(std::execution::par, train.begin(), train.end(), [&](auto& spike) {this->_body->propagate(spike); });
		}

	protected:
		CellBody::Ptr _body;
	};
}