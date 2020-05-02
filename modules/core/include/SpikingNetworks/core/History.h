#pragma once

#include <vector>

#include <SpikingNetworks/core/Concepts.h>
#include <SpikingNetworks/core/Event.h>


namespace SpikingNetworks::core
{

	template <concepts::IsEvent EventT>
	class EventHistory
	{
	public:
		enum EventStatus
		{
			PROCESSED,
			EMITTED
		};

		EventHistory() : _history(), _processed(0), _emitted(0)
		{}

		std::vector<EventT> get_event_history() const
		{
			return _history;
		}

		const unsigned int get_event_count() const;
		const unsigned int get_processed_count() const;
		const unsigned int get_emitted_count() const;

		void reset_history();

	protected:
		void event_processed(EventT event)
		{
			_history.push_back({ event, EventStatus::PROCESSED });
			_processed++;
		}
		template <typename C> requires concepts::IterableEvents<EventT, C>
		void event_processed(C& events)
		{
			for (auto event : events)
				event_processed(event);
		}

		void event_emitted(EventT event)
		{
			_history.push_back({ event, EventStatus::EMITTED });
			_emitted++;
		}
		template <typename C> requires concepts::IterableEvents<EventT, C>
		void event_emitted(C& events)
		{
			for (auto event : events)
				event_emitted(event);
		}

		std::vector<std::pair<EventT, EventStatus>> _history;
		unsigned int _processed;
		unsigned int _emitted;
	};

	using SpikeHistory = EventHistory<SpikingNetworks::core::SpikeEvent>;

}