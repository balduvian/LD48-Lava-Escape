#version 330 core

uniform sampler2D tex;

uniform vec4 inColor;

in vec2 texPass;

out vec4 color;

void main() {
	vec4 texColor = inColor * texture(tex, texPass);
	float luminance = (texColor.x + texColor.y + texColor.z) / 3;

	color = vec4(texColor.xyz, texColor.z * luminance);
}
