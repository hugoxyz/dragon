
//#define GLSL_VERTEX_SHADER

varying vec4 vv4Color;

#ifdef GLSL_VERTEX_SHADER

attribute vec3 av3Vertex;
attribute vec4 av4Color;

uniform mat4 um4MMatrix;
uniform mat4 um4VMatrix;
uniform mat4 um4PMatrix;

void main() {
	gl_Position = um4PMatrix * um4VMatrix * um4MMatrix * vec4(av3Vertex, 1);
    vv4Color = av4Color;
}

#else

void main() {
    gl_FragColor = vv4Color;
}

#endif
