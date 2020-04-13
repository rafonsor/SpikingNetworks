#pragma once

#include <memory>
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

		bool add_segment(std::unique_ptr<SegmentT>&& segment, float weight = 1.0f)
		{
			std::scoped_lock lck(_mutex);
			if (_segments.find(segment->id()) != _segments.end())
				return false;
			_segments.insert({ segment->id() , std::pair{ std::move(segment), weight } });
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

		bool add_connection(ConnectionBase::Shared connection, float weight)
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

	protected:
		std::mutex _mutex;
		float _total_weight;
		SegmentT* _parent;

		/*
		On the question of which kind of pointer and ownership a segment should have on other segments...
		Well it seems logical that ownership should extend outwards from the soma, in which case, by storing segments as unique_ptr,
		deleting a soma should propagate the destruction of all other segments in a manner that makes sense, whereas connection would be share_ptr.
		This is conditioned on the current approach where neurons only have direct ownership of their soma and pointers to beginning of neurites (and all connections?)

		... OK main issue with this is then we only have a path outwards, meaning input segments cannot properly propagate spikes inwards....
		so we could have two sets of segments or maybe a ptr to parent?
		then we have to override broadcast behaviour so that e.g. dendritic propagates through parent while axon normally to segments and spine only to connections.

		i don't think the parent route will be a problem for Segments because there should only be bifurcations and no circular links (for connections yes)
		*/
		std::unordered_map<UUID, std::pair<std::unique_ptr<SegmentT>, float>> _segments;
		std::unordered_map<UUID, std::pair<ConnectionBase::Shared, float>> _connections;
	};

}