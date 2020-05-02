#pragma once

#include <memory>

#include <SpikingNetworks/core/Concepts.h>
#include <SpikingNetworks/core/Event.h>
#include <SpikingNetworks/core/History.h>
#include <SpikingNetworks/core/Id.h>
#include <SpikingNetworks/core/Macros.h>
#include <SpikingNetworks/core/Propagate.h>
#include <SpikingNetworks/core/Segment.h>


namespace SpikingNetworks::core
{

	class CellPart :
		public Id,
		public Segment<CellPart>,
		public virtual PropagateInterface<SpikeEvent>,
		public SpikeHistory
	{
	SN_CLASS_POINTERS(CellPart)
	SN_CLASS_CLONE_ABSTRACT(CellPart)

	public:
		CellPart() : Id(), Segment()
		{}

		virtual ~CellPart()
		{}

		void virtual propagate(SpikeEvent spike);
		void virtual broadcast(SpikeEvent& spike);

	protected:
		void virtual _propagate(SpikeEvent& spike) = 0;
	};


	class CellBody : virtual public CellPart
	{
	SN_CLASS_POINTERS(CellBody)
	SN_CLASS_CLONE_ABSTRACT(CellBody)

	public:
		CellBody() : CellPart(), _spikes(), _running(false)
		{}

		virtual ~CellBody()
		{}

		void virtual propagate(SpikeEvent spike);
		void virtual step();

		void virtual integrate(std::vector<SpikeEvent> spikes) = 0;

		void virtual fire(double current) = 0;
		void virtual fire();

		void stop();

	protected:
		EventQueue<SpikeEvent> _spikes;
		bool _running;
	};

}