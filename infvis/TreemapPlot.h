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
		notRoot = false;
	};
	Node(float size, float color) {
		left = 0;
		right = 0;
	
		sizeValue = size;
		colorValue = color;
		notRoot = true;
	}

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
				left = n;
			} else if (right == 0) {
				right = n;
			} else {
				
				if (n->sizeValue + left->sizeValue < right->sizeValue)
				{
					left->addNode(n);
				} else {
					right->addNode(n);
				}
				
			}
			
		}
	};

	bool isLeaf() {
		return left == 0 && right == 0 && notRoot;
	};



	Node* left;
	Node* right;

	float sizeValue;
	float groupValue;
	float colorValue;

	bool notRoot;
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


	void renderNode(Node *n, glm::vec2 size_x, glm::vec2 size_y,int colorLoc);
};