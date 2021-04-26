//
// Created by Emmet on 11/27/2020.
//

#include "resource.h"

namespace CNLL {
    Resource::Resource(bool hasGather): hasGather(hasGather), gathered(false), loaded(false) {}

    auto Resource::gather() -> bool {
        auto result = customGather();
        gathered = true;

        return result;
    }

    auto Resource::discard() -> void {
        customDiscard();
        gathered = false;
    }

    auto Resource::load() -> void {
        customLoad();
        loaded = true;
    }

    auto Resource::unload() -> void {
        customUnload();
        loaded = false;
    }

    auto Resource::getHasGather() -> bool {
        return hasGather;
    }

    auto Resource::getGathered() -> bool {
        return !hasGather || gathered;
    }

    auto Resource::getLoaded() -> bool {
        return loaded;
    }

    auto Resource::onDestroy() -> void {
    	if (gathered) discard();
    	if (loaded) unload();
    }
}

