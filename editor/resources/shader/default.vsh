

attribute vec3 av3Vertex; //顶点位置
attribute vec4 av4Color;  //顶点颜色
attribute vec2 av2UV; //纹理坐标
attribute vec3 av3Normal; //法线

uniform mat4 um4MMatrix; //模型矩阵
uniform mat4 um4VMatrix; //视图矩阵
uniform mat4 um4PMatrix; //投影矩阵
uniform mat3 um3NMatrix; //法线矩阵,用于把法线转到视图坐标内,如果物体是刚体,它应该与 um4MMatrix 等同,只是它少了一维

uniform vec3 uv3LightPosition; //光源位置
uniform vec3 uv3EyePosition; //眼睛或摄像头位置

uniform vec4 uv4Ambient; //环境光
uniform vec4 uv4Diffuse; //漫反射
uniform vec4 uv4Specular; //镜面高光
uniform float ufShininess; //光泽度,越小越光滑

varying vec4 vv4Color; //颜色值 fragment中使用
varying vec4 vv4ColorFactor; //根据反射计算出来的颜色因子 fragment中使用
varying vec2 vv2UV; //纹理坐标

void main() {
	vec4 v4ModulePos = um4MMatrix * vec4(av3Vertex, 1);

	vec3 P = v4ModulePos.xyz; //当前顶点
	vec3 N = normalize(um3NMatrix * av3Normal); //转换到视图空间的光线
	vec3 L = normalize(uv3LightPosition - P); //光源到当前点的向量
	vec3 E = normalize(uv3EyePosition - P); //眼睛(摄像头)到当前点的向量
	vec3 H = normalize((L + E)/2.0); //光源与眼睛(摄像头)形成夹角的中间向量

	float df = max(0.0, dot(N, L)); //漫反射因子, 法线与光源的点积
	float sf = max(0.0, dot(N, H)); //镜面因子, 法线与上面H的点积
	sf = pow(sf, ufShininess); //镜面因子, 再用光泽度来幂乘
	vv4ColorFactor = uv4Ambient + df * uv4Diffuse + sf * uv4Specular; //环境光, 漫反射, 镜面 三者相加

	vv4Color = av4Color;
	vv2UV = av2UV;
	gl_Position = um4PMatrix * um4VMatrix * v4ModulePos;
}
