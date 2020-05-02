#include <SpikingNetworks/core/History.h>


namespace SpikingNetworks::core
{

	const unsigned int EventHistory::get_event_count() const
	{
		return _history.size();
	}

	const unsigned int EventHistory::get_processed_count() const
	{
		return _processed;
	}

	const unsigned int EventHistory::get_emitted_count() const
	{
		return _emitted;
	}

	void EventHistory::reset_history()
	{
		_history.clear();
		_processed = 0;
		_emitted = 0;
	}

}