/*
 *  InteractiveMesh.cpp
 *  mallanegra
 *
 *  Created by Dude on 20/12/11.
 *  Copyright 2011 lummo. All rights reserved.
 *
 */

#include "InteractiveMesh.h"


//--------------------------------------------------------------
void InteractiveMesh::setup(int _rowsmesh, int _colsmesh, int _distsmesh, float _particle_drag, float _string_k, int _idParticle){

	//////////////////////////////////////////////////////////////////////////
	//Setup Mesh
	rowsmesh = _rowsmesh;//= 10*2;
	colsmesh = 	_colsmesh;// 10*2;
	distsmesh = _distsmesh;//20;
	particle_drag = _particle_drag;//0.98;
	string_k = _string_k;//0.001;
	
	smesh.setup(rowsmesh,colsmesh,distsmesh, particle_drag, string_k, _idParticle);
	
	for (int i=0; i<rowsmesh*colsmesh; i++) {
		smesh.setParticleDrag(i, particle_drag);
	}
	cout << "particle_drag = " << particle_drag << endl;
	
	//Use second level
	smesh.updateLevelMeshNode(1);

	levelps2draw = 0;
	
	//Copy main identifier particle of mesh
	idmeshnode = smesh.getIdupdatedparticle();
	
	//vars from mesh to send data
	posMesh = ofVec3f(0,0,0);
	lastposMesh = ofVec3f(0,0,0);
	acc = 0;
	
	//////////////////////////////////////////////////////////////////////////
	//Setup vectors
	
	//Setup initial forces to the mesh
	inimeshpoints.reserve(rowsmesh*colsmesh);
	updatedmeshpoints.reserve(rowsmesh*colsmesh);
	distupdated2inimeshpoints.reserve(rowsmesh*colsmesh);
	
	for (int i = 0; i < rowsmesh*colsmesh; i++)
	{
		inimeshpoints[i]			= ofVec3f(0,0,0);
		updatedmeshpoints[i]		= ofVec3f(0,0,0);
		smesh.updateForceMeshNode(i, ofVec3f(0,0,0));
		distupdated2inimeshpoints[i] = 0;					  
	}
	
	infolevelready2send.reserve(8);
	
	//Fill empty vector
	for(int i=0;i++;i<8)
	{
		infolevelready2send[i] = 0.0f;
	}

	//////////////////////////////////////////////////////////////////////////
	//General Vars

	//Setup lastHandPos
	last_handpos = ofVec3f(0,0,0);
	bfirstUpdate = true;
	
	//setup key menu options
	bkeyparticles = false;
	bkeydrag = false;
	bkeymesh = false;

	bdrawdebug = true;
	bdrawSprings = true;
	
	//////////////////////////////////////////////////////////////////////////
	//OSC Port setup
	sender1.setup( HOST1, PORT1 );
	//bSenderStarted = false;
	receiver.setup(PORT2);
	

}



