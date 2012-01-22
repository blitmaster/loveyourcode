/*
 *  SpringMesh.cpp
 *  springs
 *
 *  Created by Dude on 02/12/11.
 *  Copyright 2011 lummo. All rights reserved.
 *
 */

#include "SpringMesh.h"


//--------------------------------------------------------------
void SpringMesh::setup(int _rows, int _cols, int _dist, float _pdrag, float _sfmult, int _idparticle){
	
	rows = _rows;
	cols = _cols;
	
	int maxX = (cols-1)*_dist;
	int maxY = (rows-1)*_dist;
	
	
	//Setup middle point default node
	if(_idparticle <= rows*cols && _idparticle >= 0)idupdatedparticle = _idparticle;
	else {
		//default position
		idupdatedparticle = (rows*(cols*0.5)+rows*0.5);
		cout << "default idparticle" << endl;
	}
	
	//Defaul level particles
	levelparticles = 0;
	
	//Defuil Paint level particles
	levelps2draw = 0;
	
	
	//position all particles
	for (int i=0; i<rows*cols; i++) {
		int x = int(i) % cols * _dist;
		int y = int(i) / cols * _dist;
		//Particle p(ofxVec3f(x,y,0), y==0 ? 0 : 1, .96);

		
		//Borders set with no mass
		if (x == 0 || x == maxX || y == 0 || y == maxY) {
			Particle p(ofxVec3f(x,y,0), 0 , _pdrag);
			ps.push_back(p);
		}
		else //Rest of particles set with mass
		{
			Particle p(ofxVec3f(x,y,0), 1 , _pdrag);
			ps.push_back(p);
		}
		
		cout << "ParticleMesh x="		<< x	<< " y="	<< y	<< endl;


		if (i == idupdatedparticle) {
			//Default Id center position particle
			idparticleposition = ofVec3f(x, y, 0);
		}

	}
	
		cout << "ParticleMesh Max x="	<< maxX	<< "Max y=" << maxY	<< endl;
	
	//create all springs
	for (int i=0; i<rows*cols; i++) {
		int x = int(i) % cols;
		int y = int(i) / cols;
		
		//horizontal structural springs
		if (x<cols-1) {
			sp.push_back(Spring(&ps[i],&ps[i+1], _sfmult));
		}
		
		//vertical structural springs
		if (y<rows-1) {
			sp.push_back(Spring(&ps[i],&ps[i+cols], _sfmult));
		}
		
		//shear springs left to right
		if (x<cols-1 && y<rows-1) {
			sp.push_back(Spring(&ps[i],&ps[i+cols+1], _sfmult));
		}
		
		//shear springs right to left
		if (y>0 && x<cols-1 && y<rows) {
			sp.push_back(Spring(&ps[i],&ps[i-cols+1], _sfmult));
		}
		
		//bending springs horizontal
		if (x<cols-2) {
			sp.push_back(Spring(&ps[i],&ps[i+2], _sfmult));
		}
		
		//bending springs vertical
		if (y<rows-2) {
			sp.push_back(Spring(&ps[i],&ps[i+2*cols], _sfmult));
		}
		
		//bending springs diagonal
		if (y<rows-2 && x<cols-2) {
			sp.push_back(Spring(&ps[i],&ps[i+2+2*cols], _sfmult));
		}
	}
	
	
	
	//Particle Vector as 3d points
	pointsmesh.reserve(rows*cols);
	
}


//--------------------------------------------------------------
void SpringMesh::update(){

	//update springs
	for (int i=0; i<sp.size(); i++) {
		sp[i].update();
	}
	
	//update particles
	for (int i=0; i<ps.size(); i++) {
		ps[i].update();
	}
}

//--------------------------------------------------------------
bool SpringMesh::checkLargeMovements(int _id_node, float tdist)
{
	float auxdistance;
	bool bfar = false;
	
	if (levelparticles == 1) {
		
		auxdistance = ps[_id_node].pos.distance(ps[_id_node-1].pos);
		//cout << "Distance betwwen idnode and idnode -1 = " << auxdistance << endl;
		
		if (auxdistance > tdist) {
			bfar = true;
			//cout << "Its Far! = " << auxdistance << endl;
		}
		
	}
	else if(levelparticles == 2)
	{
		auxdistance = ps[_id_node].pos.distance(ps[_id_node-2].pos);
		//cout << "Distance betwwen idnode and idnode -2 = " << auxdistance << endl;
		
		if (auxdistance > tdist) {
			bfar = true;
			//cout << "Its Far! = " << auxdistance << endl;
		}
	}
	
	return bfar;

}

//--------------------------------------------------------------
void SpringMesh::updateForceMeshNode(int _id_node, ofVec3f _vectorforce){
	
	//apply forces
	if (_id_node > 0 && _id_node < ps.size()) {
		
		if (levelparticles == 0) {
		
			ps[_id_node].addForce(_vectorforce);

		}
		else if(levelparticles == 1)
		{
			//main particle
			ps[_id_node].addForce(_vectorforce);
			//1rs level neighbour particle
			ps[_id_node-1].addForce(_vectorforce);
			ps[_id_node+1].addForce(_vectorforce);
			
			//horizontal neighbour
			ps[_id_node+1*cols].addForce(_vectorforce);
			ps[_id_node-1*cols].addForce(_vectorforce);

			//diagonal Lefts
			ps[_id_node+1+(-1*cols)].addForce(_vectorforce);
			ps[_id_node-1+(-1*cols)].addForce(_vectorforce);
			//diagonal Rights
			ps[_id_node+1+(+1*cols)].addForce(_vectorforce);
			ps[_id_node-1+(+1*cols)].addForce(_vectorforce);
		}
	}
	
	//update springs
	for (int i=0; i<sp.size(); i++) {
		sp[i].update();
	}
	
	//update particles
	for (int i=0; i<ps.size(); i++) {
		ps[i].update();
	}
	
	//update points mesh ofvec3f
	for (int i=0; i<ps.size(); i++) {
		pointsmesh[i] = ps[i].pos;
	}

}


