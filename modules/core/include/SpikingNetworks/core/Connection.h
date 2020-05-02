#pragma once

#include <memory>

#include <SpikingNetworks/core/Cell.h>
#include <SpikingNetworks/core/ConnectionBase.h>
#include <SpikingNetworks/core/Macros.h>


namespace SpikingNetworks::core
{

	class Connection2 : public virtual ConnectionBase
	{
	SN_CLASS_POINTERS(Connection2)

	protected:
		Connection2() : _segment_one(nullptr), _segment_two(nullptr), _weight(0.0f)
		{}

		Connection2(CellPart::Ptr segment_one, CellPart::Ptr segment_two, float weight = 1.0f);

		~Connection2()
		{
			//if (_segment_one != nullptr)
			//	_segment_one->remove_connection(id());
			//if (_segment_two != nullptr)
			//	_segment_two->remove_connection(id());
		}

		CellPart::Ptr _segment_one;
		CellPart::Ptr _segment_two;
		float _weight;
	};

	class DirectedConnection2 : public virtual Connection2
	{
	SN_CLASS_POINTERS(DirectedConnection2)
	SN_CLASS_CLONE(DirectedConnection2)

	public:
		DirectedConnection2(CellPart::Ptr segment_one, CellPart::Ptr segment_two, float weight) : Connection2(segment_one, segment_two, weight)
		{}

		void propagate(SpikeEvent event);
	};

	class UndirectedConnection2 : public virtual Connection2
	{
	SN_CLASS_POINTERS(UndirectedConnection2)
	SN_CLASS_CLONE(UndirectedConnection2)

	public:
		UndirectedConnection2(CellPart::Ptr segment_one, CellPart::Ptr segment_two, float weight) : Connection2(segment_one, segment_two, weight)
		{}

		void propagate(SpikeEvent event);
	};

	/*
	// Connections for networked stimuli
	class RemoteConnection : public virtual Connection {};

	// Connections for actuation or control of external objects
	class InterfaceConnection : public virtual Connection || RemoteConnction {};
	*/

}