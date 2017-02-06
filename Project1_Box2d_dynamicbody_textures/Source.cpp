#include <Box2D/Box2D.h>
#include <glfw/glfw3.h>
#include <SOIL.h>
#define M_PI 3.141592653
GLFWwindow* window;
const int WIDTH=640;
const int HEIGHT=480;
const float M2P=60;
const float P2M=1/M2P;
b2World* world;
b2Body* body;
b2Vec2 points[4];
bool dyn=true;

GLuint textureID;
GLuint loadTextures(const char* filename) {
	unsigned char* image;
	int width, height;
	//generating texture object and attaching to the target
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//wrapping along the s and t axis of texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//filtering so thet the resolution fits to the object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load the img data
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//free the image memory from GPL
	SOIL_free_image_data(image);
	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

static void mouse_callback(GLFWwindow* window,int button,int action,int mods);
b2Body* addRect(int x,int y,int w,int h,bool dyns=true)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(x*P2M,y*P2M);
	if(dyns){
		dyn=false;
		bodyDef.type=b2_staticBody;
		textureID=NULL;		
	}
	else{
		dyn=true;		
		bodyDef.type=b2_dynamicBody;
	}
	body=world->CreateBody(&bodyDef);
	b2PolygonShape shape;
	shape.SetAsBox(w/2*P2M,h/2*P2M);
	b2FixtureDef fixtureDef;
	fixtureDef.shape=&shape;
	fixtureDef.density=1.0;
	body->CreateFixture(&fixtureDef);
	return body;
}
b2Body* myRect;
void drawSquare(b2Vec2 points[],b2Vec2 center,float angle)
{
	glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(center.x*M2P,center.y*M2P,0);
	glRotatef(angle*180.0/M_PI,0,0,1);	
	if(dyn){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textureID);
	glBegin(GL_QUADS);	
glTexCoord2f(250/256,250/256);glVertex2f(points[0].x*M2P,points[0].y*M2P);
glTexCoord2f(256/256,250/256);glVertex2f(points[1].x*M2P,points[1].y*M2P);
glTexCoord2f(256/256,256/256);glVertex2f(points[2].x*M2P,points[2].y*M2P);
glTexCoord2f(250/256,256/256);glVertex2f(points[3].x*M2P,points[3].y*M2P);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	}else{
		//textureID=NULL;
		glBegin(GL_QUADS);
		for(int i=0;i<4;i++)
				glVertex2f(points[i].x*M2P,points[i].y*M2P);
		glEnd();
		//dyn=true;
	}
	glPopMatrix();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	b2Body* tmp=world->GetBodyList();
	
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,textureID);

	while(tmp){
		for(int i=0;i<4;i++)
points[i]=((b2PolygonShape *)tmp->GetFixtureList()->GetShape())->GetVertex(i);
		drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());	
		if(body->GetType()==0)dyn=false;else dyn=true;
		tmp=tmp->GetNext();
	}
	
}
void init()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(0,WIDTH,HEIGHT,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	world=new b2World(b2Vec2(0.0,9.8));
	addRect(WIDTH/2,HEIGHT-50,WIDTH,30,true);
	myRect=addRect(150,300,50,50,false);
}
int main()
{
	glfwInit();
	window=glfwCreateWindow(WIDTH,HEIGHT,"Hello world",NULL,NULL);
	glfwMakeContextCurrent(window);
	glViewport(0,0,WIDTH,HEIGHT);
	glfwSetMouseButtonCallback(window,mouse_callback);
	init();
	textureID=loadTextures("./images/1.jpg");
	do{
		display();
		world->Step(1.0/30.0,8,3);
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSwapInterval(5);		
	}while(!glfwWindowShouldClose(window));
	glfwTerminate();
	return 0;
}

static void mouse_callback(GLFWwindow* window,int button,int action,int mods)
{	
	if(button==GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS)
	{		
		double x;
		double y;
		glfwGetCursorPos(window,&x,&y);
		dyn=true;
		addRect(x,y,50,50,false);
	}
}
