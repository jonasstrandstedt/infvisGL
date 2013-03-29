#pragma once

#include "Plot.h"

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct TreemapData {

	int DataCubeEntry;

	int group;

	float sizeValue;
	float groupValue;
	float colorValue;
};


class Node {

public:
	Node () {
		left =0;
		right = 0;
		
		sizeValue = 0;
		colorValue = 0;
		root = false;
		container = true;
	};
	Node(float size, float color) {

		//std::cout << "TreemapPlot::Creating node" << std::endl;
		left = 0;
		right = 0;
	
		sizeValue = size;
		colorValue = color;
		root = false;
		container = false;
	}

	~Node()
	{
		if (left != 0)
		{
			delete left;
		}
		if (right != 0)
		{
			delete right;
		}
	};

	void addNode(Node *n)
	{
		sizeValue += n->sizeValue;
		if (isLeaf())
		{
			left = new Node(sizeValue, colorValue);
			right = n;
		} else {
			if (left == 0)
			{
				//std::cout << "TreemapPlot::Left node free, adding" << std::endl;
				left = n;
			} else if (right == 0) {
				//std::cout << "TreemapPlot::Right node free, adding" << std::endl;
				right = n;
			} else {
				
				if (n->sizeValue + left->sizeValue < right->sizeValue)
				{
					if (left->container)
					{
						Node *temp = left;
						left = new Node();
						left->root = true;
						left->addNode(temp);
						left->addNode(n);
					} else {
						left->addNode(n);
					}
				} else {
					if (right->container)
					{
						Node *temp = right;
						right = new Node();
						right->root = true;
						right->addNode(temp);
						right->addNode(n);
					} else {
						right->addNode(n);
					}
				}
				
			}
			
		}
	};

	bool isLeaf() {
		return left == 0 && right == 0 && !root && !container;
	};

	bool isContainer() {
		return !root && container;
	}



	Node* left;
	Node* right;

	float sizeValue;
	float groupValue;
	float colorValue;

	bool root;
	bool container;
};


class TreemapPlot: public Plot
{
public:
	TreemapPlot();
	TreemapPlot(glm::vec2 in_x, glm::vec2 in_y);
	~TreemapPlot();

	void renderPlot();

	void setGroupIndex(int index, int g = 7);
	void setSizeIndex(int index);
	void setYear(int y);

protected:
private:
	gl4::VBO *primitive;

	int groups;
	int sizeIndex;
	int groupIndex;
	int year;


	void renderNode(Node *n, glm::vec2 size_x, glm::vec2 size_y,int colorLoc, const glm::vec2 &scale);
};