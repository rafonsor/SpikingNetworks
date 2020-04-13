
#include <algorithm>

#include <SpikingNetworks/core/Time.h>


namespace SpikingNetworks::core
{

	void set_simulation_time()
	{
		_SIMULATION_START_POINT = std::chrono::time_point<std::chrono::milliseconds>();
	}

	void set_simulation_time(unsigned long time)
	{
		_SIMULATION_START_POINT = std::chrono::time_point<std::chrono::milliseconds>(std::chrono::milliseconds(time));
	}

	unsigned long get_simulation_time()
	{
		return (unsigned long) (std::chrono::time_point<std::chrono::milliseconds>() - _SIMULATION_START_POINT).count();
	}

	void set_time_delta(double delta)
	{
		_SIMULATION_TIME_DELTA = delta;
	}

	const double get_time_delta()
	{
		return _SIMULATION_TIME_DELTA;
	}

	const double* get_time_delta_pointer()
	{
		return &_SIMULATION_TIME_DELTA;
	}

}