#pragma once

#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace SpikingNetworks::core
{
	class UUID : public boost::uuids::uuid
	{
	public:
		UUID() : boost::uuids::uuid(boost::uuids::random_generator()())
		{}
	};


	class Id
	{
	public:
		const UUID& id() const;

	protected:
		Id() : _id()
		{}

	private:
		const UUID _id;
	};

}

template <>
struct std::hash<SpikingNetworks::core::UUID>
{
	size_t operator() (const SpikingNetworks::core::UUID& id) const
	{
		return boost::hash<SpikingNetworks::core::UUID>()(id);
	}
};
