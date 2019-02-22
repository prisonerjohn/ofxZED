#version 150

uniform sampler2DRect tex;

in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
	vec4 col = texture(tex, vTexCoord);
	fragColor = col.bgra;
}