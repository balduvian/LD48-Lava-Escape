//
// Created by Emmet on 11/27/2020.
//

#ifndef CNLL_RESOURCE_H
#define CNLL_RESOURCE_H

namespace CNLL {
    class Resource {
    public:
        explicit Resource(bool);

        auto gather() -> bool;
        auto discard() -> void;
        auto load() -> void;
        auto unload() -> void;

        auto getHasGather() -> bool;
        auto getGathered() -> bool;
        auto getLoaded() -> bool;

    protected:
        virtual auto customGather() -> bool = 0;
        virtual auto customDiscard() -> void = 0;
        virtual auto customLoad() -> void = 0;
        virtual auto customUnload() -> void = 0;

        auto onDestroy() -> void;

    private:
    	bool hasGather;
        bool gathered;
        bool loaded;
    };
}

#endif //CNLL_RESOURCE_H
