#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "InteractiveMesh.h"
#include "myTrackHand.h"


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	
	myTrackHand myhand;
	
	InteractiveMesh blackmesh;
	
	//Setup Vars
	int rowsmesh;
	int colsmesh;
	int distsmesh;
	float particle_drag;
	float string_k;
	
	bool bdrawdebug;
	
	//Camera
	ofEasyCam simpleCam;
	int dist2object;
	bool bkeycam;
	
};

#endif
