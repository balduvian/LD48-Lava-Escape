#version 330 core

uniform sampler2D tex;

uniform vec4 inColor;

in vec2 texPass;

out vec4 color;

void main() {
	color = texture(tex, texPass);

	if(color.x > 0.98)
		color = inColor; 
}
