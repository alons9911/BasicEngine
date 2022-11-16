#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../Engine3D/stb_image.h"
#include "assignment1.cpp"
using namespace std;


static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	//AddTexture("../res/textures/lena256.jpg",false);

	AddShape(Plane,-1,TRIANGLES);


	// new code 
	int width, height, numComponents;
	unsigned char* data = stbi_load("../res/textures/lena256.jpg", &width, &height, &numComponents, 4);
	
	vector<vector<unsigned char>> *grayscale = imageToGray(data, width, height);
	unsigned char *grayscaled_data = inflatePixelsToRGBA(grayscale, width, height);
	AddTexture(width, height, grayscaled_data);

	vector<vector<unsigned char>> *floyed_steinberged = floyedSteinberg(grayscale);
    unsigned char *floyed_data = inflatePixelsToRGBA(floyed_steinberged, width, height);
    AddTexture(width, height, floyed_data);

    vector<vector<unsigned char>> *halftoned = halftone(grayscale);
    unsigned char *halftoned_data = inflatePixelsToRGBA(halftoned, width * 2, height * 2);
    AddTexture(width * 2, height * 2, halftoned_data);

    vector<vector<unsigned char>> *cannyed = cannyAlgorithm(grayscale);
    unsigned char *canny_data = inflatePixelsToRGBA(cannyed, width, height);
	AddTexture(width, height, canny_data);

    // new code
	
	pickedShape = 0;

	MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
