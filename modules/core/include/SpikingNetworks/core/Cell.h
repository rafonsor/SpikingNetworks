#pragma once

#include <algorithm>
#include <execution>
#include <unordered_map>

#include <SpikingNetworks/core/Event.h>


namespace SpikingNetworks::core
{


	class CellPart
	{
	public:
		virtual ~CellPart() {}

		void virtual propagate(SpikeEvent spike) = 0;
	};
	using CellPartPtr = std::unique_ptr<CellPart>;


	class Connection : virtual public CellPart, std::enable_shared_from_this<Connection>
	{
	public:
		Connection() : _segment_one(nullptr), _segment_two(nullptr), _weight(0.0f)
		{}

		Connection(CellPart* segment_one, CellPart* segment_two, float weight = 1.0f) : _segment_one(segment_one), _segment_two(segment_two), _weight(weight)
		{}

	protected:
		CellPart* _segment_one;
		CellPart* _segment_two;
		float _weight;
	};

	class DirectConnection : public Connection
	{
		DirectConnection(CellPart* segment_one, CellPart* segment_two, float weight) : Connection(segment_one, segment_two, weight)
		{}
	};

	class Segment : virtual public CellPart
	{
	public:
		Segment() : _mutex(), _total_weight(0), _segments(), _connections()
		{}

		void virtual propagate(SpikeEvent spike);

		bool add_segment(Segment* segment, float weight = 1.0f);
		bool remove_segment(Segment* segment);
		float get_segment_weight(Segment* segment) const;
		bool set_segment_weight(Segment* segment, float weight);
		bool update_segment_weight(Segment* segment, float weight_delta);

		bool add_connection(Connection* connection, float weight = 1.0f);
		bool remove_connection(Connection* connection);
		float get_connection_weight(Connection* connection) const;
		bool set_connection_weight(Connection* connection, float weight);
		bool update_connection_weight(Connection* connection, float weight_delta);

		float total_weight() const;
		size_t segments() const;
		size_t connections() const;

	protected:
		void virtual _propagate(SpikeEvent& spike) = 0;

		std::mutex _mutex;
		float _total_weight;
		std::unordered_map<Segment*, float> _segments;
		std::unordered_map<Connection*, float> _connections;

	};
	using SegmentPtr = std::shared_ptr<Segment>;

	class CellBody : virtual public Segment
	{
	public:
		void virtual progagate(SpikeEvent spike);
		void virtual step();

		void virtual integrate(std::vector<SpikeEvent> spikes) = 0;
		
		void virtual fire(float potential) = 0;
		void virtual fire();

		void stop();

	protected:
		EventQueue<SpikeEvent> _spikes;
		bool _running;
	};
	using CellBodyPtr = std::unique_ptr<CellBody>;


	class Cell
	{
	public:
		virtual ~Cell() {}

		void set_body(CellBodyPtr body);

	protected:
		CellBodyPtr _body;
	};
}