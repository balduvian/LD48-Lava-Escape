//
// Created by Emmet on 11/27/2020.
//

#include <iostream>
#include "loader.h"

namespace CNLL {
    Loader::Loader() : gatherThread(), loadResources(), unloadResources(), completed() {}

    Loader::~Loader() {
    	if (gatherThread.joinable()) gatherThread.join();
    }

    /**
     * first function to call in the loading process
     */
    auto Loader::setup(u32 numResources) -> void {
    	loadResources.clear();
        loadResources.reserve(numResources);
        unloadResources.clear();

        total = numResources;
        completed = 0;
        error = false;
    }

	auto Loader::setup(u32 numLoadResources, u32 numUnloadResources) -> void {
		loadResources.clear();
		loadResources.reserve(numLoadResources);
		unloadResources.clear();
		unloadResources.reserve(numUnloadResources);

		total = numLoadResources + numUnloadResources;
		completed = 0;
		error = false;
	}

    /**
     * call for every resource the loading needs, after setup
     */
    auto Loader::giveLoadResource(Resource *resource) -> void {
        loadResources.push_back(resource);
    }

    /**
     * all calls must be after giveLoadResource
     */
	auto Loader::giveUnloadResource(Resource *resource) -> void {
    	/* only unload this resource if it should not be loaded */
    	if (std::find(loadResources.begin(), loadResources.end(), resource) == loadResources.end())
		    unloadResources.push_back(resource);
    	else
    		++completed;
	}

    /**
     * for loading, call this after resources have been given
     */
    auto Loader::start() -> void {
        /* preliminary check for resources that are already loaded */
        for (auto *resource : loadResources)
            if (resource->getLoaded()) ++completed;

        /* and for which unload resources are already unloaded */
        for (auto *resource : unloadResources)
        	if (!resource->getLoaded()) ++completed;

        /* on a separate thread, gather each resource sequentially */
        gatherThread = std::thread([this]() {
	        /* only gather resources that are not already loaded, and not already gathered */
            for (auto *resource : loadResources)
                if (!resource->getGathered() && !resource->getLoaded()) {
                	if (!resource->gather()) {
                		error = true;
                		break;
                	}
                }

            /* discard all the gathered resources ready for unloading */
	        if (!error) for (auto *resource : unloadResources)
		        if (resource->getGathered()) resource->discard();
        });
    }

    /**
     * for loading, call this in a gameloop until getDone() returns true
     */
    auto Loader::update() -> void {
    	if (error) {
    		if (gatherThread.joinable()) gatherThread.join();
    		std::cout << "LOADING ERROR" << std::endl;
    		exit(-2);

    	} else {
		    /* load all currently gathered resources */
		    for (auto *resource : loadResources) {
			    if (resource->getGathered() && !resource->getLoaded()) {
				    resource->load();
				    resource->discard();
				    ++completed;
			    }
		    }

		    /* unload all currently discarded resources */
		    for (auto *resource : unloadResources) {
			    if (!resource->getGathered()) {
				    resource->unload();
				    ++completed;
			    }
		    }

		    /* if we're done stop the gather thread */
		    if (getDone() && gatherThread.joinable()) gatherThread.join();
	    }
    }

    /**
     * load everything, blocking
     * will be done after returning
     * no need to update
     */
    auto Loader::quickLoad() -> void {
	    for (auto &resource : loadResources) {
	    	if (!resource->getGathered() && !resource->getLoaded()) resource->gather();
	    	if (!resource->getLoaded()) {
	    		resource->load();
	    		if (resource->getGathered()) resource->discard();
	    	}
	    }
    }

    auto Loader::getDone() -> bool {
        return completed == total;
    }

    auto Loader::getTotal() -> i32 {
    	return total;
    }

    auto Loader::getCompleted() -> i32 {
        return completed;
    }
}
