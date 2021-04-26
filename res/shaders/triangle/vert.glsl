#version 330 core

layout (location = 0) in vec3 vertices;

uniform mat4 projView;
uniform vec2[4] offset;

void main() {
	vec2 off = offset[gl_VertexID];
    gl_Position = projView * vec4(vec3(off.x, off.y, 1), 1);
}