#pragma once

#include <chrono>
#include <thread>


namespace SpikingNetworks::core
{
	constexpr auto MILLI_TO_MICRO_SECONDS = 1000;
	inline double _SIMULATION_TIME_DELTA(0.001);  // 1ms
	inline std::chrono::time_point<std::chrono::milliseconds> _SIMULATION_START_POINT(std::chrono::milliseconds(0));

	void set_simulation_time();
	void set_simulation_time(unsigned long time);

	unsigned long get_simulation_time();

	template <typename Number, typename Duration>
	void wait_until(Number until)
	{
		auto timeout = std::chrono::time_point<std::chrono::high_resolution_clock, Duration>(Duration(until));
		std::this_thread::sleep_until<std::chrono::high_resolution_clock, Duration>(timeout);
	}

	void set_time_delta(double delta);
	const double get_time_delta();
	const double* get_time_delta_pointer();
}