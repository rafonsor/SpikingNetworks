#pragma once

#include <SpikingNetworks/core/Event.h>


namespace SpikingNetworks::core
{

	template <concepts::IsEvent EventT>
	class PropagateInterface
	{
	public:
		void virtual propagate(EventT spike) = 0;
	};

}