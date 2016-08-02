#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable


uniform sampler2DRect texture;

/********************
Geometry shader使用時のみであるが、
colorとtextureを両方使用する時は、上手くパスできないので、cpu側から明示的に渡す。
Geometry shaderを使用しない時は大丈夫だった.

Gray Scaleでtextureを作成しておいて、BaseColorでColor Controlするのが良い.
********************/
uniform vec4 BaseColor;

/******************************
******************************/
void main(){
	vec2 pos = gl_TexCoord[0].st;
	vec4 color = texture2DRect(texture, pos);
	
	// gl_FragColor = color;
	gl_FragColor = color * BaseColor;
	// gl_FragColor = BaseColor;
	
	// gl_FragColor = gl_Color;
}
