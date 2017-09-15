
uniform sampler2D us2Sampler;

varying vec4 vv4Color; //颜色值 fragment中使用
varying vec4 vv4ColorFactor; //根据反射计算出来的颜色因子 fragment中使用
varying vec2 vv2UV; //纹理坐标

void main() {
    vec4 tColor = texture2D(us2Sampler, vec2(vv2UV.s, vv2UV.t));
    gl_FragColor = vec4(vv4ColorFactor.rgb * vv4Color.rgb * vv4Color.a + vv4ColorFactor.rgb * tColor.rgb * tColor.a,
        max(vv4Color.a, tColor.a));
    //gl_FragColor = vec4(vv4ColorFactor.rgb, 1);
    gl_FragColor = vec4(1, 1, 1, 1);
}
