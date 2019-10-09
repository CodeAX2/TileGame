#version 440 core

#ifdef GL_ES
precision mediump float;
#endif

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform sampler2D texture;

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = gl_Color * pixel;
	gl_FragColor += vec4(gl_TexCoord[1].x * 10,0,0,0);
	
}