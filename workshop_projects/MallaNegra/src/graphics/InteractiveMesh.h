/*
 *  blackMesh.h
 *  InteractiveMesh
 *
 *  Created by Dude on 20/12/11.
 *  Copyright 2011 lummo. All rights reserved.
 *
 */

#ifndef _BLACKMESH
#define _BLACKMESH

#include "ofMain.h"

#include "SpringMesh.h"

#include "ofxOsc.h"

#define HOST1 "localhost"
//#define HOST1 "193.145.40.158"
#define PORT1 6699
#define PORT2 6677

class InteractiveMesh{
	
public:
	
	void setup(int _rowsmesh, int _colsmesh, int _distsmesh, float _particle_drag, float _string_k, int _idParticle);
	void update(ofVec3f myhandpos);
	void draw();
	void keypressed(int key);
	
	
	//Getters
	
	ofVec3f getPosIdupdatedparticle();
	
	//Setters
	//
	

	//Calc Special features for Update or not the mesh
	float calcAccMesh();
	ofxVec3f posMesh, lastposMesh;
	float acc;
	
	void calcInfoLevelRead(int levelparticles);

	//////////////////////////////////////////////
	//Control vars
	bool bkeymesh;
	bool bkeyparticles;
	bool bkeydrag;
	
	bool bdrawSprings, bdrawdebug;
	bool bmesh_updatingPoints;
	
	//For check changs of hand position
	ofVec3f	last_handpos;

	
private:
	
	//////////////////////////////////////////////
	//Vars mesh
	int idmeshnode;
	int rowsmesh;
	int colsmesh;
	int distsmesh;
	float string_k, particle_drag;
	
	//Adapted Mesh 
	SpringMesh smesh;
	
	//////////////////////////////////////////////
	//Reading Levels of the mesh
	int levelofReading;
	//Vector for special Control of node mesh	
	vector<float> distupdated2inimeshpoints;
	vector<ofVec3f> updatedmeshpoints;
	vector<ofVec3f> inimeshpoints;
	vector<float> infolevelready2send;
	
	//////////////////////////////////////////////
	//Control var
	bool bfirstUpdate;
	int levelps2draw;

	//////////////////////////////////////////////
	//Osc 
	ofxOscSender sender1;
	//bool bSenderStarted;
	ofxOscReceiver receiver;

};

#endif