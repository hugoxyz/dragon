
attribute vec3 av3Vertex;
attribute vec4 av4Color;

uniform mat4 um4MVMatrix;
uniform mat4 um4PMatrix;

varying vec4 vv4Color;

void main() {
	gl_Position = um4PMatrix * um4MVMatrix * vec4(av3Vertex, 1);
	vv4Color = av4Color;
}
