
attribute vec3 av3Vertex; //顶点位置
attribute vec4 av4Color;  //顶点颜色

uniform mat4 um4MMatrix; //模型矩阵
uniform mat4 um4VMatrix; //视图矩阵
uniform mat4 um4PMatrix; //投影矩阵

varying vec4 vv4Color;

void main() {
	vv4Color = av4Color;
	gl_Position = um4PMatrix * um4VMatrix * um4MMatrix * vec4(av3Vertex, 1);
}
