/*
 *  SpringMesh.h
 *  springs
 *
 *  Created by Dude on 02/12/11.
 *  Copyright 2011 lummo. All rights reserved.
 *
 */

#ifndef _SRINGMESH
#define _SRINGMESH

#include "ofMain.h"

#include "ofxVectorMath.h"
#include "Particle.h"
#include "Spring.h"

class SpringMesh{
	
public:
	
	void setup(int row, int col, int dist, float particledrag, float _stringforcemultiplier, int _idParticle);
	void update();
	void updateForceMeshNode(int _id_node, ofVec3f _vectorforce);
	void draw(bool bdrawdebug, bool bdrawSprings);
	

	void updateIdMeshNode(int idparticle );
	void updateLevelMeshNode(int level);
	void updateReadLevelMesh(int readlevel);
	bool checkLargeMovements(int _id_node, float tdist);
	
	void setParticleDrag(int idparticle, float _pdrag);
	
	//Getters
	vector<Particle> getParticles();
	ofVec3f getParticlePosition(int idparticle);
	int getIdupdatedparticle();
	ofVec3f getPosIdupdatedparticle();

	void printValues();
	
	vector<ofVec3f> getUpdatedPoints();

	//Setters
	void setParticlePosition(int _idparticle, ofVec3f _p);
	
private:
	
	//id particle modifiers
	int idupdatedparticle,levelparticles, levelps2draw;
	ofVec3f idparticleposition;
	
	vector<Particle> ps;
	vector<Spring> sp;

	//alternative vector
	vector<ofVec3f> pointsmesh;
	
	int rows;
	int cols;

};

#endif
