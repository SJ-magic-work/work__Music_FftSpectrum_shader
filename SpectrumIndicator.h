/************************************************************
dexcription
	spectrumの解析結果Arrayを受け取って、Spectrum Analyzer表示する。
************************************************************/

#pragma once

#include "ofMain.h"
#include "ofxGui.h"


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class SPECTRUM_INDICATOR__BASE{
	private:
		
	protected:
		/****************************************
		****************************************/
		int NUM_VERTS; // per dot ... 1回のshader commandで渡す個数.
		
		ofVbo Vbo;
		ofVec3f* Verts;
		ofFloatColor* Color;
		
		int N;
		
		int NUM_DISP_SPECTRUMS;
		float IndicatorTextureSize;
		
		ofxPanel gui;
		ofxFloatSlider IndicatorVerticalSpace_ratio;
		ofxFloatSlider IndicatorTextureSize_ratio;
		ofxFloatSlider FreqFilter_k;
		
		/********************
		********************/
		void init_vbo();
		void vbo_drawLine(ofVec3f from, ofVec3f to);
		void vbo_drawPoint(ofVec3f p);
		
	public:
		SPECTRUM_INDICATOR__BASE(int _NUM_VERTS){
			NUM_VERTS = _NUM_VERTS;
			
			Verts = new ofVec3f[NUM_VERTS];
			Color = new ofFloatColor[NUM_VERTS];
		}
		
		~SPECTRUM_INDICATOR__BASE(){
			delete[] Verts;
			delete[] Color;
		}
		
		void drawGui();
		void load_setting();
};


/**************************************************
**************************************************/
class SPECTRUM_INDICATOR__TYPE_POINT : public SPECTRUM_INDICATOR__BASE{
	private:
		/****************************************
		****************************************/
		float IndicatorHoriSpace;
		float IndicatorVertSpace;
		float IndicatorGainSize;
		
		ofxIntSlider NumDisp_Spectrums;
		ofxFloatSlider IndicatorGainSize_ratio;
		ofxVec4Slider IndicatorColor;
		
		/****************************************
		****************************************/
		void setup_gui();
		void update_DynamicParam();
		
	public:
		/****************************************
		****************************************/
		SPECTRUM_INDICATOR__TYPE_POINT() : SPECTRUM_INDICATOR__BASE(1) {}
		
		void setup(int _N);
		void update();
		void draw(ofShader& shader, float *spectrum);
		void save_setting();
		void load_ColorTheme_setting(int ColorId);
};

/**************************************************
**************************************************/
class SPECTRUM_INDICATOR__TYPE_STRAIGHT : public SPECTRUM_INDICATOR__BASE{
	private:
		/****************************************
		****************************************/
		
		/********************
		********************/
		float x_Blank;
		float IndicatorHoriSpace;
		
		/********************
		********************/
		enum{
			UPPER_LINE,
			MIDDLE_LINE,
			LOWER_LINE,
			
			NUM_INDICATORS,
		};
		
		float x_ofs[NUM_INDICATORS];
		float y_ofs[NUM_INDICATORS];
		float Speed[NUM_INDICATORS];
		float MaxLightHeight[NUM_INDICATORS];
		
		/********************
		********************/
		ofxFloatSlider x_Blank_ratio;
		ofxFloatSlider y_ofs_ratio;
		
		ofxGuiGroup guiGp[NUM_INDICATORS];
		ofxFloatSlider MaxLightHeight_ratio[NUM_INDICATORS];
		ofxVec4Slider IndicatorColor[NUM_INDICATORS];
		ofxFloatSlider SpeedRatio[NUM_INDICATORS];
		

		/****************************************
		****************************************/
		void update_DynamicParam();
		void setup_gui();
		void set_x_ofs();
	
	public:
		/****************************************
		****************************************/
		SPECTRUM_INDICATOR__TYPE_STRAIGHT() : SPECTRUM_INDICATOR__BASE(2) {}
		
		void setup(int _N);
		void update();
		void draw(ofShader& shader, float *spectrum);
		void save_setting();
		void load_ColorTheme_setting(int ColorId);
		
		void test();
};

/**************************************************
**************************************************/
class SPECTRUM_INDICATOR__TYPE_CIRCLE : public SPECTRUM_INDICATOR__BASE{
	private:
		/****************************************
		****************************************/
		enum{
			SIZE_S,
			SIZE_M,
			SIZE_L,
			
			NUM_INDICATORS,
		};
		
		ofxGuiGroup guiGp[NUM_INDICATORS];
		
		ofxVec4Slider IndicatorColor[NUM_INDICATORS];
		ofxFloatSlider RotataionSpeed[NUM_INDICATORS];
		ofxFloatSlider MaxLightHeight_ratio[NUM_INDICATORS];
		ofxFloatSlider radius_ratio[NUM_INDICATORS];
		
		
		float IndicatorAngleStep;
		float radius[NUM_INDICATORS];
		float MaxLightHeight[NUM_INDICATORS];
		float BaseAngle[NUM_INDICATORS];
		
		/****************************************
		****************************************/
		void update_DynamicParam();
		void setup_gui();
	
	public:
		/****************************************
		****************************************/
		SPECTRUM_INDICATOR__TYPE_CIRCLE() : SPECTRUM_INDICATOR__BASE(2) {}
		
		void setup(int _N);
		void update();
		void draw(ofShader& shader, float *spectrum);
		void save_setting();
		void load_ColorTheme_setting(int ColorId);
};

/**************************************************
**************************************************/
class SPECTRUM_INDICATOR{
	private:
		/****************************************
		****************************************/
		enum INDICATOR_TYPE{
			TYPE_NONE,
			TYPE_STRAIGHT,
			TYPE_CIRCLE,
			
			NUM_INDICATOR_TYPES,
		};
		enum INDICATOR_TYPE IndicatorType;
		bool b_AddPointIndicator;
		
		ofShader shader_Line;
		ofShader shader_Point;
		ofImage IndicatorImage;
		
		bool b_dispGui;
		
		SPECTRUM_INDICATOR__TYPE_STRAIGHT Indicator_Straight;
		SPECTRUM_INDICATOR__TYPE_CIRCLE Indicator_Circle;
		SPECTRUM_INDICATOR__TYPE_POINT Indicator_Point;
		
		/****************************************
		****************************************/
		void clear_VboSetting_gl();
		


	public:
		/****************************************
		****************************************/
		void setup(int _N);
		void update();
		void draw(float *spectrum);
		
		void set_dispGui(bool val);
		void toggle_PointIndicator();
		void change_IndicatorType(int type);
		void save_setting();
		void load_setting(int type);
		
		void load_ColorTheme_setting(int ColorId);
		
		int get_NumIndicatorTypes() { return NUM_INDICATOR_TYPES; }
		
		void test();
};

