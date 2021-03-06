#include "ScatterPlot.h"
#include <iomanip>
#include <sstream>

ScatterPlot::ScatterPlot() : Plot()
{
	std::cout << "ScatterPlot::ScatterPlot()" << std::endl;
	primitive = new gl4::Circle(1.0,100);
	primitive->init();
	obj = new gl4::VBO();
	obj->init();
	year = 0;
	x_index = 0;
	y_index = 0;
	sizeIndex = 0;
}

ScatterPlot::ScatterPlot(glm::vec2 in_x, glm::vec2 in_y) : Plot(in_x, in_y)
{
	std::cout << "ScatterPlot::ScatterPlot()" << std::endl;
	primitive = new gl4::Circle(1.0,100);
	primitive->init();
	year = 0;
	x_index = 0;
	y_index = 0;
	sizeIndex = 0;
}
ScatterPlot::~ScatterPlot()
{
	std::cout << "ScatterPlot::~ScatterPlot()" << std::endl;
	delete primitive;
}

void ScatterPlot::setYear(int y) 
{
	year = y;
	invalidate();
}

void ScatterPlot::setSizeIndex(int sa)
{
	sizeIndex = sa;
	invalidate();
}

void ScatterPlot::setAxisIndex(int axis, int index)
{
	if (axis == AXIS_X)
	{
		x_index = index;
	} else if (axis == AXIS_Y)
	{
		y_index = index;
	}
	invalidate();
}

