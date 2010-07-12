#include "testApp.h"

#include "ofxMSABulletPhysics.h"

ofxMSABulletRigidWorld world;

//--------------------------------------------------------------
void testApp::setup(){	 
	world.setup(ofPoint(0, 0, 0), ofPoint(ofGetWidth(), ofGetHeight(), 0));
	
//	{
//		btCollisionShape* groundShape = new btBoxShape(btVector3(ofGetWidth(), 50, 50));
//		//	collisionShapes.push_back(groundShape);
//		btTransform groundTransform;
//		groundTransform.setIdentity();
//		groundTransform.setOrigin(btVector3(0, ofGetHeight()-10,0));
//		
//		
//		btScalar mass(0.);
//		//rigidbody is dynamic if and only if mass is non zero, otherwise static
//		bool isDynamic = (mass != 0.f);
//		
//		btVector3 localInertia(0,0,0);
//		if (isDynamic)
//			groundShape->calculateLocalInertia(mass,localInertia);
//		
//		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
//		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
//		btRigidBody* body = new btRigidBody(rbInfo);
//		
//		//add the body to the dynamics world
//		world.addBulletRigidBody(body);
//	}	
	
	{
		
		//create a dynamic rigidbody
		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(10.));
		//	collisionShapes.push_back(colShape);
		
		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();
		btScalar	mass(1.f);
		
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);
		
		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);
		
		startTransform.setOrigin(btVector3(ofGetWidth()/2,ofGetHeight()/2,0));
		
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		world.addBulletRigidBody(body);
	}
	
	ofPoint p(700, 200, 0);
	world.createRigidSphere(p, 10);
	
}

//--------------------------------------------------------------
void testApp::update(){
	world.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 255, 255);
	
	int n = world.getBulletWorld().getNumCollisionObjects();

	world.debugDraw();
	
	for (int j=n-1; j>=0 ;j--)
	{
		btCollisionObject* obj = world.getBulletWorld().getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);
//			printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
			glPushMatrix();
			glTranslatef(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
			float radius = ((btSphereShape*) body->getCollisionShape())->getRadius();
//			getRadius()
			ofCircle(0, 0, radius);
			glPopMatrix();
			
		}
	}
	
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 2) + "\nnumBodies: " + ofToString(n), 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	for(int i=0; i<10; i++) {
		ofPoint p(ofRandomWidth(), ofRandomHeight(), 0);
		world.createRigidSphere(p, ofRandom(10, 50));

	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
}
