#include "cinder/app/AppBasic.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include "MSAPhysics.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace MSA;

#define	SPRING_MIN_STRENGTH		0.005f
#define SPRING_MAX_STRENGTH		0.1f

#define	SPRING_MIN_WIDTH		1.0f
#define SPRING_MAX_WIDTH		3.0f

#define NODE_MIN_RADIUS			5.0f
#define NODE_MAX_RADIUS			15.0f

#define MIN_MASS				1.0f
#define MAX_MASS				3.0f

#define MIN_BOUNCE				0.2f
#define MAX_BOUNCE				0.9f

#define	FIX_PROBABILITY			10.0f		// % probability of a particle being fixed on creation

#define FORCE_AMOUNT			10.0f

#define EDGE_DRAG				0.98f

#define	GRAVITY					1.0f

#define MAX_ATTRACTION			10.0f
#define MIN_ATTRACTION			3.0f

#define SECTOR_COUNT			1		// currently there is a bug at sector borders

bool				mouseAttract	= false;
bool				doMouseXY		= false;		// pressing left mmouse button moves mouse in XY plane
bool				doMouseYZ		= false;		// pressing right mouse button moves mouse in YZ plane
bool				doRender		= true;
int					forceTimer		= false;


float				rotSpeed		= 0;
float				mouseMass		= 1;

static float		width;
static float		height;


Physics::World		physics;
Physics::Particle	mouseNode;

gl::Texture		ballImage;	

// We'll create a new Cinder Application by deriving from the AppBasic class
class msaPhysicsBasicApp : public AppBasic {
public:

	void prepareSettings(Settings *settings) {
		settings->setWindowSize( 1280, 720);
		settings->setFrameRate( 60.0f );
	}
	
	
	
