/************************************************************
************************************************************/
#include "Particle.h"


/************************************************************
Particle methods
************************************************************/

/******************************
******************************/
Particle::Particle(){
	friction = 0.01;
}

/******************************
******************************/
void Particle::setup(ofVec2f _position, ofVec2f _velocity){
	position = _position;
	velocity = _velocity;
}

/******************************
******************************/
void Particle::setup(float positionX, float positionY, float velocityX, float velocityY){
	position = ofVec2f(positionX, positionY);
	velocity = ofVec2f(velocityX, velocityY);
}

/******************************
******************************/
void Particle::resetForce(){
	force.set(0, 0);
}

/******************************
******************************/
void Particle::updateForce(){
	velocity = velocity * (1 - friction);
}

/******************************
******************************/
void Particle::updatePos(){
	velocity += force;
	position += velocity;
}

/******************************
******************************/
void Particle::update(){
	updateForce();
	updatePos();
	
	throughOfWalls();
}


/******************************
******************************/
void Particle::throughOfWalls(){
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	if (position.x < minx) {
		position.x = maxx;
	}
	if (position.y < miny) {
		position.y = maxy;
	}
	if (position.x > maxx) {
		position.x = minx;
	}
	if (position.y > maxy) {
		position.y = miny;
	}
}

/******************************
description
	引き付けあう力
******************************/
void Particle::addAttractionForce(float x, float y, float radius, float scale)
{
	ofVec2f posOfForce;
	posOfForce.set(x,y);
	
	ofVec2f diff = position - posOfForce;
	float length = diff.length();

	bool bAmCloseEnough = true;
	if (radius > 0){
		if (length > radius){
			bAmCloseEnough = false;
		}
	}

	if (bAmCloseEnough == true){
		// 距離から点にかかる力ベクトルを計算
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x = force.x - diff.x * scale * pct;
		force.y = force.y - diff.y * scale * pct;
	}
}

/************************************************************
PARTICLE_SET methods
************************************************************/
/******************************
******************************/
PARTICLE_SET::PARTICLE_SET()
{
	Verts = new ofVec3f[NUM_PARTICLES];
	Color = new ofFloatColor[NUM_PARTICLES];
}

/******************************
******************************/
PARTICLE_SET::~PARTICLE_SET()
{
	delete[] Verts;
	delete[] Color;
}

/******************************
******************************/
void PARTICLE_SET::setup()
{
	/********************
	********************/
	atraction = false;
	
	/********************
	********************/
	init_particleArray();
	
	/********************
	********************/
	for(int i = 0; i < NUM_PARTICLES; i++){
		Color[i].set(1.0, 1.0, 1.0, 1.0);
		Verts[i].set(ofVec3f(0.0, 0.0, 0.0));
	}
	Vbo.setVertexData(Verts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
	Vbo.setColorData(Color, NUM_PARTICLES, GL_DYNAMIC_DRAW);
	
	/********************
	********************/
	CommonColor = ofColor(0, 120, 255);
}

/******************************
******************************/
void PARTICLE_SET::update(int _mouseX, int _mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
	
	/********************
	update the position of all particle.
	********************/
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	for (int i = 0; i < particles.size(); i++){

		if (atraction) {
			particles[i].addAttractionForce(mouseX, mouseY, ofGetWidth(), 0.1);
		}

		particles[i].update();
	}
	
	/********************
	********************/
	for (int i = 0; i < particles.size(); i++) {
		Verts[i] = ofVec3f(particles[i].position.x, particles[i].position.y, 0);
		Color[i] = CommonColor;
	}
	
	Vbo.updateVertexData(Verts, NUM_PARTICLES);
	Vbo.updateColorData(Color, NUM_PARTICLES);
}

/******************************
******************************/
void PARTICLE_SET::draw()
{
	/********************
	********************/
	// ofBackground(0, 0, 0, 0);
	
	/********************
	********************/
	ofSetColor(255, 255, 255, 255);
	glPointSize(1.0);
	glEnable(GL_POINT_SMOOTH);
	Vbo.draw(GL_POINTS, 0, NUM_PARTICLES);
	
	/********************
	注)
	ofVboで描画すると、openGlの設定が何らか変わるようだ。
	この結果、次に来る
		ofDrawBitmapString()
		image.draw()
	等が描画されない現象が発生。
	
	この対応として、
		ofCircle(). ofRect().
	等を1発いれてやることで、OKとなった。
	おそらく、この関数内で、openGl設定が、また変わるのだろう。
		α = 0;
	にて描画する。
	********************/
	ofSetColor(255, 255, 255, 0);
	ofCircle(mouseX, mouseY, 4);
	
	ofSetColor(255, 255, 255, 255);

	/*
	// 重力の点を描く
	if (atraction) {
		ofSetColor(255, 0, 0);
	} else {
		ofSetColor(0, 255, 255);
	}
	ofCircle(mouseX, mouseY, 4);
	
	//文字の背景
	ofSetColor(0, 127);
	ofRect(0, 0, 200, 60);
	
	// ログを表示
	ofSetColor(255);
	ofDrawBitmapString("fps = " + ofToString(ofGetFrameRate()), 10, 20);
	ofDrawBitmapString("Particle num = " + ofToString(particles.size()), 10, 40);
	*/
}

/******************************
******************************/
void PARTICLE_SET::init_particleArray()
{
	particles.clear();
	for (int i = 0; i < NUM_PARTICLES; i++) {
		Particle p;
		
		p.friction = 0.0015;
		p.setup(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), ofVec2f(0, 0));
				
		particles.push_back(p);
	}
}

/******************************
******************************/
void PARTICLE_SET::set_attractive(bool b_val)
{
	atraction = b_val;
}

/******************************
******************************/
void PARTICLE_SET::set_color(ofColor _color)
{
	CommonColor = _color;
}

