/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Engine.h"

#include "infvis/SplitContainer.h"
#include "infvis/ScatterPlot.h"
#include "infvis/TreemapPlot.h"

#include "infvis/DataCube.h"
#include "infvis/DataLoader.h"
#include "infvis/ColorMap.h"

DataCube * dc;
DataLoader * dl;
ColorMap * cm;
ColorMap * tmcm;
ColorMap::HSVPart * cmp1;
ColorMap::LinearPart * cmp2;
ColorMap::LinearPart * cmp3;

gl4::Engine *engine;
glm::vec2 angle;
int year = 50;
SplitContainer *sc;
ScatterPlot *sp;
TreemapPlot *tp;

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
	delete cm;
	delete cmp1;
	delete cmp2;
	delete cmp3;

	// return success
	return 0;
}

void keyboardCallback(int key, int state) 
{

	const int * datacount = dc->getDataCount();
	// increase and decrease tessellation levels
	if(key == GLFW_KEY_RIGHT && state == GLFW_PRESS) {
		if(year < datacount[1]-1)
			year++;

		sp->setYear(year);
		tp->setYear(year);
		std::cout << "Rendering year " << year << "/" << datacount[1] << std::endl;
	}
	if(key == GLFW_KEY_LEFT && state == GLFW_PRESS) {
		if(year > 0)
			year--;

		sp->setYear(year);
		tp->setYear(year);
		std::cout << "Rendering year " << year << "/" << datacount[1] << std::endl;
	}
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

	gl4::Shader *s4 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_text.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("text", s4);

	gl4::Shader *s5 = new gl4::Shader( "data/shaders/vs.glsl","data/shaders/fs_black.glsl");
	gl4::ShaderManager::getInstance()->addShaderProgram("black", s5);

	dl = new DataLoader();
	dl->addAttribFromFile("fertility", "data/fertility_rate.csv");
	dl->addAttribFromFile("population", "data/total_population.csv");
	dl->addAttribFromFile("gdp", "data/gdp.csv");
	dl->addAttribFromFile("life", "data/life_excpectancy_female.csv");
	//dl->addAttribFromFile("broadband", "data/fixed_broadband_connections.csv");
	dc = dl->getDataCube();
	dc->calculateAttribRanges();
	dc->interpolateAndClamp();

	cm = new ColorMap();

	cmp1 = new ColorMap::HSVPart(glm::vec3(0.0,0.5,0.5), glm::vec3(360.0,0.5,0.5));
	cmp2 = new ColorMap::LinearPart(glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0));
	cmp3 = new ColorMap::LinearPart(glm::vec3(0.0,0.0,1.0), glm::vec3(1.0,0.0,0.0));

	cm->addPart(cmp1);
	//cm->addPart(cmp2);
	//cm->addPart(cmp3);
	cm->setDataCube(dc);
	cm->setIndex(0);


	tmcm = new ColorMap();
	tmcm->addPart(cmp1);
	tmcm->setDataCube(dc);
	tmcm->setIndex(2);
	
	glm::vec2 rangex = dc->getAttribRange(0);
	glm::vec2 rangey = dc->getAttribRange(1);
	glm::vec2 rangez = dc->getAttribRange(2);
	glm::vec2 rangew = dc->getAttribRange(3);

	std::cout << "dc->GetAttribRange(0) = " << rangex[0] << " -> " << rangex[1] << std::endl;
	std::cout << "dc->GetAttribRange(1) = " << rangey[0] << " -> " << rangey[1] << std::endl;
	std::cout << "dc->GetAttribRange(2) = " << rangez[0] << " -> " << rangez[1] << std::endl;
	std::cout << "dc->GetAttribRange(3) = " << rangew[0] << " -> " << rangew[1] << std::endl;

	sc = new SplitContainer(glm::vec2(0,WINDOW_WIDTH),glm::vec2(0, WINDOW_HEIGHT));
	
	sp = new ScatterPlot();
	sp->setInput(dc);
	sp->setYear(year);
	sp->setColorMap(cm);
	sp->setSizeIndex(1);
	sp->setAxisIndex(AXIS_X, 0);
	sp->setAxisIndex(AXIS_Y, 3);

	tp = new TreemapPlot();
	tp->setInput(dc);
	tp->setYear(year);
	tp->setSizeIndex(1);
	tp->setGroupIndex(0, 7);
	tp->setColorMap(tmcm);
	
	
	sc->addChild(tp);
	sc->addChild(sp);
}

void myRenderFunc(void) 
{
	sc->render();

	
	FontManager * fmgr = FontManager::getInstance();

	fmgr->printText(WINDOW_WIDTH / 2.0 - 30,WINDOW_HEIGHT / 2.0+15.0f,dc->getTimeName(year).c_str(),20.0, glm::vec4(0.0,0.0,0.0,1.0));
	
}

void myUpdateFunc(float dt)
{

}