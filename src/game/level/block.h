//
// Created by Emmet on 4/23/2021.
//

#ifndef CNGE_8_2_BLOCK_H
#define CNGE_8_2_BLOCK_H

#include "cnge/types.h"

namespace Game {
	class Block {
	private:
		u32 type;
		u32 brokenStatus;

	public:
		constexpr static auto AIR = 0;
		constexpr static auto BEDROCK = 1;
		constexpr static auto DIRT = 2;
		constexpr static auto STONE = 3;
		constexpr static auto CRYSTAL = 4;

		Block();
		Block(u32);

		auto getType() -> u32;
		auto setType(u32) -> void;

		auto getBrokenStatus() -> u32;
		auto setBrokenStatus(u32) -> void;
	};
}

#endif //CNGE_8_2_BLOCK_H
