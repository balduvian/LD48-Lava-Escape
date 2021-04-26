#version 330 core

uniform sampler2D tex;

uniform vec4 inColor;

in vec2 texPass;

out vec4 color;

void main() {
	// grab distance field color from texture
	vec4 passColor = texture(tex, texPass);

	// two of the three channels must be greater than the threshold
	// try all possible combinations and use * as an "and" operator
	// use += as an "or" operator
	float alpha = step(0.5, passColor.x) * step(0.5, passColor.y);
	alpha += step(0.5, passColor.y) * step(0.5, passColor.z);
	alpha += step(0.5, passColor.z) * step(0.5, passColor.x);

	alpha = clamp(alpha, 0, 1);

	vec4 inverseColor = vec4((1 - inColor).xyz, 0.75);
	vec4 resultColor = mix(inverseColor, inColor, alpha);

	// the output uses alpha to determine if it's shown
	color = resultColor;//vec4(inColor.x, inColor.y, inColor.z, alpha * inColor.w);
}
