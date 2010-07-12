
#include "testApp.h"

testApp *myApp;
int main( ){
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
	myApp = new testApp;
	ofRunApp(myApp);
}
