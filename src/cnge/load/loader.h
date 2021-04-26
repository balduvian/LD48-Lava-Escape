//
// Created by Emmet on 11/27/2020.
//

#ifndef CNLL_LOADER_H
#define CNLL_LOADER_H

#include "resource.h"
#include "cnge/types.h"
#include <vector>
#include <thread>

namespace CNLL {
    class Loader {
        public:
            Loader();
			~Loader();

            auto setup(u32) -> void;
	        auto setup(u32, u32) -> void;

            auto giveLoadResource(Resource*) -> void;
			auto giveUnloadResource(Resource*) -> void;

            auto start() -> void;
            auto update() -> void;
			auto quickLoad() -> void;

            auto getDone() -> bool;
            auto getTotal() -> i32;
            auto getCompleted() -> i32;

        private:
            std::thread gatherThread;

            std::vector<Resource*> loadResources;
            std::vector<Resource*> unloadResources;

            i32 total;
            i32 completed;

            bool error;
    };
}

#endif //CNLL_LOADER_H