void ScatterPlot::renderPlot()
{
	std::cout << "ScatterPlot::renderPlot()" << std::endl;

	FontManager * fmgr = FontManager::getInstance();

	// settings
	const int * datacount = dc->getDataCount();
	const float padding_x_left = 60.0;
	const float padding_y_bottom = 40.0;
	const float padding_x_right = 10.0;
	const float padding_y_top = 10.0;
	const float PI = 3.14159265;

	// start stage 2
	const float container_width = x[1] - x[0]; 
	const float container_height = y[1] - y[0]; 
	const float scatterplot_width = container_width - padding_x_left - padding_x_right; 
	const float scatterplot_height = container_height - padding_y_bottom - padding_y_top;

	const glm::vec2 scale = glm::vec2(1.0f/container_width, 1.0f / container_height);

	gl4::ShaderManager::getInstance()->bindShader("uniform_color");
	GLuint program = gl4::ShaderManager::getInstance()->getShaderProgram("uniform_color");
	int colorLoc = glGetUniformLocation(program, "uniform_color");

	glm::vec4 color = glm::vec4(1.0,1.0,1.0,1.0);
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

	glm::vec2 rangex = dc->getAttribRange(x_index);
	glm::vec2 rangey = dc->getAttribRange(y_index);
	glm::vec2 sizeRange = dc->getAttribRange(sizeIndex);

	for (int i = 0; i < datacount[0]; ++i)
	{
		if (dc->isSetAll(i, year))
		{
			float x = dc->getItem(i, year, x_index);
			float y = dc->getItem(i, year, y_index);
			float sizeval = dc->getItem(i, year, sizeIndex);

			float posx = (x - rangex[0]) / (rangex[1] - rangex[0]) * scatterplot_width / container_width; // 0 -> 1
			float posy = (y - rangey[0]) / (rangey[1] - rangey[0]) * scatterplot_height / container_height; // 0 -> 1
			glm::vec3 position = glm::vec3(posx,posy,0);

			float area = 1.0f + (sizeval - sizeRange[0]) / (sizeRange[1] - sizeRange[0])*10;

			if (colormap != 0)
			{
				float colorval = dc->getItem(i, year, colormap->getIndex());
				color = glm::vec4(colormap->map(colorval),1.0f);
			}
			/*
			std::cout << "xy = (" << x << ", " << y << ")" << std::endl;
			std::cout << "pos = (" << posx << ", " << posy << ")" << std::endl;
			*/

			glUniform4fv(colorLoc, 1, glm::value_ptr(color));

			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, position);
			transform = glm::translate(transform, glm::vec3(padding_x_left*scale[0], padding_y_bottom*scale[1],0));
			transform = glm::scale(transform, glm::vec3(area));
			transform = glm::scale(transform, glm::vec3(scale, 1.0));

			glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
			glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

			primitive->render();
		}


	}

	float desiredXdist = 40.0;
	float desiredYdist = 40.0;

	int numXminor = int(container_height / desiredXdist);
	int numYminor = int(container_width / desiredYdist);

	float stepXpos = (container_width / (float)numXminor) / container_width;
	float stepXvalue = (rangex[1] - rangex[0]) / (float)numYminor;

	float stepYpos = (container_height / (float)numYminor) / container_height;
	float stepYvalue = (rangey[1] - rangey[0]) / (float)numXminor;

	// Draw AXIS

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	color = glm::vec4(0.0,0.0,0.0,1.0);
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

	glm::mat4 transform;;

	// X-AXIS MAJOR

	transform = glm::translate(glm::mat4(), glm::vec3(padding_x_left*0.9f*scale[0], padding_y_bottom*scale[1],0));
	transform = glm::scale(transform, glm::vec3(container_width,1, 0.0));
	transform = glm::scale(transform, glm::vec3(scale, 0.0));

	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	obj->render();

	// Y-AXIS MAJOR

	transform = glm::translate(glm::mat4(), glm::vec3(padding_x_left*scale[0], padding_y_bottom*0.9f*scale[1],0));
	transform = glm::scale(transform, glm::vec3(1,container_height, 0.0));
	transform = glm::scale(transform, glm::vec3(scale, 0.0));

	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	obj->render();

	color = glm::vec4(0.0,0.0,0.0,0.8);
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

	// X-AXIS MINORS
	for(int i=-numXminor+1; i<numXminor; ++i)
	{
		float ycoord = padding_y_bottom*scale[1] + stepXpos*i;

		if(ycoord < padding_y_bottom*scale[1])
			continue;

		if(ycoord > 1.0)
			break;

		transform = glm::translate(glm::mat4(), glm::vec3(padding_x_left*0.9f*scale[0], padding_y_bottom*scale[1] + stepXpos*i,0));
		transform = glm::scale(transform, glm::vec3(container_width,0.2, 0.0));
		transform = glm::scale(transform, glm::vec3(scale, 0.0));

		glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

		obj->render();

		if (ycoord> padding_y_bottom *scale[1])
		{
			std::ostringstream buff;
			buff<< std::setprecision(1) << std::fixed << rangey[0]+stepYvalue*i;
			fmgr->addText(	padding_x_left- 30.0f,ycoord*container_height,buff.str().c_str(),14,glm::vec4(0.0,0.0,0.0,1.0));
		}
	}

	// Y-AXIS MINORS
	for(int i=-numYminor+1; i<numYminor; ++i)
	{
		float xcoord = padding_x_left*scale[0] + stepYpos*i;

		if(xcoord < padding_x_left*scale[0])
			continue;

		if(xcoord > 1.0)
			break;

		transform = glm::translate(glm::mat4(), glm::vec3(padding_x_left*scale[0] + stepYpos*i, padding_y_bottom*0.9f*scale[1],0));
		transform = glm::scale(transform, glm::vec3(0.2,container_height, 0.0));
		transform = glm::scale(transform, glm::vec3(scale, 0.0));

		glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

		obj->render();
		if (xcoord>= padding_x_left *scale[0])
		{
			std::ostringstream buff;
			buff<< std::setprecision(1) << std::fixed << rangex[0]+stepXvalue*i;
			fmgr->addText(	xcoord*container_width,padding_y_bottom,buff.str().c_str(),14,glm::vec4(0.0,0.0,0.0,1.0));
		}
	}


	glDisable(GL_BLEND);

	// color and size
	std::string sizeAttrib = "Size: "; 
	sizeAttrib += dc->getAttribName(sizeIndex);
	sizeAttrib += ", Color: ";
	sizeAttrib += dc->getAttribName(colormap->getIndex());
	fmgr->addText(	5.0f,20.0f,sizeAttrib.c_str(),16,glm::vec4(0.0,0.0,0.0,1.0));

	// y
	std::string yAttrib = ""; 
	yAttrib += dc->getAttribName(y_index);
	fmgr->addText(	5.0f,container_height,yAttrib.c_str(),16,glm::vec4(0.0,0.0,0.0,1.0));


	// x
	std::string xAttrib = ""; 
	xAttrib += dc->getAttribName(x_index);
	fmgr->addText(	container_width-60.0f,padding_y_bottom-20.0f,xAttrib.c_str(),16,glm::vec4(0.0,0.0,0.0,1.0));

	fmgr->render();
	fmgr->clearText();
}