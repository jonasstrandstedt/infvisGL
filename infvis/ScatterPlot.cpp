#include "ScatterPlot.h"


ScatterPlot::ScatterPlot(glm::vec2 in_x, glm::vec2 in_y) : Plot(in_x, in_y)
{
	std::cout << "ScatterPlot::ScatterPlot()" << std::endl;
	primitive = new gl4::Circle(1.0,100);
	primitive->init();
	year = 0;
	colormap = 0;
}
ScatterPlot::~ScatterPlot()
{
	std::cout << "ScatterPlot::~ScatterPlot()" << std::endl;
	delete primitive;
}

void ScatterPlot::setYear(int y) 
{
	year = y;
}
void ScatterPlot::render()
{
	glViewport(	x[0],y[0],x[1] - x[0], y[1] - y[0]);

	float scalex = x[1] - x[0];
	float scaley = y[1] - y[0];
	if (scalex / scaley > 1.0)
	{
		scaley = scalex / scaley;
		scalex = 1.0;
	}
	

	gl4::ShaderManager::getInstance()->bindShader("uniform_color");
	GLuint program = gl4::ShaderManager::getInstance()->getShaderProgram("uniform_color");
	int colorLoc = glGetUniformLocation(program, "uniform_color");

	float size = 200.0;
	glm::vec3 color = glm::vec3(1.0,1.0,1.0);
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	size = 1.0;

	glm::vec2 rangex = dc->getAttribRange(0);
	glm::vec2 rangey = dc->getAttribRange(1);

	const int * datacount = dc->getDataCount();


	for (int i = 0; i < datacount[0]; ++i)
	{
		
		float x = dc->getItem(i, year, 0);
		float y = dc->getItem(i, year, 1);

		float posx = (x - rangex[0]) / (rangex[1] - rangex[0]);
		float posy = (y - rangey[0]) / (rangey[1] - rangey[0]);
		size = (y - rangey[0]) / (rangey[1] - rangey[0])* 0.2;

		if (colormap != 0)
		{
			color = colormap->map(x);
		}
		//std::cout << "xy = (" << x << ", " << y << ")" << std::endl;
		//std::cout << "pos = (" << posx << ", " << posy << ")" << std::endl;

		glm::vec3 position = glm::vec3(posx,posy,0);
		
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));

		glm::mat4 transform = glm::translate(glm::mat4(), position);
		transform = glm::scale(transform, glm::vec3(size));
		transform = glm::scale(transform, glm::vec3(scalex, scaley, 0.0));
		transform = glm::translate(transform, glm::vec3(-0.5, -0.5, 0.0));

		glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
		glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

		primitive->render();

	}

}