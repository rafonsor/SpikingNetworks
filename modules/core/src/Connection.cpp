
#include <SpikingNetworks/core/Connection.h>

namespace SpikingNetworks::core
{
	Connection2::Connection2(CellPart::Ptr segment_one, CellPart::Ptr segment_two, float weight) : _segment_one(segment_one), _segment_two(segment_two), _weight(weight)
	{
		_segment_one->add_connection(this, weight);
		_segment_two->add_connection(this, weight);
	}

	void DirectedConnection2::propagate(SpikeEvent event)
	{
		if (event.origin == _segment_one->id() && event.origin != _segment_two->id())
			_segment_two->propagate(event);
	}

	void UndirectedConnection2::propagate(SpikeEvent event)
	{
		if (event.origin == _segment_one->id())
			_segment_two->propagate(event);
		else
			_segment_one->propagate(event);
	}
}