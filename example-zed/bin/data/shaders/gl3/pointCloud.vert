#version 330

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;

out vec4 vColor;

void main()
{
	gl_Position = modelViewProjectionMatrix * position;

	// Decompose float into 4 chars for color.
	uint rgbaInt = floatBitsToUint(color.a);
	uint bInt = (rgbaInt / 256U / 256U) % 256U;
	uint gInt = (rgbaInt / 256U) % 256U;
	uint rInt = (rgbaInt) % 256U;
	vColor = vec4(rInt / 255.0f, gInt / 255.0f, bInt / 255.0f, 1.0);
}