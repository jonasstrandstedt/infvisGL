#include "FontManager.h"
#include "TextureManager.h"

#include <fstream>
#include <sstream>

FontManager * FontManager::instance = NULL;

inline void setVertex(	Vertex * v,
						float x, float y, float z,
						float nx, float ny, float nz,
						float s, float t,
						float r, float g, float b, float a,
						float a0=0.0f, float a1=0.0f, float a2=0.0f,
						float fa=0.0f)
{
    v->location[0] = x;
    v->location[1] = y;
    v->location[2] = z;

	v->normal[0] = nx;
	v->normal[1] = ny;
	v->normal[2] = nz;

	v->tex[0] = s;
	v->tex[1] = t;

	v->color[0] = r;
	v->color[1] = g;
	v->color[2] = b;
	v->color[3] = a;

	v->attribute[0] = a0;
	v->attribute[1] = a1;
	v->attribute[2] = a2;

	v->float_attribute = fa;
}

FontManager * FontManager::getInstance()
{
	if(!FontManager::instance)
	{
		FontManager::instance = new FontManager();
	}
	return FontManager::instance;
}

void FontManager::render(float width, float height)
{

	width = 1024.0f;
	height = 768.0f;
	glViewport(0, 0, width, height);

	glm::mat4 Orthogonal = glm::ortho(0.0f, width, 0.0f, height, 0.1f, 100.0f);
	glm::mat4 OrthogonalView = glm::lookAt(		glm::vec3(0,0,1), // Camera pos
												glm::vec3(0,0,0), // and looks at the origin
												glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
																);
	glm::mat4 _orthogonalProjectionMatrix = Orthogonal * OrthogonalView;

	float size = 1.0;
	float pixelScale = 1.0;
	float scalex = 1.0;
	float scaley = 1.0;


	gl4::ShaderManager::getInstance()->bindShader("text");
	gl4::TextureManager::getInstance()->bindTexture("fontmap", 0);
	GLuint program = gl4::ShaderManager::getInstance()->getShaderProgram("text");
	int colorLoc = glGetUniformLocation(program, "text");

	glm::mat4 transform = glm::mat4();

	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	glEnable(GL_BLEND);

	GLboolean params = false;
	glGetBooleanv(GL_DEPTH_TEST, &params);
	if (params)
	{
		glDisable (GL_DEPTH_TEST);
	}

	// Lite oklart vilken blend som krävs
    //glBlendEquation(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	buffer->render();

	glDisable(GL_BLEND);
	//glEnable (GL_DEPTH_TEST);

	if (params)
	{
		glEnable (GL_DEPTH_TEST);
	}

}

void FontManager::addText(float x, float y, const char * text, float fontSize, const glm::vec3 &color)
{
	float r = 1.0;
	float g = 0.0;
	float b = 0.0;
	float a = 1.0;

	float scale = fontSize / characterSize;

    for(size_t i=0; i<strlen(text); ++i )
    {
    	unsigned char index = (unsigned char)text[i];
    	Character * c = &charmap[index];

        float x0  = x + c->x_offset * scale;
        float y0  = y - c->y_offset * scale;
        float x1  = x0 + c->width * scale;
        float y1  = y0 - c->height * scale;

        float s0 = c->s0;
        float t0 = c->t0;
        float s1 = c->s1;
        float t1 = c->t1;

        int ind[6] = {0,1,2, 0,2,3};
        Vertex vert[4];

        setVertex(&vert[0], x0,y0,0.0,	0.0,0.0,1.0,	s0,t0,	r,g,b,a);
        setVertex(&vert[1], x0,y1,0.0,	0.0,0.0,1.0,	s0,t1,	r,g,b,a);
        setVertex(&vert[2], x1,y1,0.0,	0.0,0.0,1.0,	s1,t1,	r,g,b,a);
        setVertex(&vert[3], x1,y0,0.0,	0.0,0.0,1.0,	s1,t0,	r,g,b,a);

        printf("x0: %1.2f, y0: %1.2f, x1: %1.2f, y1: %1.2f \n",x0,y0,x1,y1);

        buffer->push_back(vert, 4, ind, 6 );
        x += c->x_advance * scale;
    }
}

FontManager::FontManager()
{
	const char * fontfile = "data/fonts/arial.fnt";
	const char * fontimage = "data/fonts/arial_dist.png";
	fontmapWidth = 512;
	fontmapHeight = 512;
	characterSize = 64.0f;

	gl4::TextureManager::getInstance()->loadTexture("fontmap", fontimage);
	loadFontData(fontfile, 512, 512);

	buffer = new FontVBO();

	addText(100.0,300.0,"Är du tuff eller?");

	buffer->init();
}

FontManager::~FontManager()
{
	delete buffer;
}

void FontManager::loadFontData(const char * filename, int width, int height)
{
	printf("loadFontData \n");
    charmap.clear();
    charmap.reserve(256);

    Character c;
    c.x = c.y = c.width = c.height = 0;
    c.x_offset=c.y_offset=c.x_advance = 0.0f;

    for(int i=0; i<256; ++i)
    {
        c.index = i;
        charmap.push_back(c);
    }

    std::string param;
    std::string junk;
    std::string line;

    std::ifstream file;
    file.open(filename, std::ifstream::in);

    if ( file.fail() )
        file.close();

    getline(file,line);
    getline(file,line);

    getline(file,line);

    int pos;
    int index;

    while ( !file.eof() )
    {
        std::istringstream ss(line, std::istringstream::in);

        ss >> param;
        if(param == "char")
        {
        	//printf("found char! \n");

            ss >> param; pos = param.find('=');
            index = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.x = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.y = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.width = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.height = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.x_offset = atof(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.y_offset = atof(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.x_advance = atof(param.substr(pos+1,param.size()-1).c_str());

        	c.s0 = (float)c.x / (float)width;
        	c.t0 = 1.0 - (float)c.y / (float)height;
        	c.s1 = (float)(c.x+c.width) / (float)width;
        	c.t1 = 1.0 - (float)(c.y+c.height) / (float)height;

        	//printf("%i %i %i %i %i %f %f %f \n", index, c.x, c.y, c.width, c.height, c.x_offset, c.y_offset, c.x_advance);

            charmap[index] = c;
        }

        std::getline(file,line);
    }

    file.close();
}

FontManager::FontVBO::FontVBO()
{
	_mode = GL_TRIANGLES;
	vertices.reserve(256);
	indices.reserve(256);
}

FontManager::FontVBO::~FontVBO()
{
	_varray = NULL;
	_iarray = NULL;
}

void FontManager::FontVBO::push_back(Vertex * vData, int vCount, int * iData, int iCount)
{
	int offset = (int)vertices.size();

	for(int i=0; i<vCount; ++i)
		vertices.push_back(vData[i]);

	for(int i=0; i<iCount; ++i)
		indices.push_back(offset+iData[i]);
}

void FontManager::FontVBO::init()
{
	// Set pointers to temporary data into
	_varray = vertices.data();
	_iarray = indices.data();

	_vsize = vertices.size();
	_isize = indices.size();

	printf("_vsize = %i, _isize = %i \n", _vsize, _isize);

	VBO::init();

	//vertices.clear();
	//indices.clear();
}