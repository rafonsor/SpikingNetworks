#pragma once

#include <memory>

#include <SpikingNetworks/core/Cell.h>
#include <SpikingNetworks/core/ConnectionBase.h>
#include <SpikingNetworks/core/Macros.h>


namespace SpikingNetworks::core
{

	class Connection2 : public virtual ConnectionBase
	{
	public:
		SN_CLASS_POINTERS(Connection2)

	protected:
		Connection2() : _segment_one(nullptr), _segment_two(nullptr), _weight(0.0f)
		{}

		Connection2(CellPart::Ptr segment_one, CellPart::Ptr segment_two, float weight = 1.0f) : _segment_one(segment_one), _segment_two(segment_two), _weight(weight)
		{}

		~Connection2()
		{
			if (_segment_one != nullptr)
				_segment_one->remove_connection(id());
			if (_segment_two != nullptr)
				_segment_two->remove_connection(id());
		}

		CellPart::Ptr _segment_one;
		CellPart::Ptr _segment_two;
		float _weight;
	};

	class DirectedConnection2 : public virtual Connection2
	{
		DirectedConnection2(CellPart::Ptr segment_one, CellPart::Ptr segment_two, float weight) : Connection2(segment_one, segment_two, weight)
		{
			segment_one->add_connection(shared_from_this(), weight);
		}
	};

	class UndirectedConnection2 : public virtual Connection2
	{
		UndirectedConnection2(CellPart::Ptr segment_one, CellPart::Ptr segment_two, float weight) : Connection2(segment_one, segment_two, weight)
		{
			segment_one->add_connection(shared_from_this(), weight);
			segment_two->add_connection(shared_from_this(), weight);
		}
	};

	/*
	// Connections for networked stimuli
	class RemoteConnection : public virtual Connection {};

	// Connections for actuation or control of external objects
	class InterfaceConnection : public virtual Connection || RemoteConnction {};
	*/

}