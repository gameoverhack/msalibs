#include "testApp.h"

#include "MSAOpenCL.h"

#define USE_CAMERA                      // Comment out this #define to use a video clip instead of a webcam
                                        // added by gameover (matt gingold)
#ifdef USE_CAMERA
ofVideoGrabber	video;
#else
ofVideoPlayer   video;
string          url = "E:/gameoverload/VideoProjects/PerfectArtistProject/RenderFinals/ANIMECodec/PA_DW_DANC_SEG_WID_PERHIH_ENGHIH_EMTPOS_M_Daniel_Cunningham.mov";
#endif

int					vidWidth;
int					vidHeight;
float				captureFPS;				// video capture fps


MSA::OpenCL			openCL;
MSA::OpenCLImage	clImage[2];				// two OpenCL images

#ifndef TARGET_OSX
ofTexture		    texture[2];
cl_sampler		    clSampler;				// sampler used for sampling clImages
#endif

int					activeImageIndex = 0;

unsigned char		*pixels;				// temp buffer


// parameters
bool				doBlur		= true;
int					blurAmount	= 5;
bool				doFlipX		= true;
bool				doFlipY		= false;
bool				doGreyscale	= true;
bool				doInvert	= true;
bool				doThreshold	= false;
cl_float			threshLevel	= 0.5;


//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(50, 50, 50);


	// dump everything to console
	ofSetLogLevel(OF_LOG_VERBOSE);


	// disable vsync (to allow >60fps)
	ofSetVerticalSync(false);


    // init video/grabber
#ifdef USE_CAMERA
	video.initGrabber(640, 480);
#else
	video.loadMovie(url);
	video.play();
#endif

	vidWidth	= video.getWidth();
	vidHeight	= video.getHeight();


	// allocate temp buffer
	pixels		= new unsigned char[vidWidth * vidHeight * 4];


	// init OpenCL from OpenGL context to enable GL-CL data sharing
	openCL.setupFromOpenGL();

#ifdef TARGET_OSX
	// create OpenCL textures and related OpenGL textures
	clImage[0].initWithTexture(vidWidth, vidHeight, GL_RGBA);
	clImage[1].initWithTexture(vidWidth, vidHeight, GL_RGBA);
#else
    // allocate textures
	texture[0].allocate(vidWidth, vidHeight, GL_RGBA);
	texture[1].allocate(vidWidth, vidHeight, GL_RGBA);
	// create OpenCL textures mapped to OpenGL textures
	clImage[0] = *(openCL.createImageFromTexture(texture[0]));      // it works, but this referencing is nasty -> causing unclean exits on WIN32
	clImage[1] = *(openCL.createImageFromTexture(texture[1]));
#endif

	// load and compile OpenCL program
	openCL.loadProgramFromFile("MSAOpenCL/ImageProcessing.cl");


	// load kernels
	openCL.loadKernel("msa_boxblur");
	openCL.loadKernel("msa_flipx");
	openCL.loadKernel("msa_flipy");
	openCL.loadKernel("msa_greyscale");
	openCL.loadKernel("msa_invert");
	openCL.loadKernel("msa_threshold");

	// create image sampler
	// gameover: has been hard coded into the kernels - maybe should be kept as an argument??
	//clSampler = clCreateSampler(openCL.getContext(), CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST, NULL);

}

