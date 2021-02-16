#pragma once

#include <chrono>

namespace	NTime{

	const auto begin_prog		= std::chrono::steady_clock::now();
	const auto begin_prog_ms	= std::chrono::time_point_cast<std::chrono::milliseconds>(begin_prog);

	static unsigned long long	getTicks(){

			auto ticks		= std::chrono::steady_clock::now();
			auto ticks_ms	= std::chrono::time_point_cast<std::chrono::milliseconds>(ticks);

			return ticks_ms.time_since_epoch().count()-begin_prog_ms.time_since_epoch().count();
	}
};
