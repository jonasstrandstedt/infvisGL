/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Engine.h"
#include "Sphere.h"

#include "infvis/DataCube.h"

gl4::VBO *obj;
gl4::Sphere *sphere;
gl4::Engine *engine;
glm::vec2 angle;
bool wireframe = false;

int tess;

void keyboardCallback(int key, int state);
void myRenderFunc(void);
//void myDeferredRenderFunc(void);
void myInitFunc(void);
void myUpdateFunc(float dt);

int main(int argc, char **argv) {

	// create engine object
	engine = new gl4::Engine(argc, argv);
	
	// set callbacks
	engine->setKeyBoardCallbackfunc(keyboardCallback);
	engine->setUpdateFunc(myUpdateFunc);
	engine->setRenderFunc(myRenderFunc);
	//engine->setDeferredRenderFunc(myDeferredRenderFunc);
	engine->setInitFunc(myInitFunc);
	
	// init
	if( ! engine->initGL()) {
		return 1;
	}

	// do render
	engine->render();

	// cleanup
	delete sphere;
	delete obj;
	delete engine;

	// return success
	return 0;
}

void keyboardCallback(int key, int state) 
{
	// increase and decrease tessellation levels
	if(key == 'I' && state == GLFW_PRESS) {
		if(tess < 64)
			tess++;
	}
	if(key == 'O' && state == GLFW_PRESS) {
		if(tess > 1)
			tess--;
	}

	// toggle wireframe
	if(key == 'W' && state == GLFW_PRESS) {
		wireframe = ! wireframe;
	}
}


void myInitFunc(void) 
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);

	angle[0] = 0.0f;
	angle[1] = 0.0f;
	obj = new gl4::VBO();
	obj->init();

	sphere = new gl4::Sphere(1.0, 30);
	sphere->init();

	tess = 1;

	// load textures
	gl4::TextureManager::getInstance()->loadTexture("earth_diffuse", "data/earth_nasa_lowres.tga");

	gl4::Shader *s1 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_textur.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("textur", s1);
	gl4::Shader *s2 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_color.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("color", s2);

	DataCube dc(2,4,6);

	dc.SetItem(0, 1, 4, 32.0f);

	for(int x = 0; x < 2; ++x)
	{
		for(int y = 0; y < 4; ++y)
		{
			for(int z = 0; z < 6; ++z)
			{
				printf(" %f ",dc.GetItem(x, y, z));
			}
			printf("\n");
		}

		printf("\n ---------- \n\n");
	}
}

void myRenderFunc(void) 
{

	gl4::ShaderManager::getInstance()->bindShader("color");

	glm::mat4 plane_transform = glm::scale(glm::mat4(1.0), glm::vec3(5));
	plane_transform = glm::translate(plane_transform,glm::vec3(-0.5,-0.3, 0.0));
	plane_transform = glm::rotate(plane_transform,-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	engine->usePerspectiveProjection(plane_transform);
	obj->render();

	
	gl4::ShaderManager::getInstance()->bindShader("textur");
	gl4::TextureManager::getInstance()->bindTexture("earth_diffuse");
	glm::mat4 transform = glm::rotate(glm::mat4(1.0f),angle[1], glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform,angle[0], glm::vec3(1.0f, 0.0f, 0.0f));
	engine->usePerspectiveProjection(transform);
	sphere->render();

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