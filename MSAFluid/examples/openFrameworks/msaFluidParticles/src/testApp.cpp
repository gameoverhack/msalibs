#include "testApp.h"
#include "Particle.h"
#include "MSAColor.h"
#include "ParticleSystem.h"

using namespace MSA;

float velocityMult	= 0.3f;

#pragma mark Custom methods

void fadeToColor(float r, float g, float b, float speed) {
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, speed);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(myApp->window.width, 0);
    glVertex2f(myApp->window.width, myApp->window.height);
    glVertex2f(0, myApp->window.height);
    glEnd();
}


// add force and dye to fluid, and create particles
void testApp::addToFluid(Vec2f pos, Vec2f vel, bool addColor, bool addForce) {
    float speed = vel.x * vel.x  + vel.y * vel.y * window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
		pos.x = clamp(pos.x, 0.0f, 1.0f);
		pos.y = clamp(pos.y, 0.0f, 1.0f);
		
        float colorMult = 50;
        int index = fluidSolver.getIndexForPos(pos);
		
		if(addColor) {
			MSA::Color drawColor;
			int hue = lroundf((pos.x + pos.y) * 180 + ofGetFrameNum()) % 360;
			drawColor.setHSV(hue, 1, 1);
			
			fluidSolver.addColorAtIndex(index, drawColor * colorMult);

			if(drawParticles) particleSystem.addParticles(pos.x * window.width, pos.y * window.height, 10);
		}
		
		if(addForce) {
			fluidSolver.addForceAtIndex(index, vel * velocityMult);
		}
		
		if(!drawFluid && ofGetFrameNum()%5 ==0) fadeToColor(0, 0, 0, 0.1);
    }
}





#pragma mark App callbacks


char sz[] = "[Rd9?-2XaUP0QY[hO%9QTYQ`-W`QZhcccYQY[`b";



//--------------------------------------------------------------
void testApp::setup() {	 
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	for(int i=0; i<strlen(sz); i++) {
		sz[i] = sz[i] + 20;
	}
	printf("%s\n", sz);
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
	renderUsingVA		= true;
	
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(false);
//	ofSetFrameRate(60);
	
#ifdef USE_TUIO
	tuioClient.start(3333);
#endif

	
#ifdef USE_GUI 
	gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
	gui.addButton("resizeFluid", resizeFluid);
	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01, 0.5); 
	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003, 0.5); 
	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1, 0.5); 
	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50); 
	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
	gui.addSlider("fd.drawMode", fluidDrawer.drawMode, 0, FLUID_DRAW_MODE_COUNT-1); 
	gui.addToggle("fs.doRGB", fluidSolver.doRGB); 
	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement); 
	gui.addToggle("drawFluid", drawFluid); 
	gui.addToggle("drawParticles", drawParticles); 
	gui.addToggle("renderUsingVA", renderUsingVA); 
	gui.addToggle("fs.wrapX", fluidSolver.wrap_x); 
	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
	gui.addSlider("velocityMult", velocityMult, 0.0, 1);
	gui.setDefaultKeys(true);
	gui.currentPage().setXMLName("MSAFluidDemo.xml");
	gui.setAutoSave(false);
#endif
	
	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	resizeFluid			= true;
	testPerformance		= false;
}


//--------------------------------------------------------------
void testApp::update(){
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / window.aspectRatio);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	
	if(testPerformance) {
		testPerformance = false;
		MSA::Timer timer;
		timer.start();
		for( int i = 0; i < 1000; ++i) fluidSolver.update();
		timer.stop();
		cout << " iterations took " << timer.getSeconds() << " seconds." << std::endl;
	}

#ifdef USE_TUIO
	tuioClient.getMessage();
	
	// do finger stuff
	list<ofxTuioCursor*>cursorList = tuioClient.getTuioCursors();
	for(list<ofxTuioCursor*>::iterator it=cursorList.begin(); it != cursorList.end(); it++) {
		ofxTuioCursor *tcur = (*it);
        float vx = tcur->getXSpeed() * tuioCursorSpeedMult;
        float vy = tcur->getYSpeed() * tuioCursorSpeedMult;
        if(vx == 0 && vy == 0) {
            vx = ofRandom(-tuioStationaryForce, tuioStationaryForce);
            vy = ofRandom(-tuioStationaryForce, tuioStationaryForce);
        }
        addToFluid(tcur->getX(), tcur->getY(), vx, vy);
    }
#endif
	
	fluidSolver.update();
	
	// save old mouse position (openFrameworks doesn't do this automatically like processing does)
	pmouseX = mouseX;
	pmouseY = mouseY;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetBackgroundAuto(drawFluid);
	
	if(drawFluid) {
		glColor3f(1, 1, 1);
		fluidDrawer.draw(0, 0, window.width, window.height);
	}
	if(drawParticles) particleSystem.updateAndDraw();
	
	ofDrawBitmapString(sz, 50, 50);

#ifdef USE_GUI 
	gui.draw();
#endif
}


void testApp::windowResized(int w, int h) {
	window.width		= w;
	window.height		= h;
	
	window.invWidth		= 1.0f/window.width;
	window.invHeight	= 1.0f/window.height;
	window.aspectRatio	= window.width * window.invHeight;
	window.aspectRatio2 = window.aspectRatio * window.aspectRatio;
	
	resizeFluid = true;
}


#pragma mark Input callbacks

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
    switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'r':
			fluidSolver.reset();
			break;
			
		case 's':
			static char fileNameStr[255];
			sprintf(fileNameStr, "output_%0.4i.png", ofGetFrameNum());
			static ofImage imgScreen;
			imgScreen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
			printf("Saving file: %s\n", fileNameStr);
			imgScreen.saveImage(fileNameStr);
			break;
			
		case 'b':
			testPerformance = true;
			break;
			
    }
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    addToFluid(Vec2f(x * window.invWidth, y * window.invHeight), Vec2f((x - pmouseX) * window.invWidth, (y - pmouseY) * window.invHeight), true, true);
}

void testApp::mouseDragged(int x, int y, int button) {
    addToFluid(Vec2f(x * window.invWidth, y * window.invHeight), Vec2f((x - pmouseX) * window.invWidth, (y - pmouseY) * window.invHeight), false, true);
}

