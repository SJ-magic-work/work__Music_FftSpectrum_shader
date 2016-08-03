#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxGui.h"


/************************************************************
class
************************************************************/

/**************************************************
description
	particle positionÇÃä«óù.
	ï`âÊã@î\ÇÕï€éùÇµÇ»Ç¢.
**************************************************/
class Particle {
private:
	void updateForce();
	void updatePos();
	void throughOfWalls();
	
public:
	/****************************************
	****************************************/
	ofVec2f position;
	ofVec2f velocity;
	ofVec2f force;
	float friction;
	
	/****************************************
	****************************************/
	Particle();

	void setup(ofVec2f position, ofVec2f velocity);
	void setup(float positionX, float positionY, float velocityX, float velocityY);

	void resetForce();

	void update();
	
	
	void addAttractionForce(float x, float y, float radius, float scale);
	float get_Speed()	{ return velocity.length(); }
};

/**************************************************
**************************************************/
class PARTICLE_SET {
private:
	/********************
	********************/
	enum{
		// NUM_PARTICLES = 100000,
		NUM_PARTICLES = 30000,
	};
	
	/********************
	********************/
	vector<Particle> particles;
	// ofVboMesh mesh;
	ofVbo Vbo;
	ofVec3f* Verts;
	ofFloatColor* Color;
	
	
	bool atraction;
	
	int mouseX;
	int mouseY;
	
	
	/********************
	********************/
	bool b_dispGui;
	
	ofxPanel gui;
	ofxVec4Slider CommonColor;
	ofxFloatSlider SpeedThresh;
	

public:
	PARTICLE_SET();
	~PARTICLE_SET();
	
	void setup();
	void update(int _mouseX, int _mouseY);
	void draw();

	void init_particleArray();
	void set_attractive(bool b_val);
	void set_color(ofColor _color);
	void set_dispGui(bool val);
};



