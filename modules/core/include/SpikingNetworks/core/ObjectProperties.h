#pragma once

#include <unordered_set>

#include <yaml-cpp/yaml.h>

#include <SpikingNetworks/core/Exceptions.h>


namespace SpikingNetworks::core
{
	class ObjectProperties
	{
	public:
		ObjectProperties(const std::string& yaml) : _properties()
		{
			_node = YAML::Load(yaml);
		}

		template <typename ValueT>
		ValueT getProperty(std::string name)
		{
			if (_properties.find(name) == _properties.end())
				throw SpikingNetworks::exception::ObjectPropertyUnknown(name);
			return _node[name].as<ValueT>();
		}

		template <typename ValueT>
		void setProperty(std::string name, ValueT value)
		{
			if (_properties.find(name) == _properties.end())
				throw SpikingNetworks::exception::ObjectPropertyUnknown(name);
			_node[name] = value;
		}

	protected:
		template <typename ValueT>
		void defineRequiredProperty(std::string name, std::string description)
		{
			if (!_node[name])
				throw SpikingNetworks::exception::ObjectPropertyRequired(description);
			_properties.insert(name);
		}

		template <typename ValueT>
		void defineOptionalProperty(std::string name, ValueT value, std::string)
		{
			if (!_node[name])
				_node[name] = value;
			_properties.insert(name);
		}

	private:
		YAML::Node _node;
		std::unordered_set<std::string> _properties;
	};
}