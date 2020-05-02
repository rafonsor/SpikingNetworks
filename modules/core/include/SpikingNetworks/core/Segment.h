#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <utility>

#include <SpikingNetworks/core/ConnectionBase.h>
#include <SpikingNetworks/core/Id.h>


namespace SpikingNetworks::core
{

	template <typename SegmentT>
	class Segment
	{
	public:
		Segment() : _mutex(), _total_weight(0), _parent(nullptr), _segments(), _connections()
		{}

		Segment(const Segment& other) : _mutex()
		{
			_parent = other._parent;
			_total_weight = other._total_weight;
			_segments = other._segments;
			_connections = other._connections;
		}

		~Segment()
		{}

		void set_parent(SegmentT* parent)
		{
			_parent = parent;
		}

		SegmentT* get_parent() const
		{
			return _parent;
		}

		bool add_segment(SegmentT* segment, float weight = 1.0f)
		{
			std::scoped_lock lck(_mutex);
			if (_segments.find(segment->id()) != _segments.end())
				return false;
			_segments.insert({ segment->id() , std::pair{ segment, weight } });
			return true;
		}

		bool remove_segment(UUID segment)
		{
			std::scoped_lock lck(_mutex);
			return _segments.erase(segment) > 0;
		}

		SegmentT* get_segment(UUID segment)
		{
			std::scoped_lock lck(_mutex);
			auto it = _segments.find(segment);
			return it != _segments.end() ? it->second.first.get() : nullptr;
		}

		float get_segment_weight(UUID segment) const
		{
			auto it = _segments.find(segment);
			return it != _segments.end() ? it->second.second : 0.0f;
		}

		bool set_segment_weight(UUID segment, float weight)
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

		bool update_segment_weight(UUID segment, float weight_delta)
		{
			std::scoped_lock lck(_mutex);
			auto it = _segments.find(segment);
			if (it == _segments.end())
				return false;
			it->second.second += weight_delta;
			_total_weight += weight_delta;
			return true;
		}

		bool add_connection(ConnectionBase::Ptr connection, float weight = 1.0f)
		{
			std::scoped_lock lck(_mutex);
			if (_connections.find(connection->id()) != _connections.end())
				return false;
			_connections.insert({ connection->id(), std::pair{ connection, weight } });
			return true;
		}

		bool remove_connection(UUID connection)
		{
			std::scoped_lock lck(_mutex);
			return _connections.erase(connection) > 0;
		}

		ConnectionBase::Ptr get_connection(UUID connection)
		{
			std::scoped_lock lck(_mutex);
			auto it = _connections.find(connection);
			return it != _connections.end() ? it->second.first.get() : nullptr;
		}

		float get_connection_weight(UUID connection) const
		{
			std::scoped_lock lck(_mutex);
			auto it = _connections.find(connection);
			return it != _connections.end() ? it->second.second : 0.0f;
		}

		bool set_connection_weight(UUID connection, float weight)
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

		bool update_connection_weight(UUID connection, float weight_delta)
		{
			std::scoped_lock lck(_mutex);
			auto it = _connections.find(connection);
			if (it == _connections.end())
				return false;
			it->second.second += weight_delta;
			_total_weight += weight_delta;
			return true;
		}

		float total_weight() const
		{
			return _total_weight;
		}

		size_t segment_count() const
		{
			return _segments.size();
		}

		size_t connection_count() const
		{
			return _connections.size();
		}

		const std::unordered_map<UUID, std::pair<SegmentT*, float>>& segments() const
		{
			return _segments;
		}

		void set_segments(const std::unordered_map<UUID, std::pair<SegmentT*, float>>& segments)
		{
			_segments = segments;
		}

		const std::unordered_map<UUID, std::pair<ConnectionBase::Ptr, float>>& connections() const
		{
			return _connections;
		}

	protected:
		std::mutex _mutex;
		float _total_weight;
		SegmentT* _parent;
		std::unordered_map<UUID, std::pair<SegmentT*, float>> _segments;
		std::unordered_map<UUID, std::pair<ConnectionBase::Ptr, float>> _connections;
	};

}