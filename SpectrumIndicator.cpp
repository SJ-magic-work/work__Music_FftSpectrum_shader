/************************************************************
************************************************************/
#include "SpectrumIndicator.h"




/************************************************************
SPECTRUM_INDICATOR__BASE
************************************************************/

/******************************
******************************/
void SPECTRUM_INDICATOR__BASE::init_vbo()
{
	for(int i = 0; i < NUM_VERTS; i++){
		Color[i].set(1.0, 1.0, 1.0, 1.0);
		Verts[i].set(ofVec3f(0.0, 0.0, 0.0));
	}
	Vbo.setVertexData(Verts, NUM_VERTS, GL_DYNAMIC_DRAW);
	Vbo.setColorData(Color, NUM_VERTS, GL_DYNAMIC_DRAW);
}

/******************************
******************************/
void SPECTRUM_INDICATOR__BASE::vbo_drawLine(ofVec3f from, ofVec3f to)
{
	if(NUM_VERTS == 2){
		Verts[0].set(from);
		Verts[1].set(to);
		
		Vbo.updateVertexData(Verts, NUM_VERTS);
		Vbo.draw(GL_LINES, 0, NUM_VERTS);
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR__BASE::vbo_drawPoint(ofVec3f p)
{
	if(NUM_VERTS == 1){
		Verts[0].set(p);
		
		Vbo.updateVertexData(Verts, NUM_VERTS);
		Vbo.draw(GL_POINTS, 0, NUM_VERTS);
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR__BASE::drawGui()
{
	gui.draw();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__BASE::load_setting()
{
	ofFileDialogResult res;
	res = ofSystemLoadDialog("Load setting");
	if(res.bSuccess) gui.loadFromFile(res.filePath);
}


/************************************************************
SPECTRUM_INDICATOR__TYPE_STRAIGHT
************************************************************/

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::setup(int _N)
{
	/********************
	********************/
	setup_gui();
	init_vbo();
	
	/********************
	********************/
	N = _N;
	
	// NUM_DISP_SPECTRUMS = int(N * 0.3);
	NUM_DISP_SPECTRUMS = int(N * 0.15);
	set_x_ofs();
	
	/********************
	********************/
	update_DynamicParam();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::setup_gui()
{
	gui.setup();
	
	/********************
	********************/
	gui.add(FreqFilter_k.setup("Freq Filter", 1.0, 0.1, 1.0));
	gui.add(x_Blank_ratio.setup("x blank ratio", 0.05, 0.0, 0.5));
	gui.add(y_ofs_ratio.setup("y_ofs ratio", 0.04, 0.01, 0.5));
	gui.add(IndicatorVerticalSpace_ratio.setup("v space ratio", 0.65, 0.3, 2.0));
	gui.add(IndicatorTextureSize_ratio.setup("texture size", 0.5, 0.2, 2.0));
	
	/********************
	********************/
	for(int GroupNum = UPPER_LINE; GroupNum < NUM_INDICATORS; GroupNum++){
		/********************
		different default value
		
		Speed:
			逆流しも混ぜていたが、信号の流れを普段から見ている人間からして、不自然。
			全て正方向にして、Speedを微妙に変えるよう、Design修正。
		********************/
		if(GroupNum == UPPER_LINE){
			guiGp[GroupNum].setup("Upper Line");
			
			guiGp[GroupNum].add(SpeedRatio[GroupNum].setup("Speed", 0.007, -0.2, 0.2));
			
		}else if(GroupNum == MIDDLE_LINE){
			guiGp[GroupNum].setup("Middle Line");
			
			guiGp[GroupNum].add(SpeedRatio[GroupNum].setup("Speed", 0.01, -0.2, 0.2));
			
		}else if(GroupNum == LOWER_LINE){
			guiGp[GroupNum].setup("Lower Line");
			
			guiGp[GroupNum].add(SpeedRatio[GroupNum].setup("Speed", 0.008, -0.2, 0.2));
			
		}
		
		/********************
		same default value
		********************/
		guiGp[GroupNum].add(MaxLightHeight_ratio[GroupNum].setup("line Height", 0.11, 0.01, 0.3));
		
		{
			ofVec4f initColor = ofVec4f(0.1, 0.5, 1.0, 1.0);
			ofVec4f minColor = ofVec4f(0.0, 0.0, 0.0, 0.0);
			ofVec4f maxColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
			
			guiGp[GroupNum].add(IndicatorColor[GroupNum].setup("color", initColor, minColor, maxColor));
		}
		
		
		/********************
		********************/
		gui.add(&guiGp[GroupNum]);
	}
	
	/********************
	********************/
	// gui.minimizeAll();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::set_x_ofs()
{
	x_ofs[UPPER_LINE]	= -2 * ofGetWidth() + ofRandom(0, ofGetWidth());
	
	x_ofs[MIDDLE_LINE]	= x_ofs[UPPER_LINE] + ofGetWidth() / 3;
	
	x_ofs[LOWER_LINE]	= x_ofs[MIDDLE_LINE] + ofGetWidth() / 3;
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::update_DynamicParam()
{
	/********************
	********************/
	y_ofs[UPPER_LINE]	= ofGetHeight() * y_ofs_ratio;
	y_ofs[MIDDLE_LINE]	= ofGetHeight()/2;
	y_ofs[LOWER_LINE]	= ofGetHeight() - ofGetHeight() * y_ofs_ratio;
	
	x_Blank = ofGetWidth() * x_Blank_ratio;
	
	for(int GroupNum = UPPER_LINE; GroupNum < NUM_INDICATORS; GroupNum++){
		Speed[GroupNum] = ofGetWidth() * SpeedRatio[GroupNum];
		MaxLightHeight[GroupNum] = ofGetHeight() * MaxLightHeight_ratio[GroupNum];
	}
	
	/********************
	********************/
	IndicatorHoriSpace = float(ofGetWidth()) / NUM_DISP_SPECTRUMS;
	
	IndicatorTextureSize = IndicatorHoriSpace * IndicatorTextureSize_ratio;
	if(IndicatorTextureSize < 1.5) IndicatorTextureSize = 1.5;
	
	/********************
	********************/
	static int Last_WindowWidth = ofGetWidth();
	if(ofGetWidth() != Last_WindowWidth){
		set_x_ofs();
	}
	Last_WindowWidth = ofGetWidth();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::update()
{
	/********************
	********************/
	update_DynamicParam();
	
	/********************
	********************/
	static float LastINT_sec = 0;
	float now = ofGetElapsedTimef();
	float dt = ofClamp(now - LastINT_sec, 0, 0.1);
	
	for(int GroupNum = UPPER_LINE; GroupNum < NUM_INDICATORS; GroupNum++){
		x_ofs[GroupNum] += Speed[GroupNum] * dt;
		
		if(0 < Speed[GroupNum]){
			if(0 <= x_ofs[GroupNum]){
				x_ofs[GroupNum] -= 2 * ofGetWidth();
			}
		}else{
			if(x_ofs[GroupNum] <= -2 * ofGetWidth()){
				x_ofs[GroupNum]  += 2 * ofGetWidth();
			}
		}
	}
	
	/*
	printf("x_ofs = %5.0f, %5.0f, %5.0f\r", x_ofs[UPPER_LINE], x_ofs[MIDDLE_LINE], x_ofs[LOWER_LINE]);
	fflush(stdout);
	*/
	
	/********************
	********************/
	LastINT_sec = now;
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::draw(ofShader& shader, float *spectrum)
{
	shader.setUniform1f( "DispTextureSize", IndicatorTextureSize ); // 描画サイズ
	shader.setUniform1f( "texture_VerticalSpaceRatio", IndicatorVerticalSpace_ratio );
	
	/********************
	********************/
	for(int LineId = UPPER_LINE; LineId < NUM_INDICATORS; LineId++){
		shader.setUniform4f("BaseColor", IndicatorColor[LineId]);
		
		ofPushMatrix();
		ofTranslate(0, y_ofs[LineId]);
		
		float spectrum_out = spectrum[NUM_DISP_SPECTRUMS - 1];
		int id = 0;
		int sgn = 1;
		for(int i = 0; i < 3; i++){ // NUM_DISP_SPECTRUMS 本のSpectrumをx方向に3回描画
			/********************
			Design 修正
			Low Powerのみ見えている区間があり、連動感が足りなかったため。
			折り返しPatternをやめたのは、"Edge"な箇所が普段から音声信号を見ている人間にとって自然に見えたため。
			********************/
			id = 0; sgn = 1;
			/*
			if( (i == 0) || (i == 2) )	{id = 0; sgn = 1;}
			if(i == 1)					{id = NUM_DISP_SPECTRUMS; sgn = -1;} // NUM_DISP_SPECTRUMS -> 1
			*/
			
			for(int j = 0 ; j < NUM_DISP_SPECTRUMS; j++){
				/********************
				上式で算出すると、IndicatorHoriSpaceの精度不足により誤差が積算する。
				その結果、x_ofs[]のjumpのtimingで微妙に「クッ」となるので、
				下式で算出すると良い。
				********************/
				// float x = x_ofs[LineId] + i * IndicatorHoriSpace * NUM_DISP_SPECTRUMS + j * IndicatorHoriSpace;
				float x = x_ofs[LineId] + i * ofGetWidth() + j * IndicatorHoriSpace;
				spectrum_out = FreqFilter_k * spectrum[id] + (1 - FreqFilter_k) * spectrum_out;
				
				if( (x_Blank <= x) && (x < (ofGetWidth() - x_Blank)) ){
					if(LineId == UPPER_LINE){
						vbo_drawLine( ofVec3f(x, 0, 0), ofVec3f(x, MaxLightHeight[LineId] * spectrum_out, 0) );
						
					}else if(LineId == LOWER_LINE){
						vbo_drawLine( ofVec3f(x, 0, 0), ofVec3f(x, - MaxLightHeight[LineId] * spectrum_out, 0) );
						
					}else if(LineId == MIDDLE_LINE){
						vbo_drawLine( ofVec3f(x, 0, 0), ofVec3f(x, MaxLightHeight[LineId] * spectrum_out, 0) );
						vbo_drawLine( ofVec3f(x, 0, 0), ofVec3f(x, - MaxLightHeight[LineId] * spectrum_out, 0) );
					}
				}
				
				id += sgn;
			}
		}
		
		ofPopMatrix();
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::test()
{
	MaxLightHeight[LOWER_LINE] = 100;
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_STRAIGHT::save_setting()
{
	gui.saveToFile("straight.xml");
}

/************************************************************
SPECTRUM_INDICATOR__TYPE_CIRCLE
************************************************************/

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_CIRCLE::setup(int _N)
{
	/********************
	********************/
	setup_gui();
	init_vbo();
	
	/********************
	********************/
	const int TargetNum = 50;
	N = _N;
	
	if(TargetNum < N){
		NUM_DISP_SPECTRUMS = TargetNum;
	}else{
		NUM_DISP_SPECTRUMS = N;
	}
	IndicatorAngleStep = float(360) / NUM_DISP_SPECTRUMS;
	
	for(int GroupNum = SIZE_S; GroupNum < NUM_INDICATORS; GroupNum++){
		BaseAngle[GroupNum] = 0;
	}
	
	/********************
	********************/
	update_DynamicParam();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_CIRCLE::setup_gui()
{
	gui.setup();
	
	/********************
	********************/
	gui.add(FreqFilter_k.setup("Freq Filter", 1.0, 0.1, 1.0));
	gui.add(IndicatorVerticalSpace_ratio.setup("v space ratio", 1.1, 0.3, 2.5));
	gui.add(IndicatorTextureSize_ratio.setup("texture size", 0.0145 ,0.005, 0.07));
	
	/********************
	********************/
	for(int GroupNum = SIZE_S; GroupNum < NUM_INDICATORS; GroupNum++){
		/********************
		different default value
		********************/
		if(GroupNum == SIZE_S){
			guiGp[GroupNum].setup("S");
			
			guiGp[GroupNum].add(RotataionSpeed[GroupNum].setup("Speed", 25, 5, 60));
			guiGp[GroupNum].add(radius_ratio[GroupNum].setup("radius", 0.06, 0.01, 0.6));
			guiGp[GroupNum].add(MaxLightHeight_ratio[GroupNum].setup("line Height", 0.2, 0.01, 0.4));
			
			
		}else if(GroupNum == SIZE_M){
			guiGp[GroupNum].setup("M");
			
			guiGp[GroupNum].add(RotataionSpeed[GroupNum].setup("Speed", -8, -60, -2));
			guiGp[GroupNum].add(radius_ratio[GroupNum].setup("radius", 0.138, 0.1, 0.6));
			guiGp[GroupNum].add(MaxLightHeight_ratio[GroupNum].setup("line Height", 0.2, 0.01, 0.4));
			
		}else if(GroupNum == SIZE_L){
			guiGp[GroupNum].setup("L");
			
			guiGp[GroupNum].add(RotataionSpeed[GroupNum].setup("Speed", 1.5, 1, 30));
			guiGp[GroupNum].add(radius_ratio[GroupNum].setup("radius", 0.263, 0.1, 0.6));
			guiGp[GroupNum].add(MaxLightHeight_ratio[GroupNum].setup("line Hight", 0.2, 0.01, 0.4));
		}
		
		/********************
		same default value
		********************/
		{
			ofVec4f initColor = ofVec4f(0.1, 0.5, 1.0, 1.0);
			ofVec4f minColor = ofVec4f(0.0, 0.0, 0.0, 0.0);
			ofVec4f maxColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
			
			guiGp[GroupNum].add(IndicatorColor[GroupNum].setup("color", initColor, minColor, maxColor));
		}
		
		
		/********************
		********************/
		gui.add(&guiGp[GroupNum]);
	}
	
	/********************
	********************/
	// gui.minimizeAll();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_CIRCLE::update_DynamicParam()
{
	IndicatorTextureSize = ofGetWidth() * IndicatorTextureSize_ratio;
	if(IndicatorTextureSize < 1.5) IndicatorTextureSize = 1.5;
	
	/********************
	********************/
	for(int GroupNum = SIZE_S; GroupNum < NUM_INDICATORS; GroupNum++){
		MaxLightHeight[GroupNum] = ofGetHeight() * MaxLightHeight_ratio[GroupNum];
		radius[GroupNum] = ofGetWidth() * radius_ratio[GroupNum];
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_CIRCLE::update()
{
	/********************
	********************/
	update_DynamicParam();
	
	/********************
	********************/
	static float LastINT_sec = 0;
	float now = ofGetElapsedTimef();
	float dt = ofClamp(now - LastINT_sec, 0, 0.1);
	
	for(int GroupNum = SIZE_S; GroupNum < NUM_INDICATORS; GroupNum++){
		BaseAngle[GroupNum] += RotataionSpeed[GroupNum] * dt;
	}
	
	/********************
	********************/
	LastINT_sec = now;
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_CIRCLE::draw(ofShader& shader, float *spectrum)
{
	shader.setUniform1f( "DispTextureSize", IndicatorTextureSize ); // 描画サイズ
	shader.setUniform1f( "texture_VerticalSpaceRatio", IndicatorVerticalSpace_ratio );
	
	/********************
	********************/
	for(int LineId = SIZE_S; LineId < NUM_INDICATORS; LineId++){
		shader.setUniform4f("BaseColor", IndicatorColor[LineId]);
		
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
		ofRotate(BaseAngle[LineId]);
		
		float spectrum_out = spectrum[NUM_DISP_SPECTRUMS/2 - 1];
		
		// same pattern繰り返しで、同軸方向に突き抜けるイメージ
		for(int i = 0; i < 2; i++){
			for(int i = 0; i < NUM_DISP_SPECTRUMS/2; i++){
				spectrum_out = FreqFilter_k * spectrum[i] + (1 - FreqFilter_k) * spectrum_out;
				
				ofRotate(IndicatorAngleStep); // Loopで積算されていくので、"i * IndicatorAngleStep" としないように!!!!!
				vbo_drawLine( ofVec3f(0, -radius[LineId], 0), ofVec3f(0, -radius[LineId] - MaxLightHeight[LineId] * spectrum_out, 0) );
			}
		}
		
		ofPopMatrix();
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_CIRCLE::save_setting()
{
	gui.saveToFile("circle.xml");
}

/************************************************************
SPECTRUM_INDICATOR__TYPE_POINT
************************************************************/

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_POINT::setup(int _N)
{
	/********************
	********************/
	setup_gui();
	init_vbo();
	
	/********************
	********************/
	N = _N;
	
	/********************
	********************/
	update_DynamicParam();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_POINT::setup_gui()
{
	gui.setup();
	
	/********************
	********************/
	gui.add(FreqFilter_k.setup("Freq Filter", 1.0, 0.1, 1.0));
	gui.add(NumDisp_Spectrums.setup("Num Disp", 5, 3, 15));
	gui.add(IndicatorTextureSize_ratio.setup("texture size", 0.36, 0.1, 1.0));
	gui.add(IndicatorGainSize_ratio.setup("Gain", 2.7, 0, 10));
	
	{
		ofVec4f initColor = ofVec4f(0.1, 0.5, 1.0, 0.65);
		ofVec4f minColor = ofVec4f(0.0, 0.0, 0.0, 0.0);
		ofVec4f maxColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
		
		gui.add(IndicatorColor.setup("color", initColor, minColor, maxColor));
	}
	
	/********************
	********************/
	// gui.minimizeAll();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_POINT::update_DynamicParam()
{
	NUM_DISP_SPECTRUMS = NumDisp_Spectrums;
	if(N <= NUM_DISP_SPECTRUMS) NUM_DISP_SPECTRUMS = N - 1;
	
	
	IndicatorHoriSpace = ((float)ofGetWidth() / 2) / (NUM_DISP_SPECTRUMS - 1);
	IndicatorVertSpace = IndicatorHoriSpace;
	
	IndicatorTextureSize = IndicatorHoriSpace * IndicatorTextureSize_ratio;
	IndicatorGainSize = IndicatorTextureSize * IndicatorGainSize_ratio;
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_POINT::update()
{
	update_DynamicParam();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_POINT::draw(ofShader& shader, float *spectrum)
{
	/********************
	********************/
	shader.setUniform4f("BaseColor", IndicatorColor);
	
	/********************
	********************/
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	
	float spectrum_out = spectrum[NUM_DISP_SPECTRUMS - 1];
	
	for(int id = 0; id < NUM_DISP_SPECTRUMS; id++){
		int x = (int)(id * IndicatorHoriSpace);
		
		spectrum_out = FreqFilter_k * spectrum[id] + (1 - FreqFilter_k) * spectrum_out;
		shader.setUniform1f( "DispTextureSize", IndicatorTextureSize + IndicatorGainSize * spectrum_out ); // 描画サイズ
		// shader.setUniform1f( "TextureSize", IndicatorTextureSize ); // 描画サイズ
		for(int y = 0; y <= ofGetHeight()/2; y+= IndicatorVertSpace){
			/********************
			********************/
			vbo_drawPoint(ofVec3f(x, y, 0));
			if(y != 0) vbo_drawPoint(ofVec3f(x, -y, 0));
			
			/********************
			********************/
			if(x != 0){
				vbo_drawPoint(ofVec3f(-x, y, 0));
				if(y != 0) vbo_drawPoint(ofVec3f(-x, -y, 0));
			}
		}
	}
	
	ofPopMatrix();
}

/******************************
******************************/
void SPECTRUM_INDICATOR__TYPE_POINT::save_setting()
{
	gui.saveToFile("point.xml");
}


/************************************************************
SPECTRUM_INDICATOR
************************************************************/

/******************************
******************************/
void SPECTRUM_INDICATOR::setup(int _N)
{
	/********************
	********************/
	b_dispGui = false;
	IndicatorType = TYPE_NONE;
	b_AddPointIndicator = false;
	
	/********************
	********************/
	Indicator_Straight.setup(_N);
	Indicator_Circle.setup(_N);
	Indicator_Point.setup(_N);
	
	IndicatorImage.loadImage( "light_Square.png" );
	
	
	/********************
	setGeometryInputType(), setGeometryOutputType()に指定可能なtypeは決まっている。
		http://openframeworks.cc/documentation/gl/ofShader/#show_setGeometryOutputType
		
	入力は、単に１入力プリミティブあたりに何個の頂点を送るか、だけと考えて良い。
		http://miffysora.wikidot.com/geometry-shader
		
	並べた順にshaderに渡される。
	
	outputは、「typeと順番」で描画内容が変わってくるので注意してoutputする。
	inputの時に、混乱なきよう、順序を考慮しておくのが良い。
		http://openframeworks.cc/documentation/gl/ofVbo/
	
	shader.setGeometryOutputCount()で設定できるCountは、それほど大きくない(1024とか、そんな所)。
	今回のケースでは、Lineを渡すと、始点から終点まで、textureを繰り返し描画(4点 x texture枚数)。
	n本のLineを描画するのに、Vboへ全ての点を設定して一気に描画したくなるが、すぐCountがmaxを超えてしまう。
	つまり、n本のLineは、都度 描画コマンドを発行すること。
	********************/
	//Enable detailed logging of openFrameworks messages in console
	ofSetLogLevel( OF_LOG_VERBOSE );

	//We must to specify geometry shader parameters before loading shader
	shader_Line.setGeometryInputType( GL_LINES );
	shader_Line.setGeometryOutputType( GL_TRIANGLE_STRIP );
	
	shader_Point.setGeometryInputType( GL_POINTS );
	shader_Point.setGeometryOutputType( GL_TRIANGLE_STRIP );
	
	
	// Maximal possible number of output vertices
	// " * 1/2 - 1" しないと動作しなかった.(実験結果:don't know why...)
	shader_Line.setGeometryOutputCount( shader_Line.getGeometryMaxOutputCount() / 2 - 1 );
	shader_Point.setGeometryOutputCount( shader_Point.getGeometryMaxOutputCount() / 2 - 1 );

	//Load shader
	shader_Line.load( "Line_shaderVert.c", "Line_shaderFrag.c", "Line_shaderGeom.c" );
	shader_Point.load( "Point_shaderVert.c", "Point_shaderFrag.c", "Point_shaderGeom.c" );
}

/******************************
******************************/
void SPECTRUM_INDICATOR::update()
{
	/********************
	********************/
	if(b_AddPointIndicator){
		Indicator_Point.update();
	}
	
	/********************
	********************/
	switch(IndicatorType){
		case TYPE_NONE:
			// nothing.
			break;
			
		case TYPE_STRAIGHT:
			Indicator_Straight.update();
			break;
			
		case TYPE_CIRCLE:
			Indicator_Circle.update();
			break;
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR::draw(float *spectrum)
{
	/********************
	<注意>
	glDisable(GL_DEPTH_TEST);
	しないと、2d 描画にてαが効かなくなる.
	********************/
	glDisable(GL_DEPTH_TEST);
	
	/********************
	********************/
	if(b_AddPointIndicator){
		shader_Point.begin();
	
		// ofSetColor(255, 255, 255, 255);
		shader_Point.setUniform1f( "OriginalTextureImageSize", IndicatorImage.getWidth() );
		shader_Point.setUniformTexture( "texture", IndicatorImage.getTextureReference(), 1 );
		
		Indicator_Point.draw(shader_Point, spectrum);
		
		shader_Point.end();
		
	}
	
	/********************
	********************/
	if(IndicatorType != TYPE_NONE){
		shader_Line.begin();
	
		// ofSetColor(255, 255, 255, 255);
		shader_Line.setUniform1f( "OriginalTextureImageSize", IndicatorImage.getWidth() );
		shader_Line.setUniformTexture( "texture", IndicatorImage.getTextureReference(), 1 );
		
		if(IndicatorType == TYPE_STRAIGHT){
			Indicator_Straight.draw(shader_Line, spectrum);
		}else if(IndicatorType == TYPE_CIRCLE){
			Indicator_Circle.draw(shader_Line, spectrum);
		}
		
		shader_Line.end();
	}
	
	/********************
	********************/
	if(b_dispGui){
		if(IndicatorType == TYPE_STRAIGHT){
			Indicator_Straight.drawGui();
		}else if(IndicatorType == TYPE_CIRCLE){
			Indicator_Circle.drawGui();
		}
		
		if(b_AddPointIndicator){
			Indicator_Point.drawGui();
		}
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR::save_setting()
{
	Indicator_Straight.save_setting();
	Indicator_Circle.save_setting();
	Indicator_Point.save_setting();
}

/******************************
******************************/
void SPECTRUM_INDICATOR::load_setting(int type)
{
	switch(type){
		case 0:
			printf("Straight\n");
			Indicator_Straight.load_setting();
			break;
			
		case 1:
			printf("Circle\n");
			Indicator_Circle.load_setting();
			break;
			
		case 2:
			printf("Point\n");
			Indicator_Point.load_setting();
			break;
			
		default:
			printf("input Error\n");
			break;
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR::toggle_PointIndicator()
{
	b_AddPointIndicator = !b_AddPointIndicator;
}

/******************************
******************************/
void SPECTRUM_INDICATOR::set_dispGui(bool val)
{
	b_dispGui = val;
}

/******************************
param
	type
		0	TYPE_STRAIGHT
		1	TYPE_CIRCLE
		2	TYPE_POINT
******************************/
void SPECTRUM_INDICATOR::change_IndicatorType(int type)
{
	if((type < 0) || (NUM_INDICATOR_TYPES <= type)){
		return;
	}else{
		IndicatorType = (enum INDICATOR_TYPE)type;
	}
}

/******************************
******************************/
void SPECTRUM_INDICATOR::test()
{
	Indicator_Straight.test();
}

