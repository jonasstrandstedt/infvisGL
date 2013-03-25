/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Circle.h"

gl4::Circle::Circle(float radius, int segments)
{
	_mode = GL_TRIANGLES;

	// calculate and allocate memory for number of vertices and incicies
	_vsize = segments + 1;
	_isize = segments * 3;
	_varray = (Vertex*) std::malloc(_vsize*sizeof(Vertex));
	_iarray = (int*) std::malloc(_isize*sizeof(int));

	std::cout 	<< "Initializing circle with: " << std::endl
				<< "   segments = " << segments << std::endl
				<< "   radius = " << radius << std::endl
				<< "   _vsize = " << _vsize << std::endl
				<< "   _isize = " << _isize << std::endl;

	// define PI
	const float PI = 3.14159265;
	int nr = 0;

	// define center vertex
	_varray[nr].location[0] = 0.0f;
	_varray[nr].location[1] = 0.0f;
	_varray[nr].location[2] = 0.0f;
	_varray[nr].normal[0] = 1.0f;
	_varray[nr].normal[1] = 0.0f;
	_varray[nr].normal[2] = 0.0f;
	_varray[nr].tex[0] = 0.5f;
	_varray[nr].tex[1] = 1.0f;
	_varray[nr].color[0] = 1.0f;
	_varray[nr].color[1] = 0.0f;
	_varray[nr].color[2] = 0.0f;
	_varray[nr].color[3] = 1.0f;
	_varray[nr].attribute[0] = 0.0f;
	_varray[nr].attribute[1] = 0.0f;
	_varray[nr].attribute[2] = 0.0f;
	_varray[nr].float_attribute = 0.0f;

	nr++;
	for (int i = 0; i < segments; i++)
	{
		float theta = (static_cast<float>(i)*PI * 2.0)/static_cast<float>(segments);

		float x = radius*cos(theta);
		float y = radius*sin(theta);
		float z = 0.0;

		float t1 = static_cast<float>(i)/static_cast<float>(segments);
		float t2 = 1.0f-static_cast<float>(i)/static_cast<float>(segments);

		_varray[nr].location[0] = x;
		_varray[nr].location[1] = y;
		_varray[nr].location[2] = z;
		_varray[nr].normal[0] = 1.0;
		_varray[nr].normal[1] = 0.0;
		_varray[nr].normal[2] = 0.0;

		_varray[nr].tex[0] = t1;
		_varray[nr].tex[1] = t2;

		_varray[nr].color[0] = 1.0f;
		_varray[nr].color[1] = 0.0f;
		_varray[nr].color[2] = 0.0f;
		_varray[nr].color[3] = 1.0f;
		_varray[nr].attribute[0] = 0.0f;
		_varray[nr].attribute[1] = 0.0f;
		_varray[nr].attribute[2] = 0.0f;
		_varray[nr].float_attribute = 0.0f;

		nr++;

	}


	nr = 0;
	// define indicies for top cap
	for (int i = 0; i < segments-1; ++i)
	{
		_iarray[nr] = 0;
		nr++;
		_iarray[nr] = 1 + i;
		nr++;
		_iarray[nr] = 2 + i;
		nr++;
	}
	_iarray[nr] = 0;
	nr++;
	_iarray[nr] = segments;
	nr++;
	_iarray[nr] = 1;
	nr++;
}

gl4::Circle::~Circle()
{

}
