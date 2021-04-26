
#include <functional>

#include "cnge/scene/sceneManager.h"
#include "cnge/engine/audio/audioDevice.h"

#include "gameResources.h"
#include "game/scene/gameScene.h"
#include "gameLoadScreen.h"

auto main() -> int {
	/* init glfw and opengl */
	CNGE::Window::init();
	auto window = CNGE::Window(4, 6, true, true, "Lava Escape", CNGE::Window::getPrimaryMonitor(), false, true);
	CNGE::Window::wrangle();

	/* init openal */
	CNGE::AudioDevices::init();
	CNGE::AudioDevices::getDefaultOutputDevice()->setActive();

	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto loadScreenLoader = CNLL::Loader();
	loadScreenLoader.setup(Game::GameResources::loadScreenResources.size());
	for (auto &resource : Game::GameResources::loadScreenResources) loadScreenLoader.giveLoadResource(resource);
	loadScreenLoader.quickLoad();

	auto sceneManager = CNGE::SceneManager();
	sceneManager.start(window.getInput(), std::make_unique<Game::GameScene>(), std::make_unique<Game::GameLoadScreen>());

	auto loop = CNGE::Loop();
	loop.begin([&window]() { return window.getShouldClose(); }, [&](CNGE::Timing *timing) {
		window.poll();

		sceneManager.update(window.getInput(), timing);

		window.swap();
	});

	return 0;
}
