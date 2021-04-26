
#ifndef CNFW_LOOP
#define CNFW_LOOP

#include <functional>
#include <vector>
#include <chrono>

#include "../../types.h"
#include "timing.h"

namespace CNGE {
	class Loop {
	private:
		using   nanos = std::chrono::nanoseconds;
		using  millis = std::chrono::milliseconds;
		using seconds = std::chrono::seconds;
		using   clock = std::chrono::steady_clock;
		using   point = std::chrono::time_point<clock>;

		void* timer;
		
		bool sanic;
		u32 fps;

		point last, next;

		std::vector<nanos> frameHistory;
		std::vector<nanos>::iterator frameIter;
		
		Loop(bool, u32);

		static auto frameTime(u32)->u32;
		
		auto sleep(i64) const -> bool;
		auto doFrame(std::function<void(Timing*)>&, point) -> void;
		
	public:
		Loop(u32);
		Loop();
		
		~Loop();
		
		auto begin(std::function<bool()>&&, std::function<void(Timing*)>&&) -> void;
		auto setFPS(u32) -> void;
	};
}

#endif
