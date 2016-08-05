/************************************************************
************************************************************/

#pragma once


#include "ofMain.h"
#include "ofxGui.h"
#include "SpectrumIndicator.h"
#include "particle.h"


/************************************************************
************************************************************/
class ofApp : public ofBaseApp{

	private:
		/******************************
		******************************/
		
		/********************
		for parameter tune
		********************/
		enum{
			NUM_GUI_PARAMS = 5,
		};
		ofxPanel gui;
		ofxFloatSlider gui_param[NUM_GUI_PARAMS];
		
		/********************
		********************/
		bool b_DispGui;
		
		/********************
		********************/
		enum{
		/*
			WIDTH = 1024,
			HEIGHT = 768,
		*/
			WIDTH = 1280,
			HEIGHT = 720,
		};
		enum{
		/*
			WIDTH_SMALL = 630,
			HEIGHT_SMALL = 360,
		*/
			WIDTH_SMALL = 960,
			HEIGHT_SMALL = 540,
		};
		enum{
			// N = 128,
			N = 256,
		};
		float spectrum[ N ];
		ofSoundPlayer sound;
		
		ofImage img_Jacket;
		
		bool b_fullScreen;
		bool b_disp_Time_and_FrameRate;
		bool b_showCursor;
		
		SPECTRUM_INDICATOR SpectrumIndicator;
		
		/********************
		********************/
		PARTICLE_SET ParticleSet;
		bool b_Particle;
		
		/********************
		********************/
		enum{
			INPUT_SIZE = 100,
		};
		int SeekInput[INPUT_SIZE];
		int SeekInput_id;
		
		enum STATE{
			STATE_NONE,
			STATE_SEEK_INPUT,
			STATE_LOAD_GUI_SETTING,
		};
		enum STATE State;
		
		/******************************
		******************************/
		bool IsSeekTarget_inRange(int sec);
		void print_musicTime();
		void print_mousePosition();
		
		
		

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