//Getters
//--------------------------------------------------------------
vector<Particle> SpringMesh::getParticles(){
	return ps;
}

//--------------------------------------------------------------
ofVec3f SpringMesh::getParticlePosition(int _idparticle){
	return ps[_idparticle].pos;
}

//--------------------------------------------------------------
int SpringMesh::getIdupdatedparticle()
{
	return idupdatedparticle;
}

//--------------------------------------------------------------
ofVec3f SpringMesh::getPosIdupdatedparticle()
{
	return idparticleposition;
}
//--------------------------------------------------------------
vector<ofVec3f>  SpringMesh::getUpdatedPoints()
{
	return pointsmesh;
}

//--------------------------------------------------------------
void SpringMesh::printValues()
{
	for (int i = 0; i < rows*cols; i++)
	{
		cout << "pointsmesh [" << i << "]" << pointsmesh[i] << endl;
	}
}

//Setters
//--------------------------------------------------------------
void SpringMesh::setParticlePosition(int _idparticle, ofVec3f _p){
	ps[_idparticle].pos = _p;
}

//--------------------------------------------------------------
void SpringMesh::setParticleDrag(int _idparticle, float _pdrag){
	ps[_idparticle].drag = _pdrag;
}

//--------------------------------------------------------------
void SpringMesh::updateIdMeshNode(int _idmeshnode)
{
	idupdatedparticle = _idmeshnode;
}

//--------------------------------------------------------------
void SpringMesh::updateReadLevelMesh(int readlevel)
{
	levelps2draw = readlevel;
}


//--------------------------------------------------------------
void SpringMesh::updateLevelMeshNode(int _level)
{
	int maxLevel = 1;
	
	if( (_level == 1 /*|| _level == 3 || _level == 7 || _level == 9*/) && _level < maxLevel+1 )
	{
		levelparticles = _level;
	}
	else {
		levelparticles = 0;
	}	
}
//--------------------------------------------------------------
void SpringMesh::draw(bool bdrawdebug, bool bdrawSprings){
	
	//if(bdrawdebug)ofDrawAxis(50);
	
	//springs
	ofSetColor(100, 100, 100);
	
	if(bdrawdebug)
	{
		glBegin(GL_LINES);
		for (int i=0; i<sp.size(); i++) {
			glVertex3f(sp[i].a->pos.x, sp[i].a->pos.y, sp[i].a->pos.z);
			glVertex3f(sp[i].b->pos.x, sp[i].b->pos.y, sp[i].b->pos.z);
		}	
		glEnd();
	}
		
	if(bdrawSprings)
	{
		
		//particles
		ofSetColor(255, 255, 255);
		
		glPointSize(2);
		
		glBegin(GL_POINTS);
		 for (int i=0; i<ps.size(); i++) {
			
			if(levelparticles == 0)
			{
				if (i == idupdatedparticle) {
					ofSetColor(255, 0, 0);
					
				}else {
					ofSetColor(0, 255, 255);
					
				}
			}
			if (levelparticles == 1) {
				
				glPointSize(4);
				
				if (i == idupdatedparticle) {
					ofSetColor(255, 0, 0);
				}else if ( (i == idupdatedparticle +1) ||  (i == idupdatedparticle -1)
						|| (i == idupdatedparticle+1*cols) || (i == idupdatedparticle-1*cols) 
						|| (i == idupdatedparticle+1+(1*cols)) || (i == idupdatedparticle+1+(-1*cols))
						|| (i == idupdatedparticle-1+(1*cols)) || (i == idupdatedparticle-1+(-1*cols)) 
						  ) {
					ofSetColor(255, 0, 0);
				}
				else
				{
					ofSetColor(0, 255, 255);
				}
				
				glPointSize(2);
			}
			
			glVertex3f(ps[i].pos.x, ps[i].pos.y, ps[i].pos.z);
		}	
		glEnd();
		
		
		ofSetColor(255, 255, 255);
		glPointSize(4);
		glBegin(GL_POINTS);
		
		for (int i=0; i<ps.size(); i++) {
			
			if (		(i == idupdatedparticle + levelps2draw) 
					||  (i == idupdatedparticle - levelps2draw)
					||	(i == idupdatedparticle + levelps2draw * cols) || (i == idupdatedparticle - levelps2draw * cols) 
					||	(i == idupdatedparticle + levelps2draw + (levelps2draw * cols)) || (i == idupdatedparticle + levelps2draw + (-levelps2draw * cols))
					||	(i == idupdatedparticle - levelps2draw + (levelps2draw * cols)) || (i == idupdatedparticle - levelps2draw + (-levelps2draw * cols)) 
				){
				
				ofSetColor(0, 255, 0);
				glVertex3f(ps[i].pos.x, ps[i].pos.y, ps[i].pos.z);
			}
			else
			{
				ofSetColor(0, 255, 255);
			}
		
			
			
		}	
		
		glEnd();
	}
	
}

