
#pragma once

#include <GL/glew.h>
#include <gl/GL.h>

#include <memory>
#include <vector>

#include "cnge/types.h"
#include "cnge/load/resource.h"

namespace CNGE {
	class Attribute {
	public:
		u32 perVertex;
		u32 size;
		f32* data;
		GLuint buffer;

		Attribute(u32, u32, f32*);

		/// the vertices in a vao are an attribute
		/// use this function to shortcut the creation
		static Attribute createVertexAttribute(u32, f32*);

		auto genBuffers() -> void;
		auto deleteBuffers() -> void;
	};

	class VAO : public CNLL::Resource {
	public:
		VAO(
			i32, Attribute&&,
			u32, u32[],
			u32, Attribute[]
		);

		~VAO();

		auto render() -> void;
		
	protected:
		auto customGather() -> bool override;
		auto customDiscard() -> void override;
		auto customLoad() -> void override;
		auto customUnload() -> void override;

	private:
		GLuint vao;

		Attribute vertexAttrib;

		u32 attribCount;
		Attribute* attribs;

		u32 indexCount;
		u32* indices;

		GLuint ibo;

		i32 drawMode;

		/// internally adds a vertex attribute to this vao
		/// creates and returns the opengl buffer
		auto add_attribute(Attribute& attrib, u32 location) -> void;
	};

}
