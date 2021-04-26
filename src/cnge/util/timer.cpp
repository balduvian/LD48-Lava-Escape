//
// Created by Emmet on 8/21/2020.
//

#include "timer.h"
#include "../math/math.h"

namespace CNGE {
	Timer::Timer(f64 time, bool startState) : timer(0), time(time), going(startState) {}

	auto Timer::start() -> void {
		going = true;
		timer = 0;
	}

	auto Timer::pause() -> void {
		going = false;
	}

	auto Timer::resume() -> void {
		going = true;
	}

	auto Timer::stop() -> void {
		going = false;
		timer = 0;
	}

	auto Timer::update(f64 currentTime) -> bool {
		if (!going) return false;

		timer += currentTime;

		if (timer >= time) {
			stop();
			return true;
		}

		return false;
	}

	auto Timer::updateContinual(f64 currentTime) -> bool {
		if (!going) return false;

		timer += currentTime;

		if (timer >= time) {
			timer = mod(timer, time);
			return true;
		}

		return false;
	}

	auto Timer::getAlong() -> f32 {
		return timer / time;
	}

	auto Timer::setTimer(f32 timer) -> void {
		this->timer = timer;
	}

	auto Timer::setTimerMax() -> void {
		this->timer = time;
	}

	auto Timer::addTimer(f32 timer) -> void {
		this->timer += timer;
	}

	auto Timer::getTimer() -> f32 {
		return timer;
	}

	auto Timer::setTime(f32 time) -> void {
		this->time = time;
	}

	auto Timer::getTime() -> f32 {
		return time;
	}

	auto Timer::getGoing() -> bool {
		return going;
	}

}