//--------------------------------------------------------------
void testApp::update(){

	// grab new frame
	video.update();

	// if there is a new frame....
	if(video.isFrameNew()) {

		// RGB textures don't seem to work well. so need to copy the vidgrabber data into a RGBA texture
		int pixelIndex = 0;
		for(int i=0; i<vidWidth; i++) {
			for(int j=0; j<vidHeight; j++) {
				int indexRGB	= pixelIndex * 3;
				int indexRGBA	= pixelIndex * 4;
				pixels[indexRGBA  ] = video.getPixels()[indexRGB  ];
				pixels[indexRGBA+1] = video.getPixels()[indexRGB+1];
				pixels[indexRGBA+2] = video.getPixels()[indexRGB+2];
				pixels[indexRGBA+3] = 255;
				pixelIndex++;
			}
		}

#ifdef TARGET_OSX
		// write the new pixel data into the OpenCL Image (and thus the OpenGL texture)
		clImage[activeImageIndex].write(pixels);
#else
        texture[activeImageIndex].loadData(pixels, vidWidth, vidHeight, GL_RGBA);
        glFinish();
#endif

        clEnqueueAcquireGLObjects(openCL.getQueue(), 1, &clImage[activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
        clEnqueueAcquireGLObjects(openCL.getQueue(), 1, &clImage[1-activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)

		if(doBlur) {
			MSA::OpenCLKernel *kernel = openCL.kernel("msa_boxblur");
			for(int i=0; i<blurAmount; i++) {
				cl_int offset = i * i / 2 + 1;
				kernel->setArg(0, clImage[activeImageIndex].getCLMem());
				kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
				kernel->setArg(2, offset);
                kernel->run2D(vidWidth, vidHeight);
				activeImageIndex = 1 - activeImageIndex;
			}
		}

		if(doFlipX) {
			MSA::OpenCLKernel *kernel = openCL.kernel("msa_flipx");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}


		if(doFlipY) {
			MSA::OpenCLKernel *kernel = openCL.kernel("msa_flipy");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doGreyscale) {
			MSA::OpenCLKernel *kernel = openCL.kernel("msa_greyscale");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doInvert) {
			MSA::OpenCLKernel *kernel = openCL.kernel("msa_invert");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doThreshold) {
			MSA::OpenCLKernel *kernel = openCL.kernel("msa_threshold");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
			kernel->setArg(2, threshLevel);
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

        clEnqueueReleaseGLObjects(openCL.getQueue(), 1, &clImage[activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
        clEnqueueReleaseGLObjects(openCL.getQueue(), 1, &clImage[1-activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)

		// calculate capture fps
		static float lastTime = 0;
		float nowTime = ofGetElapsedTimef();
		float timeDiff = nowTime - lastTime;
		if(timeDiff > 0 ) captureFPS = 0.9f * captureFPS + 0.1f / timeDiff;
		lastTime = nowTime;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	video.draw(0, 200);

	// make sure all OpenCL kernels have finished executing before drawing
	openCL.finish();

#ifdef TARGET_OSX
	// draw the OpenGL texture (which was mapped to the OpenCL image)
	clImage[activeImageIndex].getTexture().draw(vidWidth, 200);
#else
	texture[activeImageIndex].draw(vidWidth, 200);
#endif

	ofDrawBitmapString(    " app FPS            : " + ofToString(ofGetFrameRate(), 2)
					   + "\n capture FPS        : " + ofToString(captureFPS, 2)
					   + "\n"
					   + "\n doBlur (b)         : " + (doBlur ? "X" : "")
					   + "\n   blurAmount (1-9) : " + ofToString(blurAmount)
					   + "\n doFlipX (x)        : " + (doFlipX ? "X" : "")
					   + "\n doFlipY (y)        : " + (doFlipY ? "X" : "")
					   + "\n doGreyscale (g)    : " + (doGreyscale ? "X" : "")
					   + "\n doInvert (i)       : " + (doInvert ? "X" : "")
					   + "\n doThreshold (t)    : " + (doThreshold ? "X" : "")
					   + "\n   threshLevel ([]) : " + ofToString(threshLevel, 2)
					   , 30, 30);
}

//--------------------------------------------------------------
void testApp::exit() {

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {
		case 'b':
			doBlur ^= true;
			break;

		case 'x':
			doFlipX ^= true;
			break;

		case 'y':
			doFlipY ^= true;
			break;

		case 'g':
			doGreyscale ^= true;
			break;

		case 'i':
			doInvert ^= true;
			break;

		case 't':
			doThreshold ^= true;
			break;

		case 's':
#ifdef USE_CAMERA
			video.videoSettings();
#endif
			break;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			blurAmount = key - '0';
			break;

		case '[':
			threshLevel -= 0.05;
			if(threshLevel<0) threshLevel = 0;
			break;

		case ']':
			threshLevel += 0.05;
			if(threshLevel>1) threshLevel = 1;
			break;
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

