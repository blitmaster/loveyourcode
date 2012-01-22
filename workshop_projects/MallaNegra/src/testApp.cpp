#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground(0,0,0);
	ofSetFrameRate(30);
	ofEnableSmoothing();

	//Setup Vars
	rowsmesh =  19;
	colsmesh = 	19;
	distsmesh = 10;
	particle_drag = 0.7;
	string_k = 0.001;
	
	//Mesh
	blackmesh.setup(rowsmesh, colsmesh, distsmesh, particle_drag, string_k, 180 /* or set Default particle -> -1 */);
	
	//Hand
	//ofVec3f auxhandpos = ofVec3f(colsmesh*distsmesh*0.5, rowsmesh*distsmesh*0.5, 0); // manual
	//myhand.setup((int)auxhandpos.x, (int)auxhandpos.y, (int)auxhandpos.z);
	myhand.setup(blackmesh.getPosIdupdatedparticle().x, blackmesh.getPosIdupdatedparticle().y, blackmesh.getPosIdupdatedparticle().z); // get form mesh
	
	//Draw options
	bdrawdebug = false;
	
	//camera setup
	bkeycam = false;
	dist2object = 200;
	simpleCam.setDistance(dist2object);
	
	ofSetWindowPosition(0,0);
	ofSetWindowTitle("RTI MESH /// Josep Francesc i Carles Gutierrez");

	
}

//--------------------------------------------------------------
void testApp::update(){

	//update by osc messages or keypress
	myhand.update();
	
	//update mesh
	blackmesh.update(myhand.getHandposition());
		
}

//--------------------------------------------------------------
void testApp::draw() {
	
	simpleCam.begin(); // ofRectangle(ofGetWidth(), ofGetHeight(), 10000, -10000)
	
	if(bdrawdebug){
        ofDrawAxis(100);
    }
	
	ofPushView();
	
	ofTranslate(-(colsmesh*distsmesh)*0.5, -(rowsmesh*distsmesh)*0.5, 0);
	
	blackmesh.draw();
	myhand.draw(true);
	
	ofPopView();
	
	simpleCam.end();
	
	// finally, a report:
	//ofSetHexColor(0xff0000);
	ofSetColor(255,0,0);
	char reportStr[1024];
	sprintf(reportStr, "deformable mesh\nKEY 'o' to select ID particles --\nKEY 'p' to select ID particles ++\nKEY 'm' for move particles\nKEY 'f' for apply vector forces to strings\nfps: %f", ofGetFrameRate());
	
	if(bdrawdebug){
        ofDrawBitmapString(reportStr, 20, 600);
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

	myhand.keypressed(key);
	
	blackmesh.keypressed(key);

	if(key == 'F' || key == 'f'){
		ofToggleFullscreen();
	}
	if (key == OF_KEY_F4) {
		bdrawdebug = !bdrawdebug;
	}
		
}
