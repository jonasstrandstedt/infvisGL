#include "ScatterPlot.h"

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

	// settings
	const int * datacount = dc->getDataCount();
	const float padding_x_left = 70.0;
	const float padding_y_bottom = 70.0;
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

	glm::vec3 color = glm::vec3(1.0,1.0,1.0);
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

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
				color = colormap->map(colorval);
			}
			/*
			std::cout << "xy = (" << x << ", " << y << ")" << std::endl;
			std::cout << "pos = (" << posx << ", " << posy << ")" << std::endl;
			*/

			glUniform3fv(colorLoc, 1, glm::value_ptr(color));

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


	glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(padding_x_left*scale[0], padding_y_bottom*scale[1],0));
	transform = glm::scale(transform, glm::vec3(scatterplot_width,1, 0.0));
	transform = glm::scale(transform, glm::vec3(scale, 0.0));

	gl4::ShaderManager::getInstance()->bindShader("color");

	// orthogonal projection
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	obj->render();

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(padding_x_left*scale[0], padding_y_bottom*scale[1],0));
	transform = glm::scale(transform, glm::vec3(1,scatterplot_height, 0.0));
	transform = glm::scale(transform, glm::vec3(scale, 0.0));

	gl4::ShaderManager::getInstance()->bindShader("color");

		// orthogonal projection
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	obj->render();


}