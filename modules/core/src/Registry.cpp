#include <SpikingNetworks/core/Registry.h>


namespace SpikingNetworks::core
{

	bool CellContentRegistry::add_segment(CellPart::Unique&& segment, UUID parent_id)
	{
		std::scoped_lock lck(_mutex);
		if (parent_id.is_null())
		{
			_segments.insert({ segment->id(), std::move(segment) });
			return true;
		}

		auto& parent = _segments.find(parent_id);
		if (parent == _segments.end())
			return false;

		// create a link on both sides
		if (parent->second->add_segment(segment.get()/*, weight...*/))
		{
			segment->set_parent(parent->second.get());
			_segments.insert({ segment->id(), std::move(segment) });
			return true;
		}
		return false;
	}

	CellPart::Ptr CellContentRegistry::get_segment(UUID segment_id)
	{
		std::scoped_lock lck(_mutex);
		auto& segment = _segments.find(segment_id);
		return segment != _segments.end() ? segment->second.get() : nullptr;
	}

	bool CellContentRegistry::remove_segment(UUID segment_id, bool propagate)
	{
		std::scoped_lock lck(_mutex);
		if (propagate)
		{
			auto segment = _segments.find(segment_id);
			if (segment == _segments.end())
				return false;

			for (auto child : segment->second->segments())
				remove_segment(child.first, true);
			for (auto child : segment->second->connections())
				remove_connection(child.first);
		}

		return _segments.erase(segment_id) > 0;
	}

	size_t CellContentRegistry::segment_count() const
	{
		return _segments.size();
	}

	std::unordered_map<UUID, CellPart::Unique> CellContentRegistry::copy_segments()
	{
		std::unordered_map<UUID, CellPart::Unique> clones;
		std::unordered_map<UUID, UUID> index_map;
		for (auto& segment : _segments)
		{
			auto clone = segment.second->clone();
			index_map[segment.first] = clone->id();
			clones.insert({ clone->id(), std::move(clone) });
		}

		std::function<void(CellPart::Ptr)> replace_pointers = [&](CellPart::Ptr segment) {
			std::unordered_map<UUID, std::pair<CellPart::Ptr, float>> updated_segments;
			for (auto& child : segment->segments())
			{
				auto new_id = index_map[child.first];
				updated_segments[new_id] = { clones[new_id].get(), child.second.second };
				replace_pointers(child.second.first);
			}

			auto new_parent_id = index_map[segment->get_parent()->id()];
			segment->set_parent(clones[new_parent_id].get());
			segment->set_segments(updated_segments);
		};

		return clones;
	}

	bool CellContentRegistry::add_connection(ConnectionBase::Shared connection, UUID parent_id)
	{
		std::scoped_lock lck(_mutex);
		auto parent = _segments.find(parent_id);
		if (parent == _segments.end())
			return false;

		_connections.insert({connection->id(), connection});
		return true;
	}

	ConnectionBase::Ptr CellContentRegistry::get_connection(UUID connection_id)
	{
		std::scoped_lock lck(_mutex);
		auto& connection = _connections.find(connection_id);
		return connection != _connections.end() ? connection->second.get() : nullptr;
	}

	bool CellContentRegistry::remove_connection(UUID connection_id)
	{
		return _connections.erase(connection_id);
	}

	size_t CellContentRegistry::connection_count() const
	{
		return _connections.size();
	}

}