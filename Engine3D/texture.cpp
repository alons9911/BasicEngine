//#define GLEW_STATIC
// #define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include "stb_image.h"
#include "../res/includes/glad/include/glad/glad.h"
#include <iostream>
#include <vector>
#include "../Game/assignment1.cpp"
using namespace std;

Texture::Texture(const std::string& fileName)
{
	int width, height, numComponents;
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

    // new code
	std::cout<<width<<" "<<height<<" "<<numComponents;
	std::cout<<std::endl;
	vector<vector<unsigned char>> *matrix = imageToGray(data, height, width);
	vector<vector<unsigned char>> *floyed_steinberged = floyedSteinberg(matrix);
    //unsigned char* newArray = matrixToArray(floyed_steinberged, &width, &height);
    //unsigned char* newArray = matrixToArray(matrix, &width, &height);
    unsigned char* newArrayFloyd = inflatePixelsToRGBA(floyed_steinberged, width, height);
    
    vector<vector<unsigned char>> *halftoned = halftone(matrix);
    //unsigned char *orig = (unsigned char*)(malloc(width * height * 4));
    //unsigned char *newArray = matrixToArray(halftoned, &width, &height);
    //unsigned char *newArray = grayImageToOriginalRgbaTemplate(halftoned, inflatePixelsToSquare(data, 256, 256), width * 2, height * 2);
    unsigned char *newArrayHalftone = inflatePixelsToRGBA(halftoned, width * 2, height * 2);
    cout << height << ", " << width << endl;

    data = newArrayFloyd;
    data = newArrayHalftone;
    width *= 2;
    height *= 2;
    // new code
	
    if(data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;
        
    glGenTextures(1, &m_texture);
    Bind(m_texture);
        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,-0.4f);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

Texture::Texture(int width,int height,unsigned char *data)
{
    glGenTextures(1, &m_texture);
    Bind(m_texture);
        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

