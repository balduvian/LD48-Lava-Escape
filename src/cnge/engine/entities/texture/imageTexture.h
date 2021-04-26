//
// Created by Emmet on 8/26/2020.
//

#ifndef CNGE_8_2_IMAGETEXTURE_H
#define CNGE_8_2_IMAGETEXTURE_H

#include <type_traits>
#include <concepts>
#include <tuple>
#include "texture.h"
#include "cnge/load/resource.h"

namespace CNGE {
	template<typename T>
	concept TextureType = requires(T t) {
		std::is_base_of<Texture, T>();
	};

	template<TextureType T, typename ...Parameters>
	class ImageTexture : public CNLL::Resource {
	public:
		ImageTexture(const char* filepath, TextureParams textureParams, Parameters ...inputParameters)
			: Resource(true), imageFilepath(filepath), image(), textureParams(textureParams), texture(nullptr), parameters(std::make_tuple(inputParameters...)) {}

		~ImageTexture() {
			onDestroy();
		}

		auto get() -> T* {
			return texture.get();
		}

		auto customGather() -> bool override {
			image = Image::fromPNG(imageFilepath.c_str());

			return image->isValid();
		}

		auto customDiscard() -> void override {
			image = nullptr;
		}

		auto customLoad() -> void override {
			std::apply([this](Parameters ...parameters) {
				texture = std::make_unique<T>(image->getWidth(), image->getHeight(), image->getPixels(), textureParams, parameters...);
			}, parameters);
		}

		auto  customUnload() -> void override {
			texture = nullptr;
		}

	private:
		std::string imageFilepath;
		std::unique_ptr<Image> image;

		TextureParams textureParams;
		std::unique_ptr<T> texture;

		std::tuple<Parameters...> parameters;
	};
}

#endif //CNGE_8_2_IMAGETEXTURE_H
