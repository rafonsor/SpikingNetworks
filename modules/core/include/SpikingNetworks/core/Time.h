#pragma once

#include <chrono>

namespace SpikingNetworks::core
{
	inline std::chrono::time_point<std::chrono::milliseconds> _SIMULATION_START_POINT(std::chrono::milliseconds(0));

	void set_simulation_time();
	void set_simulation_time(unsigned long time);

	unsigned long get_simulation_time();

}