	void initScene() {
		// clear all particles and springs etc
		physics.clear();
		
		// you can add your own particles to the physics system
		physics.addParticle(&mouseNode);
		mouseNode.makeFixed();
		mouseNode.setMass(MIN_MASS);
		mouseNode.moveTo(Vec3f(0, 0, 0));
		mouseNode.setRadius(NODE_MAX_RADIUS);
		
		// or tell the system to create and add particles
		physics.makeParticle(Vec3f(-width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top left back and fix
		physics.makeParticle(Vec3f( width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top right back and fix
		physics.makeParticle(Vec3f(-width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top left front and fix
		physics.makeParticle(Vec3f( width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top right front and fix
	}
	
	
	void setup(){
		ballImage = gl::Texture( loadImage( loadResource( "ball.png" ) ) );
		
		width = getWindowWidth();
		height = getWindowHeight();
		
		//	physics.verbose = true;			// dump activity to log
		physics.setGravity(Vec3f(0, GRAVITY, 0));
		
		// set world dimensions, not essential, but speeds up collision
		physics.setWorldSize(Vec3f(-width/2, -height, -width/2), Vec3f(width/2, height, width/2));
		physics.setSectorCount(SECTOR_COUNT);
		physics.setDrag(0.97f);
		physics.setDrag(1);		// FIXTHIS
		physics.enableCollision();
		
		initScene();
		
		gl::enableAlphaBlending();

		// setup lighting
		GLfloat mat_shininess[] = { 50.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_position[] = { 0, height/2, 0.0, 0.0 };
		glShadeModel(GL_SMOOTH);
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHT0);
		
		// enable back-face culling (so we can see through the walls)
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		
	}
	
	
	void addRandomParticle() {
		float posX		= Rand::randFloat(-width/2, width/2);
		float posY		= Rand::randFloat(0, height);
		float posZ		= Rand::randFloat(-width/2, width/2);
		float mass		= Rand::randFloat(MIN_MASS, MAX_MASS);
		float bounce	= Rand::randFloat(MIN_BOUNCE, MAX_BOUNCE);
		float radius	= mapRange(mass, MIN_MASS, MAX_MASS, NODE_MIN_RADIUS, NODE_MAX_RADIUS);
		
		// physics.makeParticle returns a particle pointer so you can customize it
		Physics::Particle* p = physics.makeParticle(Vec3f(posX, posY, posZ));
		
		// and set a bunch of properties (you don't have to set all of them, there are defaults)
		p->setMass(mass)->setBounce(bounce)->setRadius(radius)->enableCollision()->makeFree();
		
		// add an attraction to the mouseNode
		if(mouseAttract) physics.makeAttraction(&mouseNode, p, Rand::randFloat(MIN_ATTRACTION, MAX_ATTRACTION));
	}
	
	void addRandomSpring() {
		Physics::Particle *a = physics.getParticle((int)Rand::randFloat(0, physics.numberOfParticles()));
		Physics::Particle *b = physics.getParticle((int)Rand::randFloat(0, physics.numberOfParticles()));
		physics.makeSpring(a, b, Rand::randFloat(SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH), Rand::randFloat(10, width/2));
	}
	
	
	void killRandomParticle() {
		Physics::Particle *p = physics.getParticle(floor(Rand::randFloat(0, physics.numberOfParticles())));
		if(p && p != &mouseNode) p->kill();
	}
	
	void killRandomSpring() {
		Physics::Spring *s = physics.getSpring( floor(Rand::randFloat(0, physics.numberOfSprings())));
		if(s) s->kill();
	}
	
	void killRandomConstraint() {
		Physics::Constraint *c = physics.getConstraint(floor(Rand::randFloat(0, physics.numberOfConstraints())));
		if(c) c->kill();
	}
	
	
	void toggleMouseAttract() {
		mouseAttract = !mouseAttract;
		if(mouseAttract) {
			// loop through all particles and add attraction to mouse
			// (doesn't matter if we attach attraction from mouse-mouse cos it won't be added internally
			for(int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(&mouseNode, physics.getParticle(i), Rand::randFloat(MIN_ATTRACTION, MAX_ATTRACTION));
		} else {
			// loop through all existing attractsions and delete them
			for(int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->kill();
		}
	}
	
	void addRandomForce(float f) {
		forceTimer = f;
		for(int i=0; i<physics.numberOfParticles(); i++) {
			Physics::Particle *p = physics.getParticle(i);
			if(p->isFree()) p->addVelocity(Vec3f(Rand::randFloat(-f, f), Rand::randFloat(-f, f), Rand::randFloat(-f, f)));
		}
	}
	
	void lockRandomParticles() {
		for(int i=0; i<physics.numberOfParticles(); i++) {
			Physics::Particle *p = physics.getParticle(i);
			if(Rand::randFloat(0, 100) < FIX_PROBABILITY) p->makeFixed();
			else p->makeFree();
		}
		mouseNode.makeFixed();
	}
	
	void unlockRandomParticles() {
		for(int i=0; i<physics.numberOfParticles(); i++) {
			Physics::Particle *p = physics.getParticle(i);
			p->makeFree();
		}
		mouseNode.makeFixed();
	}
	
	
	
	//--------------------------------------------------------------
	void update() {
		width = getWindowWidth();
		height = getWindowHeight();
		
		physics.update();
	}
	
	
	//--------------------------------------------------------------
	void draw() {
		gl::clear( Color( 1.0f, 1.0f, 1.0f ) );
		
		if(doRender) {
			
			glEnable(GL_DEPTH_TEST);
			
			glPushMatrix();
			
			glTranslatef(width/2, 0, -width / 3);		// center scene
			static float rot = 0;
			glRotatef(rot, 0, 1, 0);			// rotate scene
			rot += rotSpeed;						// slowly increase rotation (to get a good 3D view)
			
			if(forceTimer) {
				float translateMax = forceTimer;
				glTranslatef(Rand::randFloat(-translateMax, translateMax), Rand::randFloat(-translateMax, translateMax), Rand::randFloat(-translateMax, translateMax));
				forceTimer--;
			}
			
			
			glDisable(GL_LIGHTING);
			glBegin(GL_QUADS);
			// draw right wall
			glColor3f(.9, 0.9, 0.9);		glVertex3f(width/2, height+1, width/2);
			glColor3f(1, 1, 1);				glVertex3f(width/2, -height, width/2);
			glColor3f(0.95, 0.95, 0.95);	glVertex3f(width/2, -height, -width/2);
			glColor3f(.85, 0.85, 0.85);		glVertex3f(width/2, height+1, -width/2);
			
			// back wall
			glColor3f(.9, 0.9, 0.9);		glVertex3f(width/2, height+1, -width/2);
			glColor3f(1, 1, 1);				glVertex3f(width/2, -height, -width/2);
			glColor3f(0.95, 0.95, 0.95);	glVertex3f(-width/2, -height, -width/2);
			glColor3f(.85, 0.85, 0.85);		glVertex3f(-width/2, height+1, -width/2);
			
			// left wall
			glColor3f(.9, 0.9, 0.9);		glVertex3f(-width/2, height+1, -width/2);
			glColor3f(1, 1, 1);				glVertex3f(-width/2, -height, -width/2);
			glColor3f(0.95, 0.95, 0.95);	glVertex3f(-width/2, -height, width/2);
			glColor3f(.85, 0.85, 0.85);		glVertex3f(-width/2, height+1, width/2);
			
			// front wall
			glColor3f(0.95, 0.95, 0.95);	glVertex3f(width/2, -height, width/2);
			glColor3f(.85, 0.85, 0.85);		glVertex3f(width/2, height+1, width/2);
			glColor3f(.9, 0.9, 0.9);		glVertex3f(-width/2, height+1, width/2);
			glColor3f(1, 1, 1);				glVertex3f(-width/2, -height, width/2);
			
			// floor
			glColor3f(.8, 0.8, 0.8);
			glVertex3f(width/2, height+1, width/2);
			glVertex3f(width/2, height+1, -width/2);
			glVertex3f(-width/2, height+1, -width/2);
			glVertex3f(-width/2, height+1, width/2);
			glEnd();
			
			//		glEnable(GL_LIGHTING);
			
			// draw springs
			glColor4f(0.5, 0.5, 0.5, 0.5);
			for(int i=0; i<physics.numberOfSprings(); i++) {
				Physics::Spring *spring = (Physics::Spring *) physics.getSpring(i);
				Physics::Particle *a = spring->getOneEnd();
				Physics::Particle *b = spring->getTheOtherEnd();
				Vec3f vec = b->getPosition() - a->getPosition();
				float dist = vec.length();
				float angle = acos( vec.z / dist ) * RAD_TO_DEG;
				if(vec.z <= 0 ) angle = -angle;
				float rx = -vec.y * vec.z;
				float ry =  vec.x * vec.z;
				
				glPushMatrix();
				glTranslatef(a->getX(), a->getY(), a->getZ());
				glRotatef(angle, rx, ry, 0.0);
				float size  = mapRange(spring->strength, SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH, SPRING_MIN_WIDTH, SPRING_MAX_WIDTH);
				
				glScalef(size, size, dist);
				gl::drawCube(Vec3f(0, 0, 0.5), Vec3f(1, 1, 1));
				glPopMatrix();
			}
			
			
			
			// draw particles
			glAlphaFunc(GL_GREATER, 0.5);
			
			if(ballImage) ballImage.enableAndBind();
			
			for(int i=0; i<physics.numberOfParticles(); i++) {
				Physics::Particle *p = physics.getParticle(i);
				if(p->isFixed()) glColor4f(1, 0, 0, 1);
				else glColor4f(1, 1, 1, 1);
				
				glEnable(GL_ALPHA_TEST);
				
				// draw ball
				glPushMatrix();
				glTranslatef(p->getX(), p->getY(), p->getZ());
				glRotatef(180-rot, 0, 1, 0);

				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(-p->getRadius(), -p->getRadius());
				glTexCoord2f(1, 0); glVertex2f(p->getRadius(), -p->getRadius());
				glTexCoord2f(1, 1); glVertex2f(p->getRadius(), p->getRadius());
				glTexCoord2f(0, 1); glVertex2f(-p->getRadius(), p->getRadius());
				glEnd();
				glPopMatrix();
				
				glDisable(GL_ALPHA_TEST);
				
				float alpha = mapRange(p->getY(), -height * 1.5f, height, 0.0f, 1.0f);
				if(alpha>0) {
					glPushMatrix();
					glTranslatef(p->getX(), height, p->getZ());
					glRotatef(-90, 1, 0, 0);
					glColor4f(0, 0, 0, alpha * alpha * alpha * alpha);
					//				ofCircle(0, 0, p->getRadius());
					float r = p->getRadius() * alpha;
					glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex2f(-r, -r);
					glTexCoord2f(1, 0); glVertex2f(r, -r);
					glTexCoord2f(1, 1); glVertex2f(r, r);
					glTexCoord2f(0, 1); glVertex2f(-r, r);
					glEnd();
					glPopMatrix();
				}
				
			}
			if(ballImage) ballImage.unbind();
			glPopMatrix();
		}
		
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor4f(0, 0, 0, 1);
		
//		console() << " FPS: " << getFrameRate()
//		<< " | Number of particles: " << physics.numberOfParticles()
//		<< " | Number of springs: " << physics.numberOfSprings()
//		<< " | Mouse Mass: " << mouseNode.getMass();
		
	}
	
	void keyDown(KeyEvent event) {
		switch(event.getChar()) {
			case 'a': toggleMouseAttract(); break;
			case 'p': for(int i=0; i<100; i++) addRandomParticle(); break;
			case 'P': for(int i=0; i<100; i++) killRandomParticle(); break;
			case 's': addRandomSpring(); break;
			case 'S': killRandomSpring(); break;
			case 'c': physics.isCollisionEnabled() ? physics.disableCollision() : physics.enableCollision(); break;
			case 'C': killRandomConstraint(); break;
			case 'r': doRender ^= true; break;
			case 'f': addRandomForce(FORCE_AMOUNT); break;
			case 'F': addRandomForce(FORCE_AMOUNT * 3); break;
			case 'l': lockRandomParticles(); break;
			case 'u': unlockRandomParticles(); break;
			case ' ': initScene(); break;
			case 'x': doMouseXY = true; break;
			case 'z': doMouseYZ = true; break;
			case ']': rotSpeed += 0.01f; break;
			case '[': rotSpeed -= 0.01f; break;
			case '+': mouseNode.setMass(mouseNode.getMass() +0.1); break;
			case '-': mouseNode.setMass(mouseNode.getMass() -0.1); break;
			case 'm': mouseNode.hasCollision() ? mouseNode.disableCollision() : mouseNode.enableCollision();
		}
	}
	
	void keyUp(KeyEvent event){
		switch(event.getChar()) {
			case 'x': doMouseXY = false; break;
			case 'z': doMouseYZ = false; break;
		}
	}
	
	void move(float x, float y) {
		static int oldMouseX = -10000;
		static int oldMouseY = -10000;
		int velX = x - oldMouseX;
		int velY = y - oldMouseY;
		if(doMouseXY) mouseNode.moveBy(Vec3f(velX, velY, 0));
		if(doMouseYZ) mouseNode.moveBy(Vec3f(velX, 0, velY));
		oldMouseX = x;
		oldMouseY = y;
	}
	
	void mouseMove(MouseEvent event) {
		move(event.getPos().x,event.getPos().y);
	}
	
	void mouseDrag(MouseEvent event) {
		if(event.isLeftDown()) doMouseXY = true;
		else doMouseYZ = true;
		move(event.getPos().x,event.getPos().y);
	}
	
	void mouseUp(MouseEvent event) {
		doMouseXY = doMouseYZ = false;
	}
	
	
};
// This line tells Cinder to actually create the application
CINDER_APP_BASIC( msaPhysicsBasicApp, RendererGl )
