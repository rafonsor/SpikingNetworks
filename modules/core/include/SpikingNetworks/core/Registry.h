#pragma once

#include <thread>
#include <unordered_map>

#include <SpikingNetworks/core/CellPart.h>
#include <SpikingNetworks/core/ConnectionBase.h>
#include <SpikingNetworks/core/Id.h>


namespace SpikingNetworks::core
{

	class CellContentRegistry
	{
	public:
		bool add_segment(CellPart::Unique&& segment, UUID parent_id = UUID::null());
		CellPart::Ptr get_segment(UUID segment_id);
		bool remove_segment(UUID segment_id, bool propagate = false);
		size_t segment_count() const;

		std::unordered_map<UUID, CellPart::Unique> copy_segments();

		bool add_connection(ConnectionBase::Shared connection, UUID parent_id);
		ConnectionBase::Ptr get_connection(UUID connection_id);
		bool remove_connection(UUID connectio_id);
		size_t connection_count() const;

	protected:
		CellContentRegistry() : _segments(), _connections()
		{}

		std::mutex _mutex;
		std::unordered_map<UUID, CellPart::Unique> _segments;
		std::unordered_map<UUID, ConnectionBase::Shared> _connections;
	};

}