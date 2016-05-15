
//#define GLSL_VERTEX_SHADER

uniform mat4 um4MVMatrix;
uniform mat4 um4PMatrix;

varying vec4 vv4Color;

#ifdef GLSL_VERTEX_SHADER

attribute vec3 av3Vertex;
attribute vec4 av4Color;

void main() {
	gl_Position = um4PMatrix * um4MVMatrix * vec4(av3Vertex, 1);
    vv4Color = av4Color;
}

#else

varying vec4 vv4Color;

void main() {
    gl_FragColor = vv4Color;
}

#endif
