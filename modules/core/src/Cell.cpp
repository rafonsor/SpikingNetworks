
#include <SpikingNetworks/core/Cell.h>


namespace SpikingNetworks::core
{

	Cell::Unique Cell::clone()
	{
		auto other = std::make_unique<Cell>();
		other->set_body(get_body()->clone());
		other->_segments = copy_segments();
		return std::move(other);
	}

	void Cell::set_body(CellPart::Unique&& body)
	{
		auto body_id = body->id();
		add_segment(std::move(body));
		_body = dynamic_cast<CellBody::Ptr>(get_segment(body_id));
	}

	const CellBody::Ptr Cell::get_body() const
	{
		return _body;
	}

	void Cell::input(SpikeEvent& spike)
	{
		_body->propagate(spike);
	}

}