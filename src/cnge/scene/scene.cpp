
#include "scene.h"

namespace CNGE {
	Scene::Scene(std::vector<CNLL::Resource*>* bundle)
		: bundle(bundle) {}

	auto Scene::dontSwitch() -> SceneSwitchReturn {
		return { nullptr, nullptr };
	}

	auto Scene::getBundle() -> std::vector<CNLL::Resource*>* {
		return bundle;
	}
}
