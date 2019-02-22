#version 150

uniform sampler2DRect tex;

in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
	vec4 col = texture(tex, vTexCoord);
	float value = col.r;
	float low1 = 0.5;
	float high1 = 5.0;
	float low2 = 1.0;
	float high2 = 0.0;
	float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
	if (d == 1.0) {
		//d = 0.0;
	}
	fragColor = vec4(vec3(d), 1.0);
}