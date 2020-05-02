
#include <SpikingNetworks/core/Id.h>


namespace SpikingNetworks::core
{

	UUID UUID::null()
	{
		return UUID(boost::uuids::nil_generator()());
	}

	bool UUID::is_null()
	{
		return is_nil();
	}

	const UUID& Id::id() const
	{
		return _id;
	}

}
