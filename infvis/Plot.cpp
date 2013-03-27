#include "Plot.h"

Plot::Plot() : Container(glm::vec2(1,1), glm::vec2(1,1)) 
{
	std::cout << "Plot::Plot()" << std::endl;
	dc = 0;
	colormap = 0;

	_fbo = 0;
	_quad = 0;

	backgroundColor = glm::vec4(1,1,1,1);
	valid = false;
	renders = 0;
}

Plot::Plot(glm::vec2 in_x, glm::vec2 in_y) : Container(in_x, in_y) 
{
	std::cout << "Plot::Plot(glm::vec2 in_x, glm::vec2 in_y)" << std::endl;
	dc = 0;
	colormap = 0;

	backgroundColor = glm::vec4(1,1,1,1);

	_fbo = new gl4::FBO();
	_quad = new gl4::VBO();

	int _windowWidth = x[1] - x[0];
	int _windowHeight = y[1] - y[0];

	_fbo->init(_windowWidth, _windowHeight, 32, 1);
	_quad->setProportions(1,1);
	_quad->init();
	valid = false;
	renders = 0;
}
Plot::~Plot()
{
	if (_fbo)
	{
		delete _fbo;
	}
	if (_quad)
	{
		delete _quad;
	}
}

void Plot::resize(glm::vec2 in_x, glm::vec2 in_y)
{
	std::cout << "Plot::resize(glm::vec2 in_x, glm::vec2 in_y)" << std::endl;
	setSize(in_x, in_y);

	if (_fbo)
	{
		delete _fbo;
	}
	if (_quad)
	{
		delete _quad;
	}

	_fbo = new gl4::FBO();
	_quad = new gl4::VBO();

	int _windowWidth = x[1] - x[0];
	int _windowHeight = y[1] - y[0];
	
	_fbo->init(_windowWidth, _windowHeight, 32, 3);
	_quad->setProportions(1,1);
	_quad->init();

	invalidate();
}


void Plot::setColorMap(ColorMap *c)
{
	colormap = c;
	invalidate();
}

void Plot::setInput(DataCube *d) {
	dc = d;
	invalidate();
}

void Plot::setBackgroundColor(glm::vec4 bc)
{
	backgroundColor = bc;
	invalidate();
}


void Plot::render()
{
	//std::cout << "Plot::render()" << std::endl;
	if (! isValid() || renders < 2)
	{
		_fbo->clear(backgroundColor);
		_fbo->bind();

		renderPlot();

		_fbo->unbind();

		setValid();
	} 

	glViewport(x[0], y[0], x[1] - x[0], y[1] - y[0]);
	// start stage 2
	gl4::ShaderManager::getInstance()->bindShader("textur");

	// orthogonal projection
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
	glm::mat4 transform = glm::mat4(1.0);
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	// render the FBO texture
	_fbo->bindTextures();
	_quad->render();

	// unbind texture and shader
	glBindTexture(GL_TEXTURE_2D,0);
	gl4::ShaderManager::getInstance()->unbindShader();

	++renders;
}


void Plot::invalidate()
{
	valid = false;
}

void Plot::setValid()
{
	valid = true;
}


bool Plot::isValid()
{
	return valid;
}

/*
std::cout << "Plot::render()" << std::endl;
glViewport(	x[0],y[0],x[1] - x[0], y[1] - y[0]);

gl4::ShaderManager::getInstance()->bindShader("uniform_color");
int colorLoc = glGetUniformLocation(gl4::ShaderManager::getInstance()->getShaderProgram("uniform_color"), "uniform_color");

glUniform3fv(colorLoc, 1, glm::value_ptr(backgroundColor));

glm::mat4 t = glm::translate(glm::mat4(), glm::vec3(0,0,-10));
glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &t[0][0]);

obj->render();
*/

