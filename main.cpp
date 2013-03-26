/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Engine.h"
#include "infvis/SplitContainer.h"
#include "infvis/ScatterPlot.h"

#include "infvis/DataCube.h"
#include "infvis/DataLoader.h"

DataCube * dc;
DataLoader * dl;

gl4::Engine *engine;
glm::vec2 angle;
int year = 40;
SplitContainer *sc;
ScatterPlot *sp;


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
	delete sc;
	delete sp;
	delete engine;
	delete dc;

	// return success
	return 0;
}

void keyboardCallback(int key, int state) 
{

	const int * datacount = dc->GetDataCount();
	// increase and decrease tessellation levels
	if(key == GLFW_KEY_RIGHT && state == GLFW_PRESS) {
		if(year < datacount[1])
			year++;
	}
	if(key == GLFW_KEY_LEFT && state == GLFW_PRESS) {
		if(year > 0)
			year--;
	}

	sp->setYear(year);

	std::cout << "Rendering year " << year << "/" << datacount[1] << std::endl;

}


void myInitFunc(void) 
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);

	// load textures
	gl4::TextureManager::getInstance()->loadTexture("earth_diffuse", "data/earth_nasa_lowres.tga");

	gl4::Shader *s1 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_textur.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("textur", s1);

	gl4::Shader *s2 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_color.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("color", s2);

	gl4::Shader *s3 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_uniform_color.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("uniform_color", s3);

	dl = new DataLoader();
	dl->addAttribFromFile("fertility", "data/fertility_rate.csv");
	dl->addAttribFromFile("population", "data/total_population.csv");
	dl->addAttribFromFile("broadband", "data/fixed_broadband_connections.csv");
	dc = dl->getDataCube();
	dc->CalculateAttribRanges();
	
	glm::vec2 rangex = dc->GetAttribRange(0);
	glm::vec2 rangey = dc->GetAttribRange(1);
	glm::vec2 rangez = dc->GetAttribRange(2);

	std::cout << "dc->GetAttribRange(0) = " << rangex[0] << " -> " << rangex[1] << std::endl;
	std::cout << "dc->GetAttribRange(1) = " << rangey[0] << " -> " << rangey[1] << std::endl;
	std::cout << "dc->GetAttribRange(2) = " << rangez[0] << " -> " << rangez[1] << std::endl;

	sc = new SplitContainer(glm::vec2(0,WINDOW_WIDTH),glm::vec2(0, WINDOW_HEIGHT));
	sp = new ScatterPlot(glm::vec2(1,1), glm::vec2(1,1));
	sp->setInput(dc);
	sp->setYear(year);
	sc->setTopChild(sp);
	sc->setBottomChild(sp);

}

void myRenderFunc(void) 
{
	sc->render();
	
}


void myUpdateFunc(float dt)
{

}