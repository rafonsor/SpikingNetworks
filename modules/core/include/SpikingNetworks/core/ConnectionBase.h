#pragma once

#include <memory>

#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/Id.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/Propagate.h>


namespace SpikingNetworks::core
{

	class ConnectionBase :
		public Id,
		virtual public PropagateInterface<SpikeEvent>,
		public std::enable_shared_from_this<ConnectionBase>
	{
	public:
		SN_CLASS_POINTERS(ConnectionBase)
	};


}