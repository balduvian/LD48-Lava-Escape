//
// Created by Emmet on 8/21/2020.
//

#ifndef CNGE_8_2_TIMER_H
#define CNGE_8_2_TIMER_H

#include "../types.h"

namespace CNGE {
	class Timer {
	private:
		f64 timer, time;
		bool going;

	public:
		Timer(f64, bool = false);

		auto start() -> void;
		auto pause() -> void;
		auto resume() -> void;
		auto stop() -> void;

		auto update(f64) -> bool;
		auto updateContinual(f64) -> bool;

		auto getAlong() -> f32;

		auto setTimer(f32) -> void;
		auto addTimer(f32) -> void;
		auto getTimer() -> f32;
		auto setTimerMax() -> void;

		auto setTime(f32) -> void;
		auto getTime() -> f32;

		auto getGoing() -> bool;
	};
}

#endif //CNGE_8_2_TIMER_H
