/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Engine.h"
#include "Circle.h"

#include "infvis/DataCube.h"
#include "infvis/DataLoader.h"

DataCube * dc;
DataLoader * dl;

gl4::Circle *obj;
gl4::Engine *engine;
glm::vec2 angle;
bool wireframe = false;
int year = 40;

int tess;

void keyboardCallback(int key, int state);
void myRenderFunc(void);
//void myDeferredRenderFunc(void);
void myInitFunc(void);
void myUpdateFunc(float dt);

void scatterPlot(void);

int main(int argc, char **argv) {

	// create engine object
	engine = new gl4::Engine(argc, argv);
	
	// set callbacks
	engine->setKeyBoardCallbackfunc(keyboardCallback);
	engine->setUpdateFunc(myUpdateFunc);
	engine->setRenderFunc(scatterPlot);
	//engine->setDeferredRenderFunc(myDeferredRenderFunc);
	engine->setInitFunc(myInitFunc);
	
	// init
	if( ! engine->initGL()) {
		return 1;
	}

	// do render
	engine->render();

	// cleanup
	delete obj;
	delete engine;
	delete dc;

	// return success
	return 0;
}

void keyboardCallback(int key, int state) 
{

	const int * datacount = dc->getDataCount();
	// increase and decrease tessellation levels
	if(key == GLFW_KEY_RIGHT && state == GLFW_PRESS) {
		if(year < datacount[1])
			year++;
	}
	if(key == GLFW_KEY_LEFT && state == GLFW_PRESS) {
		if(year > 0)
			year--;
	}

	std::cout << "Rendering year " << year << "/" << datacount[1] << std::endl;
}


void myInitFunc(void) 
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);

	angle[0] = 0.0f;
	angle[1] = 0.0f;
	obj = new gl4::Circle(1.0,100);
	obj->init();

	tess = 1;

	// load textures
	gl4::TextureManager::getInstance()->loadTexture("earth_diffuse", "data/earth_nasa_lowres.tga");

	gl4::Shader *s1 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_textur.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("textur", s1);

	gl4::Shader *s2 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_color.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("color", s2);

	gl4::Shader *s3 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_uniform_color.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("uniform_color", s3);

/*
	const int size[3] = {2,4,4};

	dc = new DataCube(size[0], size[1], size[2]);

	dc->SetItem(0, 1, 3, 32.0f);
	dc->SetItem(1, 2, 3, -10.0f);

	dc->CalculateAttribRanges();

	printf("(Min/Max): \n");
	for(int z=0; z < size[2]; ++z)
	{
		printf("(%f,%f) ", dc->GetAttribRange(z).x, dc->GetAttribRange(z).y);
	}

	printf("\n\n");

	for(int x = 0; x < size[0]; ++x)
	{
		for(int y = 0; y < size[1]; ++y)
		{
			for(int z = 0; z < size[2]; ++z)
			{
				printf(" %f ",dc->GetItem(x, y, z));
			}
			printf("\n");
		}

		printf("\n ---------- \n\n");
	}
	dc->CalculateAttribRanges();
	
	delete dc;
*/
	dl = new DataLoader();
	dl->addAttribFromFile("fertility", "data/fertility_rate.csv");
	dl->addAttribFromFile("population", "data/total_population.csv");
	dl->addAttribFromFile("broadband", "data/fixed_broadband_connections.csv");
	dc = dl->getDataCube();
	dc->calculateAttribRanges();
	
	glm::vec2 rangex = dc->getAttribRange(0);
	glm::vec2 rangey = dc->getAttribRange(1);
	glm::vec2 rangez = dc->getAttribRange(2);

	std::cout << "dc->GetAttribRange(0) = " << rangex[0] << " -> " << rangex[1] << std::endl;
	std::cout << "dc->GetAttribRange(1) = " << rangey[0] << " -> " << rangey[1] << std::endl;
	std::cout << "dc->GetAttribRange(2) = " << rangez[0] << " -> " << rangez[1] << std::endl;

}

