

#pragma once

#include <algorithm>
#include <execution>

#include <SpikingNetworks/core/Cell.h>


namespace SpikingNetworks::core
{
	// Segment class
	void Segment::propagate(SpikeEvent spike)
	{
		// Handle spike internally
		_propagate(spike);

		// Propagate through extra-cellular connections and intra-cellular segments a uniformly distributed potential (don't think this is correct, potential wouldn't be divided, current/charge yes)
		auto prop_fn = [&](auto it) { it.first->propagate(SpikeEvent{ spike.time, spike.potential * it.second / this->total_weight() }); };
		std::for_each(std::execution::par, this->_connections.begin(), this->_connections.end(), prop_fn);
		std::for_each(std::execution::par, this->_segments.begin(), this->_segments.end(), prop_fn);
	}

	bool Segment::add_segment(Segment* segment, float weight)
	{
		std::scoped_lock lck(_mutex);
		if (_segments.find(segment) == _segments.end())
			return false;
		_segments.insert({ segment, weight });
		return true;
	}

	bool Segment::remove_segment(Segment* segment)
	{
		std::scoped_lock lck(_mutex);
		return _segments.erase(segment) > 0;
	}

	float Segment::get_segment_weight(Segment* segment) const
	{
		auto it = _segments.find(segment);
		return it != _segments.end() ? it->second : 0.0f;
	}

	bool Segment::set_segment_weight(Segment* segment, float weight)
	{
		std::scoped_lock lck(_mutex);
		auto it = _segments.find(segment);
		if (it == _segments.end())
			return false;
		it->second = weight;
		return true;
	}

	bool Segment::update_segment_weight(Segment* segment, float weight_delta)
	{
		std::scoped_lock lck(_mutex);
		auto it = _segments.find(segment);
		if (it == _segments.end())
			return false;
		it->second += weight_delta;
		return true;
	}

	bool Segment::add_connection(Connection* connection, float weight)
	{
		std::scoped_lock lck(_mutex);
		if (_connections.find(connection) == _connections.end())
			return false;
		_connections.insert({ connection, weight });
		return true;
	}

	bool Segment::remove_connection(Connection* connection)
	{
		std::scoped_lock lck(_mutex);
		return _connections.erase(connection) > 0;
	}

	float Segment::get_connection_weight(Connection* connection) const
	{
		auto it = _connections.find(connection);
		return it != _connections.end() ? it->second : 0.0f;
	}

	bool Segment::set_connection_weight(Connection* connection, float weight)
	{
		std::scoped_lock lck(_mutex);
		auto it = _connections.find(connection);
		if (it == _connections.end())
			return false;
		it->second = weight;
		return true;
	}

	bool Segment::update_connection_weight(Connection* connection, float weight_delta)
	{
		std::scoped_lock lck(_mutex);
		auto it = _connections.find(connection);
		if (it == _connections.end())
			return false;
		it->second += weight_delta;
		return true;
	}

	float Segment::total_weight() const
	{
		return _total_weight;
	}

	size_t Segment::segments() const
	{
		return _segments.size();
	}

	size_t Segment::connections() const
	{
		return _segments.size();
	}

	// CellBody class
	void CellBody::progagate(SpikeEvent spike)
	{
		std::scoped_lock lck(_mutex);
		_spikes.insert(spike);
	}

	void CellBody::step()
	{
		while (_running)
		{
			std::vector<SpikeEvent> to_integrate;
			auto now = get_simulation_time();

			{
				std::scoped_lock lck(_mutex);
				auto it = _spikes.begin();

				while ((++it)->time <= now)
					to_integrate.push_back(*it);
				_spikes.erase(_spikes.begin(), it);
			}

			integrate(to_integrate);

			// should wait here until next time step
			// ...
		}
	}

	void CellBody::fire() {
		fire(1.0f);
	}

	void CellBody::stop()
	{
		_running = false;
	}

	
	// Cell class
	void Cell::set_body(CellBodyPtr body)
	{
		_body = std::move(body);
	}

}