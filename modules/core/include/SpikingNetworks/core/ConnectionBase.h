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
		virtual public PropagateInterface<SpikeEvent>
	{
	SN_CLASS_POINTERS(ConnectionBase)
	SN_CLASS_CLONE_ABSTRACT(ConnectionBase)
	};

}