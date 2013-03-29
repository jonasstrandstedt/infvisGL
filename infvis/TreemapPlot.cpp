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
	groups = 7;
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

	Node *root = new Node();
	for (int i = 0; i < datacount[0]; ++i)
	{
		if (dc->isSetAll(i, year))
		{

			TreemapData d;

			d.DataCubeEntry = i;
			d.sizeValue = dc->getItem(i, year, sizeIndex);
			d.groupValue = dc->getItem(i, year, groupIndex);
			d.colorValue = 0.0f;
			if (colormap != 0)
			{
				d.colorValue = dc->getItem(i, year, colormap->getIndex());
			}




			Node *n = new Node(d.sizeValue, d.colorValue);
			root->addNode(n);
		}
	}



	gl4::ShaderManager::getInstance()->bindShader("uniform_color");
	GLuint program = gl4::ShaderManager::getInstance()->getShaderProgram("uniform_color");
	int colorLoc = glGetUniformLocation(program, "uniform_color");

	float startpos = 0.0f;
	glm::vec3 color = glm::vec3(1,0,0);

	const float wscale = plot_width / container_width;

	renderNode(root, glm::vec2(padding_x_left*scale[0], plot_width*scale[0]), glm::vec2(padding_y_bottom*scale[1], plot_height*scale[1]),colorLoc);

}

void TreemapPlot::renderNode(Node *n, glm::vec2 size_x, glm::vec2 size_y,int colorLoc)
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

		// orthogonal projection
		glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
		glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

		primitive->render();
	} else {
		if (n->left != 0 && n->right != 0)
		{
			float scaleleft = n->left->sizeValue / (n->left->sizeValue + n->right->sizeValue);
			float scaleright = 1.0f - scaleleft;
			if (size_x[1]  > size_y[1] )
			{
				renderNode(n->left, glm::vec2(size_x[0], size_x[1]*scaleleft),size_y, colorLoc);
				renderNode(n->right, glm::vec2(size_x[0] + size_x[1]*scaleleft, size_x[1]*scaleright),size_y, colorLoc);
			} else {
				renderNode(n->left, size_x,glm::vec2(size_y[0], size_y[1]*scaleleft), colorLoc);
				renderNode(n->right, size_x,glm::vec2(size_y[0]+size_y[1]*scaleleft, size_y[1]*scaleright), colorLoc);
			}
		} else {
			if (n->left != 0)
			{
				renderNode(n->left, size_x,size_y, colorLoc);
			} else if (n->right != 0){
				renderNode(n->right, size_x,size_y, colorLoc);
			}
		}
	}
}