//--------------------------------------------------------------
void InteractiveMesh::update(ofVec3f myhandpos){

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Receive Level to read
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		// check for mouse moved message
		if ( m.getAddress() == "/INFOPD" )
		{
			// both the arguments are int32's
			levelps2draw = m.getArgAsInt32( 0 ) +1 ;
			smesh.updateReadLevelMesh(levelps2draw);
			cout << "received level = " <<  levelps2draw  << endl;
			//and Fill the info level, just 8 values
			calcInfoLevelRead(levelps2draw);
		}
	}
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//update with hand movements
	if( myhandpos != last_handpos)
	{
		if (bfirstUpdate) {
			last_handpos = myhandpos;
			bfirstUpdate = false;
			return;
		}
	
		ofVec3f aux2particleposition = smesh.getParticlePosition(idmeshnode);
		//cout << "aux2particleposition x= " << aux2particleposition.x << " y= " << aux2particleposition.y << " z= " << aux2particleposition.z << endl;
		
		/*
		ofVec3f vectorParticle2hand = aux2particleposition - myhandpos;
		cout << "vectorParticle2hand x= " << vectorParticle2hand.x << " y= " << vectorParticle2hand.y << " z= " << vectorParticle2hand.z << endl;
		//apply distance from movement as vector force to the selected dir
		//aux2particleposition = auxparticleposition - ofVec3f(auxparticleposition.x,auxparticleposition.y, auxparticleposition.z - aux2particleposition.distance(auxparticleposition));
		ofVec3f applynewlocation = aux2particleposition + vectorParticle2hand;
		cout << "applynewlocation x= " << applynewlocation.x << "y = " << applynewlocation.y << " z= " << applynewlocation.z << endl;
		*/
		
	smesh.setParticlePosition(idmeshnode, myhandpos);
	//cout << "myhandpos x= " << myhandpos.x << "y = " << myhandpos.y << " z= " << myhandpos.z << endl;
		
	ofVec3f stepForceVector = myhandpos - last_handpos;
	smesh.updateForceMeshNode(idmeshnode, stepForceVector);
		
		//Get last points updated
		updatedmeshpoints = smesh.getUpdatedPoints();
		
		
		//Update mesh just necesery if setparticleposition...
		//smesh.update();
		
		last_handpos = myhandpos;
		
		
		//update distance points mesh
		//Calculate difference from initial points
		for (int i = 0; i < rowsmesh*colsmesh; i++)
		{
			//distupdated2inimeshpoints[i] = inimeshpoints[i].distance(updatedmeshpoints[i]);
			//cout << "smesh.getParticles()[" << i << "]" << ".pos = " << smesh.getParticles()[i].pos << endl;
			distupdated2inimeshpoints[i] = inimeshpoints[i].distance(smesh.getParticles()[i].pos);
			//cout << "distupdated2inimeshpoints[" << i << "]" << " = " << distupdated2inimeshpoints[i] << endl;
		}
		
		//and Fill the info level, just 8 values
		calcInfoLevelRead(levelps2draw);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bmesh_updatingPoints = smesh.checkLargeMovements(idmeshnode, 90);
	
	if(bmesh_updatingPoints)
	{
		smesh.update();
		
		//Calculate Mesh acceleration usable for PureData send by OSC
		acc = calcAccMesh();
		cout << "RAW acc = " << acc << endl;		
		
		//Consultar la Acc MAX i assignarla... watch !
		//acc = ofMap(acc, 0,35, 0, 1);
		//cout << "Mapped ACC = " << acc << endl;		
	
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//Send OSC packets of Center of Mesh
	ofVec3f auxparticleposition = smesh.getParticlePosition(idmeshnode);
	
	ofxOscMessage m;
	m.setAddress( "/INFOMALLA" );
	m.addFloatArg( auxparticleposition.x );
	m.addFloatArg( auxparticleposition.y );
	m.addFloatArg( auxparticleposition.z );
	
	if (bmesh_updatingPoints)m.addFloatArg( 1 );
	else m.addFloatArg( 0 );
	//Sending acc of the mesh
	m.addFloatArg( acc );
	
	for (int i=0;i<8;i++)
	{
	  m.addFloatArg( infolevelready2send[i] ); 
	  //cout << "sending info about levels: [" << i << "] = " << infolevelready2send[i] << endl; 
	}
	
	//Send Packcage Message
	sender1.sendMessage( m );
	
	//bSenderStarted = bSenderStarted + true;
		
	/*
	else {
		
		if(bSenderStarted)
		{
			ofxOscMessage m;
			m.setAddress( "/BLOB_DEPTH" );
			m.addFloatArg( auxparticleposition.x );
			m.addFloatArg( auxparticleposition.y );
			m.addFloatArg( auxparticleposition.z );
			
			//Updating mesh points
			m.addFloatArg( 0 );
			//Sending acc of the mesh
			m.addFloatArg( acc );
			
			//sender1.sendMessage( m );
			sender1.sendMessage( m );
		}
	}*/
	
	
}

//--------------------------------------------------------------
void InteractiveMesh::calcInfoLevelRead(int levelparticles){
	
	int counterInfoReady = 0;
	
	for (int i=0; i<colsmesh*rowsmesh; i++) {
	
		if (	(i == idmeshnode + levelparticles) 
			||  (i == idmeshnode - levelparticles)
			||	(i == idmeshnode + levelparticles * colsmesh) || (i == idmeshnode - levelparticles * colsmesh) 
			||	(i == idmeshnode + levelparticles + (levelparticles * colsmesh)) || (i == idmeshnode + levelparticles + (-levelparticles * colsmesh))
			||	(i == idmeshnode - levelparticles + (levelparticles * colsmesh)) || (i == idmeshnode - levelparticles + (-levelparticles * colsmesh)) 
			){
			
			cout << "value to map [" << i << "]=" << distupdated2inimeshpoints[i] << " from level = " << levelps2draw << endl;
			
			//Adapt to right values, watch!
			//float mapedvalue = ofMap(distupdated2inimeshpoints[i], 0, 2000, 0, 1, false);
			

			infolevelready2send[counterInfoReady] =  distupdated2inimeshpoints[i];
			
			if(counterInfoReady < 8)counterInfoReady++;
		}

	}

}


//--------------------------------------------------------------
float InteractiveMesh::calcAccMesh()
{
	posMesh = smesh.getParticlePosition(idmeshnode);
	
	float auxdistance = posMesh.distance(lastposMesh);
	cout << "ACC auxdistance = " << auxdistance << endl;

	lastposMesh = posMesh;
	
	return auxdistance;
}


//--------------------------------------------------------------
void InteractiveMesh::draw(){
	ofSetColor(255, 255, 255);
	smesh.draw(bdrawdebug, bdrawSprings);
}

//--------------------------------------------------------------
ofVec3f InteractiveMesh::getPosIdupdatedparticle(){
	return smesh.getPosIdupdatedparticle();
}


//--------------------------------------------------------------
void InteractiveMesh::keypressed(int key){

	/*
	if ( key == 'f'){

		bkeydrag = false;
		bkeymesh = true;
		bkeyparticles = false;
		
		cout << "bkeydrag false" << endl;
		cout << "bkeycam false" << endl;
		cout << "bkeyparticles false" << endl;
	}
	else if ( key == 'm'){

		bkeydrag = false;
		bkeymesh = false;
		bkeyparticles = true;
		
		cout << "bkeydrag false" << endl;
		cout << "bkeycam false" << endl;
		cout << "bkeyparticles true" << endl;
	}
	else if ( key == 'D'){

		bkeydrag = true;
		bkeymesh = false;
		bkeyparticles = false;
		
		cout << "bkeydrag true" << endl;
		cout << "bkeycam false" << endl;
		cout << "bkeyparticles false" << endl;
	}
	 */
	
	//Change Selected Particle
	if(key == 'p'){
		idmeshnode++;
		smesh.updateIdMeshNode(idmeshnode);
		
		cout << "smesh.id :: " << smesh.getIdupdatedparticle() << endl;
	}
	else if(key == 'o'){
		idmeshnode--;
		smesh.updateIdMeshNode(idmeshnode);
		
		cout << "smesh.id :: " << smesh.getIdupdatedparticle() << endl;
	}
	//Set Levels of selected 
	else if(key == '0'){
		smesh.updateLevelMeshNode(0);
	}
	else if(key == '1'){
		smesh.updateLevelMeshNode(1);
	}	
	else if(key == OF_KEY_F1){
		bdrawSprings = ! bdrawSprings;
	}
	else if(key == OF_KEY_F2){
		bdrawdebug = !bdrawdebug;
	}
	else if(key == 'u'){
		if(levelps2draw>0)levelps2draw--;
		smesh.updateReadLevelMesh(levelps2draw);
		cout << "levelps2draw = " << levelps2draw << endl;
	}
	else if(key == 'i'){
		if(levelps2draw<8)levelps2draw++;
		smesh.updateReadLevelMesh(levelps2draw);
		cout << "levelps2draw = " << levelps2draw << endl;
	}
	else if(key == 'P')
	{
		for (int i = 0; i < rowsmesh*colsmesh; i++)
		{
			cout << "updatedmeshpoints [" << i << "]" << updatedmeshpoints[i] << endl;
		}
		smesh.printValues();
	}
	
/*
	
	if(bkeymesh)
	{
		if (key == '+') {
			cout << "bkeymesh +"<< endl;
			nodemesh_vectorforces[idmeshnode].z--;
			smesh.updateForceMeshNode(idmeshnode, nodemesh_vectorforces[idmeshnode]);
		}
		else if(key == '-'){
			cout << "bkeymesh -"<< endl;
			nodemesh_vectorforces[idmeshnode].z++;
			smesh.updateForceMeshNode(idmeshnode, nodemesh_vectorforces[idmeshnode]);
		}
		if (key == OF_KEY_DOWN) {
			nodemesh_vectorforces[idmeshnode].x--;
			smesh.updateForceMeshNode(idmeshnode, nodemesh_vectorforces[idmeshnode]);
		}
		else if(key == OF_KEY_UP){
			nodemesh_vectorforces[idmeshnode].x++;
			smesh.updateForceMeshNode(idmeshnode, nodemesh_vectorforces[idmeshnode]);
		}
		else if(key == OF_KEY_RIGHT){
			nodemesh_vectorforces[idmeshnode].y--;
			smesh.updateForceMeshNode(idmeshnode, nodemesh_vectorforces[idmeshnode]);
		}
		else if(key == OF_KEY_LEFT){
			nodemesh_vectorforces[idmeshnode].y++;
			smesh.updateForceMeshNode(idmeshnode, nodemesh_vectorforces[idmeshnode]);
		}
		
		cout << "bkeymesh smesh.update();" << endl;
		smesh.update();
		
	}
	else if(bkeyparticles){
		
		ofVec3f auxparticleposition = smesh.getParticlePosition(idmeshnode);
		
		//new copy aux
		//ofVec3f aux2particleposition = smesh.getParticlePosition(idmeshnode);
		
		
		if (key == '+') {

			cout << "bkeyparticles +"<< endl;
			//move point
			auxparticleposition.z = auxparticleposition.z-10;
			//move particle
			smesh.setParticlePosition(idmeshnode, auxparticleposition);
		}
		else if(key == '-'){
			
			cout << "bkeyparticles -"<< endl;
			
			auxparticleposition.z = auxparticleposition.z+10;
			smesh.setParticlePosition(idmeshnode, auxparticleposition);
		}
		if (key == OF_KEY_DOWN) {
			auxparticleposition.x--;
			smesh.setParticlePosition(idmeshnode, auxparticleposition);		
		}
		else if(key == OF_KEY_UP){
			auxparticleposition.x++;
			smesh.setParticlePosition(idmeshnode, auxparticleposition);		
		}
		else if(key == OF_KEY_RIGHT){
			auxparticleposition.y--;
			smesh.setParticlePosition(idmeshnode, auxparticleposition);		
		}
		else if(key == OF_KEY_LEFT)
		{
			auxparticleposition.y++;
			smesh.setParticlePosition(idmeshnode, auxparticleposition);
		}
		
		smesh.update();
		cout << "bkeyparticles smesh.update();" << endl;
		
	}
	else if(bkeydrag){
		

		if (key == '+') {
			
			cout << "bkeydrag +"<< endl;
			//move point
			particle_drag = particle_drag + 0.1;
			
			//set all particle drag
			for (int i=0; i<rowsmesh*colsmesh; i++) {
				smesh.setParticleDrag(i, particle_drag);
			}
			cout << "bkeydrag = "<< particle_drag << endl;
			
		}
		else if(key == '-'){
			
			cout << "bkeydrag -"<< endl;
			
			particle_drag = particle_drag - 0.1;
			
			//set all particle drag
			for (int i=0; i<rowsmesh*colsmesh; i++) {
				smesh.setParticleDrag(i, particle_drag);
			}
			cout << "bkeydrag = "<< particle_drag << endl;
		}

		
	}
*/
	
	
}
