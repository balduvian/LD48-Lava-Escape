#version 330 core

uniform sampler2D tex;

uniform vec4 inColor;

in vec2 texPass;

out vec4 color;

void main() {
	color = inColor * texture(tex, texPass);

	if(color.w < 0.5)
		discard;
}