void myRenderFunc(void) 
{
	gl4::ShaderManager::getInstance()->bindShader("color");

	float size = 100.0;
	glm::vec3 position = glm::vec3(100,100,0);

	glm::mat4 transform = glm::translate(glm::mat4(), position);
	transform = glm::scale(transform, glm::vec3(size));
	transform = glm::translate(transform, glm::vec3(-0.5, -0.5, 0.0));
	
	//plane_transform = glm::translate(plane_transform,glm::vec3(-0.5,-0.3, 0.0));
	//plane_transform = glm::rotate(plane_transform,-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	//engine->usePerspectiveProjection(plane_transform);
	engine->useOrthogonalProjection(transform);

	obj->render();
	
	// gl4::ShaderManager::getInstance()->bindShader("textur");
	// gl4::TextureManager::getInstance()->bindTexture("earth_diffuse");
	// glm::mat4 transform = glm::rotate(glm::mat4(1.0f),angle[1], glm::vec3(0.0f, 1.0f, 0.0f));
	// transform = glm::rotate(transform,angle[0], glm::vec3(1.0f, 0.0f, 0.0f));
	// engine->usePerspectiveProjection(transform);
	// sphere->render();

}

void scatterPlot(void)
{
	gl4::ShaderManager::getInstance()->bindShader("uniform_color");
	GLuint program = gl4::ShaderManager::getInstance()->getShaderProgram("uniform_color");
	int colorLoc = glGetUniformLocation(program, "uniform_color");

	float size = 200.0;
	glm::vec3 color = glm::vec3(1.0,0.0,0.0);
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	/*
	glm::vec3 p = glm::vec3(200,200,0);


	glm::mat4 t = glm::translate(glm::mat4(), p);
	t = glm::scale(t, glm::vec3(size));

	engine->useOrthogonalProjection(t);


	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	obj->render();
	*/

	size = 1.0;


	glm::vec2 rangex = dc->getAttribRange(0);
	glm::vec2 rangey = dc->getAttribRange(1);

	const int * datacount = dc->getDataCount();
	for (int i = 0; i < datacount[0]; ++i)
	{
		float x = dc->getItem(i, year, 0);
		float y = dc->getItem(i, year, 1);

		float posx = (x - rangex[0]) / (rangex[1] - rangex[0]) * WINDOW_WIDTH;
		float posy = (y - rangey[0]) / (rangey[1] - rangey[0]) * WINDOW_HEIGHT;
		size = (y - rangey[0]) / (rangey[1] - rangey[0]) * 50.0;
		//std::cout << "xy = (" << x << ", " << y << ")" << std::endl;
		//std::cout << "pos = (" << posx << ", " << posy << ")" << std::endl;

		glm::vec3 position = glm::vec3(posx,posy,0);

		float val = (x - rangex[0]) / (rangex[1] - rangex[0]);

		color = glm::vec3(1.0,0.0,0.0)*(1.0f - val) + glm::vec3(0.0,0.0,1.0)*val;

		glUniform3fv(colorLoc, 1, glm::value_ptr(color));

		glm::mat4 transform = glm::translate(glm::mat4(), position);
		transform = glm::scale(transform, glm::vec3(size));
		transform = glm::translate(transform, glm::vec3(-0.5, -0.5, 0.0));

		engine->useOrthogonalProjection(transform);

		obj->render();
	}
}


void myUpdateFunc(float dt)
{

	float speed = 50.0f;
	if(engine->isKeyPressed(GLFW_KEY_RIGHT)) {
		angle[1] += dt*speed;
	}
	if(engine->isKeyPressed(GLFW_KEY_LEFT)) {
		angle[1] -= dt*speed;
	}
	if(engine->isKeyPressed(GLFW_KEY_UP)) {
		angle[0] += dt*speed;
	}
	if(engine->isKeyPressed(GLFW_KEY_DOWN)) {
		angle[0] -= dt*speed;
	}
}