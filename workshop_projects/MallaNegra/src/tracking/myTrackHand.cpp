/*
 *  myTrackHand.cpp
 *  mallanegra
 *
 *  Created by Dude on 20/12/11.
 *  Copyright 2011 lummo. All rights reserved.
 *
 */

#include "myTrackHand.h"


//--------------------------------------------------------------
void myTrackHand::setup(int _x, int _y, int _z){

	position_hand = ofVec3f(_x, _y, _z);
	
	receiver.setup(PORTREV2);
	
	bdrawHand = true;
}


//--------------------------------------------------------------
void myTrackHand::update(){
	
	//position_hand = ofVec3f(_x, _y, _z);
	
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		// check for mouse moved message
		if ( m.getAddress() == "/BLOB_DEPTH" )
		{
			// both the arguments are int32's

			//mindepth =			m.getArgAsFloat( 3 );
			//maxdepth =			m.getArgAsFloat( 4 );
			
			position_hand.x =	m.getArgAsFloat( 0 );
			position_hand.y =	m.getArgAsFloat( 1 );
			position_hand.z =	m.getArgAsFloat( 2 );
			
			//position_hand.y = 
		}
	}
}


//--------------------------------------------------------------
void myTrackHand::draw(bool bdrawdebug){

	if (bdrawHand) {
	
		ofSetColor(180, 0, 0);
		
		ofNoFill();
		
		//ofPushView();
		
		//ofRotate(-90, 0, 0, 1);
		//ofTranslate(-(_colsmesh*_distsmesh)*0.5, -(_rowsmesh*_distsmesh)*0.5, 0);
		
		ofSphere(position_hand.x, position_hand.y, position_hand.z, 10);
		
		//ofSphere(0, 0, 0, 20);
		
		//ofPopView();
		
		//ofSetColor(255, 255, 255);
		
		ofFill();

	}
	
	//if (bdrawdebug) {
		//ofSetColor(255); // white
		//ofNoFill();
		//ofRect(ofGetWidth()*0.18, -ofGetHeight()*0.02, ofGetWidth()*0.133*0.5, ofGetHeight()*0.1*0.5); // Rel 4:3
	//}
	ofFill();
}

//--------------------------------------------------------------
ofVec3f myTrackHand::getHandposition()
{
	return position_hand;
}

//--------------------------------------------------------------
void myTrackHand::setHandposition(ofVec3f _position_hand)
{
	position_hand = _position_hand;
}

//--------------------------------------------------------------
void myTrackHand::keypressed(int key){
	
	if(key == OF_KEY_F2)
	{
		bdrawHand = !bdrawHand;
	}
	
	//move x++ hand. 50 is null
	if (key == 'a') {
		position_hand.x = position_hand.x + 10;
	}
	//move y++ hand. 50 is null
	else if ( key == 's'){
		position_hand.y = position_hand.y + 10;
	}
	//move z++ hand. 50 is null
	else if ( key == 'd'){
		position_hand.z = position_hand.z + 10;
	}
	//move x-- hand. 50 is null
	else if (key == 'z') {
		position_hand.x = position_hand.x - 10;
	}
	//move y-- hand. 50 is null
	else if ( key == 'x'){
		position_hand.y = position_hand.y - 10;
	}
	//move z-- hand. 50 is null
	else if ( key == 'c'){
		position_hand.z = position_hand.z - 10;
	}
	
}



