/************************************************************
************************************************************/
#include "Particle.h"


/************************************************************
Particle methods
************************************************************/

/******************************
******************************/
Particle::Particle(){
	set_friction(1);
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
void Particle::updateForce(float dt){
	velocity = velocity * (1 - friction_DownPer_ms * dt);
}

/******************************
******************************/
void Particle::updatePos(float dt){
	velocity += force * dt;
	position += velocity * dt;
}

/******************************
******************************/
void Particle::update(float dt){
	updateForce(dt);
	updatePos(dt);
	
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
	�����t��������
	
param
	scale
		���S����̋����ɂ����Force�����܂邪�A����ɑ΂���scaling.
		���Ԃ͖��֌W�ł���_�ɒ��ӁB
******************************/
void Particle::addAttractionForce(float x, float y, float radius, float scale)
{
	/********************
	********************/
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
		// ��������_�ɂ�����̓x�N�g�����v�Z
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x = - diff.x * scale * pct;
		force.y = - diff.y * scale * pct;
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
	b_dispGui = false;
	
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
	setup_gui();
}

/******************************
******************************/
void PARTICLE_SET::setup_gui()
{
	gui.setup();
	gui.add(PointSize.setup("PointSize", 2.0, 1.0, 4));
	{
		ofVec4f initColor = ofVec4f(0, 0.5, 1.0, 0.5);
		ofVec4f minColor = ofVec4f(0, 0, 0, 0);
		ofVec4f maxColor = ofVec4f(1, 1, 1, 1);
		
		gui.add(CommonColor.setup("color", initColor, minColor, maxColor));
	}
	gui.add(SpeedThresh.setup("Speed thresh", 0.2, 0.01, 1));
	
	gui.add(friction_DownPer_sec.setup("friction sec", 0.0991, 0.01, 1.0));
	gui.add(forceScale.setup("Force Scale", 0.00019, 0.0001, 0.001));
}

/******************************
******************************/
void PARTICLE_SET::Refresh_friction()
{
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].set_friction(friction_DownPer_sec/1000);
	}
}

/******************************
******************************/
void PARTICLE_SET::update(int _mouseX, int _mouseY)
{
	/********************
	********************/
	static float LastINT = ofGetElapsedTimef();
	float now = ofGetElapsedTimef();
	float dt = ofClamp(now - LastINT, 0, 0.1) * 1000;
	
	LastINT = now;
	
	/********************
	********************/
	mouseX = _mouseX;
	mouseY = _mouseY;
	
	/********************
	�eparticle��friction�͖{����肾���A
	gui�ɂ�parameter�������邽�߁A������refresh������B
	********************/
	Refresh_friction();
	
	/********************
	update the position of all particle.
	********************/
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	for (int i = 0; i < particles.size(); i++){

		if (atraction) {
			particles[i].addAttractionForce(mouseX, mouseY, ofGetWidth(), forceScale );
		}

		particles[i].update(dt);
	}
	
	/********************
	********************/
	const double Alpha_max = CommonColor->w;
	double tan = Alpha_max / SpeedThresh;
	
	/********************
	color��Speed down�Ƌ��Ƀ����������Ă����悤�ɂ����B
	********************/
	for (int i = 0; i < particles.size(); i++) {
		/* */
		Verts[i] = ofVec3f(particles[i].get_pos_x(), particles[i].get_pos_y(), 0);
		
		
		/* */
		float Alpha;
		if(SpeedThresh < particles[i].get_Speed()){
			Alpha = Alpha_max;
		}else{
			Alpha = tan * particles[i].get_Speed();
		}
		// Color[i].set(CommonColor->x, CommonColor->y, CommonColor->z, CommonColor->w);
		Color[i].set(CommonColor->x, CommonColor->y, CommonColor->z, Alpha);
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
	glPointSize(PointSize);
	glEnable(GL_POINT_SMOOTH);
	Vbo.draw(GL_POINTS, 0, NUM_PARTICLES);
	
	/********************
	********************/
	clear_VboSetting_gl();
	
	if(b_dispGui)	gui.draw();
	
	
	/*
	// �d�͂̓_��`��
	if (atraction) {
		ofSetColor(255, 0, 0);
	} else {
		ofSetColor(0, 255, 255);
	}
	ofCircle(mouseX, mouseY, 4);
	
	//�����̔w�i
	ofSetColor(0, 127);
	ofRect(0, 0, 200, 60);
	
	// ���O��\��
	ofSetColor(255);
	ofDrawBitmapString("fps = " + ofToString(ofGetFrameRate()), 10, 20);
	ofDrawBitmapString("Particle num = " + ofToString(particles.size()), 10, 40);
	*/
}

/******************************
descrition
	ofVbo�ŕ`�悷��ƁAopenGl�̐ݒ肪���炩�ς��悤���B
	���̌��ʁA���ɗ���`�悪���]�̓���ƂȂ�Ȃ��P�[�X�����B
		����function���`�悳��Ȃ��Ȃ�
			ofDrawBitmapString()
			image.draw()
			
	���̑Ή��Ƃ��āA
		ofCircle(). ofRect().
	����1������Ă�邱�ƂŁAOK�ƂȂ����B
	�����炭�A���̊֐����ŁAopenGl�ݒ肪�A�܂��ς��̂��낤�B
		�� = 0;
	�ɂĕ`�悷��B
******************************/
void PARTICLE_SET::clear_VboSetting_gl()
{
	ofSetColor(255, 255, 255, 0);
	ofCircle(0, 0, 1);
	
	ofSetColor(255, 255, 255, 255);
}

/******************************
******************************/
void PARTICLE_SET::init_particleArray()
{
	particles.clear();
	for (int i = 0; i < NUM_PARTICLES; i++) {
		Particle p;
		
		p.set_friction(0.1/1000);
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
	// CommonColor = _color;
}

/******************************
******************************/
void PARTICLE_SET::set_dispGui(bool val)
{
	b_dispGui = val;
}

