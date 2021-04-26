
#ifndef CNGE_SCENE_MANAGER
#define CNGE_SCENE_MANAGER

#include <memory>

#include "../types.h"
#include "scene.h"
#include "../load/loadScreen.h"
#include "../engine/window.h"
#include "../load/loader.h"

namespace CNGE {
	class SceneManager {
	private:
        bool isLoading;

        CNLL::Loader loader;

		std::unique_ptr<Scene> scene;
		std::unique_ptr<LoadScreen> loadScreen;

	public:
		SceneManager();

		auto start(Input*, std::unique_ptr<Scene>&&, std::unique_ptr<LoadScreen>&&) -> void;

		/* sub update funcitons */
		auto updateLoading(Input*, Timing*) -> bool;
		auto updateScene(Input*, Timing*) -> bool;

		auto update(Input*, Timing*) -> void;
	};

}

#endif
