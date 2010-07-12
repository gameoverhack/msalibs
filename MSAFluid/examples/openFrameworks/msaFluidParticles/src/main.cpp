
#include "msaFluidParticlesApp.h"

msaFluidParticlesApp *myApp;
int main( ){
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
	myApp = new msaFluidParticlesApp;
	ofRunApp(myApp);
}
