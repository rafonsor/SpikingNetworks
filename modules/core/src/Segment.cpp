#include <SpikingNetworks/core/Segment.h>


namespace SpikingNetworks::core
{

	Segment::Segment(const Segment& other) : _mutex()
	{
		_parent = other._parent;
		_total_weight = other._total_weight;
		_segments = other._segments;
		_connections = other._connections;
	}

	bool Segment::remove_segment(UUID segment)
	{
		std::scoped_lock lck(_mutex);
		return _segments.erase(segment) > 0;
	}

	float Segment::get_segment_weight(UUID segment) const
	{
		auto it = _segments.find(segment);
		return it != _segments.end() ? it->second.second : 0.0f;
	}

	bool Segment::set_segment_weight(UUID segment, float weight)
	{
		std::scoped_lock lck(_mutex);
		auto it = _segments.find(segment);
		if (it == _segments.end())
			return false;
		_total_weight -= it->second.second;
		it->second.second = weight;
		_total_weight += weight;
		return true;
	}

	bool Segment::update_segment_weight(UUID segment, float weight_delta)
	{
		std::scoped_lock lck(_mutex);
		auto it = _segments.find(segment);
		if (it == _segments.end())
			return false;
		it->second.second += weight_delta;
		_total_weight += weight_delta;
		return true;
	}

	bool Segment::add_connection(ConnectionBase::Ptr connection, float weight = 1.0f)
	{
		std::scoped_lock lck(_mutex);
		if (_connections.find(connection->id()) != _connections.end())
			return false;
		_connections.insert({ connection->id(), std::pair{ connection, weight } });
		return true;
	}

	bool Segment::remove_connection(UUID connection)
	{
		std::scoped_lock lck(_mutex);
		return _connections.erase(connection) > 0;
	}

	ConnectionBase::Ptr Segment::get_connection(UUID connection)
	{
		std::scoped_lock lck(_mutex);
		auto it = _connections.find(connection);
		return it != _connections.end() ? it->second.first.get() : nullptr;
	}

	float Segment::get_connection_weight(UUID connection) const
	{
		std::scoped_lock lck(_mutex);
		auto it = _connections.find(connection);
		return it != _connections.end() ? it->second.second : 0.0f;
	}

	bool Segment::set_connection_weight(UUID connection, float weight)
	{
		std::scoped_lock lck(_mutex);
		auto it = _connections.find(connection);
		if (it == _connections.end())
			return false;
		_total_weight -= it->second.second;
		it->second.second = weight;
		_total_weight += weight;
		return true;
	}

	bool Segment::update_connection_weight(UUID connection, float weight_delta)
	{
		std::scoped_lock lck(_mutex);
		auto it = _connections.find(connection);
		if (it == _connections.end())
			return false;
		it->second.second += weight_delta;
		_total_weight += weight_delta;
		return true;
	}

	float Segment::total_weight() const
	{
		return _total_weight;
	}

	size_t Segment::segment_count() const
	{
		return _segments.size();
	}

	size_t Segment::connection_count() const
	{
		return _connections.size();
	}

	const std::unordered_map<UUID, std::pair<ConnectionBase::Ptr, float>>& Segment::connections() const
	{
		return _connections;
	}
	};

}