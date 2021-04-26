
#include <memory>
#include <iostream>
#include <thread>
#include <string>

#include <windows.h>
#undef near
#undef far

#include "loop.h"

namespace CNGE {
	Loop::Loop(const bool sanic, const u32 fps) : 
		timer(CreateWaitableTimer(nullptr, true, nullptr)),
		sanic(sanic), fps(fps), frameHistory(300),
		frameIter(frameHistory.begin()) {}

	auto Loop::frameTime(const u32 fps) -> u32 {
		return Timing::BILLION / fps;
	}
	
	auto Loop::sleep(const i64 ns) const -> bool {
		if (!timer) return false;

		auto li = LARGE_INTEGER{ .QuadPart = -ns };
		
		if (!SetWaitableTimer(timer, &li, 0, nullptr, nullptr, false)) {
			CloseHandle(timer);
			return false;
		}
		
		WaitForSingleObject(timer, INFINITE);
		
		return true;
	}

	auto Loop::doFrame(std::function<void(Timing*)>& frame, const point now) -> void {
		/* calculate how long this frame took */
		const auto delta = now - last;

		if (!sanic) {
			if (delta.count() < frameTime(fps) * 2_i64) {
				/* no lag happened, frames as usual */
				next = last + nanos(frameTime(fps) * 2);
				last = last + nanos(frameTime(fps));
			} else {
				/* lag detected! reset */
				next = now + nanos(frameTime(fps));
				last = now;
			}
		} else {
			last = now;
		}

		/* record how long this frame took */
		*frameIter = delta;

		/* calculate nominal fps */
		auto tempIter = frameIter;
		auto totalTime = nanos(0);
		auto frameTally = 0_u32;

		/* loop goes back in time, starting from the current frame */
		/* counting how many frames have passed in the last second */
		for (; frameTally < fps; ++frameTally) {
			totalTime += *tempIter;

			if (totalTime > seconds(1))
				break;

			/* circle back to end of frame history */
			if (tempIter == frameHistory.begin())
				tempIter = frameHistory.end() - 1;
			else
				--tempIter;
		}

		auto timing = Timing(frameTally, delta.count(), f64(delta.count()) / Timing::BILLION);
		frame(&timing);

		++frameIter;

		/* circle back to start of frame history */
		if (frameIter == frameHistory.end())
			frameIter = frameHistory.begin();
	}

	Loop::Loop(const u32 fps) : Loop(false, fps) {}
	Loop::Loop() : Loop(true, 0) {}
	
	Loop::~Loop() {
		CloseHandle(timer);
	}
	
	auto Loop::begin(std::function<bool()>&& exit, std::function<void(Timing*)>&& frame) -> void {
		last = clock::now();
		next = clock::now();

		while (!exit()) {
			/* sanic mode skips all waiting code */
			if (!sanic) {
				const auto waitTime = std::chrono::duration_cast<millis>((next - clock::now())).count() - 10;

				/* let the cpu rest for most of the time */
				/* calculate the time remaining until next frame*/
				if (!sleep(waitTime)) return;
				
				/* spin until it's exactly time for a new frame */
				while (clock::now() < next);
			}

			doFrame(frame, clock::now());
		}
	}

	auto Loop::setFPS(const u32 fps) -> void {
		this->fps = fps;
	}
}
