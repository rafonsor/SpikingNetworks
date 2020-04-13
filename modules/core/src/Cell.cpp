
#include <SpikingNetworks/core/Cell.h>
#include <iostream>


namespace SpikingNetworks::core
{
	// CellPart class
	void CellPart::propagate(SpikeEvent spike)
	{
		// Handle spike internally
		_propagate(spike);

		// Forward spike
		broadcast(spike);
	}

	void CellPart::broadcast(SpikeEvent& spike)
	{
		// Propagate through extra-cellular connections and intra-cellular segments uniformly distributed current
		auto prop_fn = [&](auto& it) { it.second.first->propagate(SpikeEvent{ spike.time, spike.origin, spike.current * it.second.second / this->total_weight() }); };
		std::for_each(std::execution::par, this->_connections.begin(), this->_connections.end(), prop_fn);
		std::for_each(std::execution::par, this->_segments.begin(), this->_segments.end(), prop_fn);
	}

	// CellBody class
	void CellBody::propagate(SpikeEvent spike)
	{
		std::scoped_lock lck(_mutex);
		_spikes.insert(spike);
	}

	void CellBody::step()
	{
		while (_running)
		{
			std::vector<SpikeEvent> to_integrate;
			unsigned long now = get_simulation_time();

			{
				std::scoped_lock lck(_mutex);
				auto it = _spikes.begin();

				while ((++it)->time <= now)
					to_integrate.push_back(*it);
				_spikes.erase(_spikes.begin(), it);
			}

			integrate(to_integrate);

			// should wait here until next time step
			wait_until<long long, std::chrono::microseconds>(((long long) now + 1) * MILLI_TO_MICRO_SECONDS);
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
	void Cell::set_body(CellBody::Unique&& body)
	{
		_body = std::move(body);
	}

	CellBody::Ptr Cell::get_body()
	{
		return _body.get();
	}

	void Cell::input(SpikeEvent& spike)
	{
		_body->propagate(spike);
	}

}