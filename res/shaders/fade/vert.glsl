#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 projView;

uniform vec4 plane;
uniform vec4 texModif;

out vec2 texPass;

void main() {
    texPass = (texCoord * texModif.xy) + texModif.zw;

	gl_ClipDistance[0] = dot(plane, model * vec4(vertex, 1));

    gl_Position = (projView * model) * vec4(vertex, 1);
}
