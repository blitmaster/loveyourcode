/*
 *  myTrackHand.h
 *  mallanegra
 *
 *  Created by Dude on 20/12/11.
 *  Copyright 2011 lummo. All rights reserved.
 *
 */

#ifndef _TRACKHAND
#define _TRACKHAND

#include "ofMain.h"

#include "ofxVectorMath.h"
#include "Particle.h"
#include "Spring.h"

#include "ofxOscReceiver.h"

#define PORTREV2 6688

class myTrackHand{
	
public:
	
	void setup(int x, int y, int z);
	void update();
	void draw(bool bdrawdebug);
	ofVec3f getHandposition();
	void setHandposition(ofVec3f _position_hand);

	void keypressed(int key);
	
	
private:
	
	ofVec3f position_hand;
	ofVec3f vector_hand;
	
	int mindepth,maxdepth;
	
	bool bdrawHand;
	
	ofxOscReceiver receiver;
	
};

#endif