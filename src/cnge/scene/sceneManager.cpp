
#include <iostream>

#include "../load/resource.h"

#include "sceneManager.h"

namespace CNGE {
	SceneManager::SceneManager()
		: isLoading(), loader(), scene(nullptr), loadScreen() {}
	
	auto SceneManager::start(Input* input, std::unique_ptr<Scene>&& sceneTo, std::unique_ptr<LoadScreen>&& loadScreenTo) -> void {
		isLoading = true;

		/* start loading and unloading */
		loader.setup(sceneTo->getBundle()->size(), (scene == nullptr) ? 0 : scene->getBundle()->size());
		for (auto &resource : *(sceneTo->getBundle())) loader.giveLoadResource(resource);
		if (scene != nullptr) for (auto &resource : *(scene->getBundle())) loader.giveUnloadResource(resource);
		loader.start();

		/* set the new scene as the scene */
		/* the old scene is deleted */
		scene = std::move(sceneTo);
		loadScreen = std::move(loadScreenTo);

		/* setup the load screen for the current window */
		loadScreen->resized(input->getWidth(), input->getHeight());
	}

	auto SceneManager::updateLoading(Input* input, Timing* timing) -> bool {
		/* resize the loading screen */
		if (input->getResized()) loadScreen->resized(input->getWidth(), input->getHeight());

		/* load */
		loader.update();

		/* when the loading screen is done */
		if (loader.getDone()) {
			/* setup the scene for the current window */
			scene->resized(input->getWidth(), input->getHeight());
			scene->start();
			
			return false;

		} else {
			/* update the loading screen */
			loadScreen->update(input, timing);
			loadScreen->render(loader.getCompleted(), loader.getTotal());
		}

		return true;
	}

	auto SceneManager::updateScene(Input* input, Timing* timing) -> bool {
		/* resize the scene with the window */
		if (input->getResized())
			scene->resized(input->getWidth(), input->getHeight());

		scene->update(input, timing);
		scene->render();

		auto [returnedScene, returnedLoadScreen] = scene->switchScene();

		// check if we need to switch scenes
		if (returnedScene != nullptr)
			return start(input, std::move(returnedScene), std::move(returnedLoadScreen)), true;

		return false;
	}

	/// manages the updating of both loading screens and scenes
	/// updates and renders
	auto SceneManager::update(Input* input, Timing* timing) -> void {
		/* loading assets and rendering loading screen */
		if (isLoading)
			isLoading = updateLoading(input, timing);
		else
			isLoading = updateScene(input, timing);
	}

}
