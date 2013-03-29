#include "TreemapPlot.h"
#include <algorithm>

TreemapPlot::TreemapPlot() : Plot()
{
	std::cout << "TreemapPlot::TreemapPlot()" << std::endl;

	primitive = new gl4::VBO();
	primitive->setProportions(1.0,1.0);
	primitive->init();

	groupIndex = 0;
	groups = 1;
	sizeIndex = 0;
	year = 0;
}

TreemapPlot::TreemapPlot(glm::vec2 in_x, glm::vec2 in_y) : Plot(in_x, in_y)
{
	std::cout << "TreemapPlot::~TreemapPlot(glm::vec2 in_x, glm::vec2 in_y)" << std::endl;

	primitive = new gl4::VBO();
	primitive->setProportions(1.0,1.0);
	primitive->init();

	groupIndex = 0;
	groups = 1;
	sizeIndex = 0;
	year = 0;

}

TreemapPlot::~TreemapPlot()
{
	std::cout << "TreemapPlot::~TreemapPlot()" << std::endl;
	delete primitive;
}

void TreemapPlot::setGroupIndex(int index, int g)
{
	groupIndex = index;
	groups = g;
	invalidate();
}

void TreemapPlot::setSizeIndex(int index)
{
	sizeIndex = index;
	invalidate();
}

void TreemapPlot::setYear(int y)
{
	year = y;
	invalidate();
}


/*
struct TreemapData {
    int DataCubeEntry;

    float sizeValue;
    float groupValue;
    float colorValue;
};
*/
bool compareTreemapData(const TreemapData &a, const TreemapData &b)
{
	return a.sizeValue > b.sizeValue;
}

void TreemapPlot::renderPlot()
{
	std::cout << "TreemapPlot::renderPlot()" << std::endl;
	// settings
	const int * datacount = dc->getDataCount();
	const float padding_x_left = 10.0;
	const float padding_y_bottom = 10.0;
	const float padding_x_right = 10.0;
	const float padding_y_top = 10.0;

	// start stage 2
	const float container_width = x[1] - x[0]; 
	const float container_height = y[1] - y[0]; 
	const float plot_width = container_width - padding_x_left - padding_x_right; 
	const float plot_height = container_height - padding_y_bottom - padding_y_top;

	const glm::vec2 scale = glm::vec2(1.0f/container_width, 1.0f / container_height);
	const glm::vec2 groupScale = dc->getAttribRange(groupIndex);

	
	Node *root = new Node();
	root->root = true;

	Node *groupNodes[groups];
	for (int i = 0; i < groups; ++i)
	{
		groupNodes[i] = new Node();
	}
	
	for (int i = 0; i < datacount[0]; ++i)
	{
		if (dc->isSetAll(i, year))
		{

			float sizeValue = dc->getItem(i, year, sizeIndex);
			float groupValue = dc->getItem(i, year, groupIndex);
			float colorValue = 0.0f;

			int group = (int)((groupValue - groupScale[0]) / (groupScale[1] - groupScale[0]) * (float) groups); 
			if (group >= groups)
			{
				group--;
			}
			if (colormap != 0)
			{
				colorValue = dc->getItem(i, year, colormap->getIndex());
			}
			//std::cout << "TreemapPlot::group = " <<  group <<  std::endl;

			Node *n = new Node(sizeValue, colorValue);
			groupNodes[group]->addNode(n);
		}
	}
	
	for (int i = 0; i < groups; ++i)
	{
		root->addNode(groupNodes[i]);
	}


	gl4::ShaderManager::getInstance()->bindShader("uniform_color");
	GLuint program = gl4::ShaderManager::getInstance()->getShaderProgram("uniform_color");
	int colorLoc = glGetUniformLocation(program, "uniform_color");
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);

	renderNode(root, glm::vec2(padding_x_left*scale[0], plot_width*scale[0]), glm::vec2(padding_y_bottom*scale[1], plot_height*scale[1]),colorLoc, scale);

	delete root;
}

void TreemapPlot::renderNode(Node *n, glm::vec2 size_x, glm::vec2 size_y,int colorLoc, const glm::vec2 &scale)
{
	if (n->isLeaf())
	{
		glm::vec3 color = glm::vec3(0,0,0);
		if (colormap != 0)
		{
			color = colormap->map(n->colorValue);
		}

		glm::mat4 transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(size_x[0], size_y[0],0));
		transform = glm::scale(transform, glm::vec3(size_x[1],size_y[1], 0.0));


		glUniform3fv(colorLoc, 1, glm::value_ptr(color));
		glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

		primitive->render();
	} else {
		if(n->isContainer()) {


			glm::vec3 color = glm::vec3(0,0,0);
			glm::mat4 transform = glm::mat4();
			const float border_width = 4.0f;

			//bottom
			transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(size_x[0], size_y[0],0));
			transform = glm::scale(transform, glm::vec3(size_x[1],scale[1]*border_width, 0.0));
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

			primitive->render();

			//top
			transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(size_x[0], size_y[0]+size_y[1],0));
			transform = glm::scale(transform, glm::vec3(size_x[1],scale[1]*border_width, 0.0));
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

			primitive->render();

			//left
			transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(size_x[0], size_y[0],0));
			transform = glm::scale(transform, glm::vec3(scale[0]*border_width,size_y[1], 0.0));
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

			primitive->render();

			//right
			transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(size_x[0]+size_x[1], size_y[0],0));
			transform = glm::scale(transform, glm::vec3(scale[0]*border_width,size_y[1], 0.0));
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

			primitive->render();


		}
		if (n->left != 0 && n->right != 0)
		{
			float scaleleft = n->left->sizeValue / (n->left->sizeValue + n->right->sizeValue);
			float scaleright = 1.0f - scaleleft;
			if (size_x[1]  > size_y[1] )
			{
				renderNode(n->left, glm::vec2(size_x[0], size_x[1]*scaleleft),size_y, colorLoc,scale);
				renderNode(n->right, glm::vec2(size_x[0] + size_x[1]*scaleleft, size_x[1]*scaleright),size_y, colorLoc,scale);
			} else {
				renderNode(n->left, size_x,glm::vec2(size_y[0], size_y[1]*scaleleft), colorLoc,scale);
				renderNode(n->right, size_x,glm::vec2(size_y[0]+size_y[1]*scaleleft, size_y[1]*scaleright), colorLoc,scale);
			}
		} else {
			if (n->left != 0)
			{
				renderNode(n->left, size_x,size_y, colorLoc,scale);
			} else if (n->right != 0){
				renderNode(n->right, size_x,size_y, colorLoc,scale);
			}
		}
	}
}