uniform sampler2DRect tex;

void main()
{
	vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
	gl_FragColor = vec4(col.b, col.g, col.r, col.a);
}