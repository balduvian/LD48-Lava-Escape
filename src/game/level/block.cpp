//
// Created by Emmet on 4/23/2021.
//

#include "block.h"

namespace Game {
	Block::Block() = default;
	Block::Block(u32 type): type(type), brokenStatus(0) {}

	auto Block::getType() -> u32 {
		return type;
	}

	auto Block::setType(u32 type) -> void {
		this->type = type;
	}

	auto Block::getBrokenStatus() -> u32 {
		return brokenStatus;
	}

	auto Block::setBrokenStatus(u32 status) -> void {
		brokenStatus = status;
	}
}
