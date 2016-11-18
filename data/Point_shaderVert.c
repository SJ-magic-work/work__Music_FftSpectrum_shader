#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

void main()
{
	/********************
	gl_Colorは頂点Color.
	
	ofTriangle()などで描画した場合は、ofSetColor()の値.
	ofVboで描画した場合は、setColorData
	
	Geometry shader使用時は、上手くFragmentまで伝えることができなかった。
	まずは、明示的にcolorをCPU側から渡して対応する。
	********************/
	gl_FrontColor =  gl_Color;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_Vertex;
}
