#version 330

in vec4 vColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vColor.rgb, 1);
}