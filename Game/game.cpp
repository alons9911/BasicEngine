#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "./Bezier1D.cpp"

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
	

	
	pickedShape = 0;
	
	//MoveCamera(0,zTranslate,10);
	pickedShape = -1;

	bezier = new Bezier1D(3, 91, this);
	bezier->Init(this);

	//bezier->SetMesh(bezier->GetLine());
    AddBezier1DShape(bezier, -1);
    //SetShapeTex(shapes.size() - 1, 1);

    MoveCamera(0, zTranslate, 40);
	
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

void Game::AddBezier1DShape(Shape* bezier_1D_line, int parent)
{
	chainParents.push_back(parent);
	shapes.push_back(bezier_1D_line);
}

void Game::MouseProccessing(int button)
{
	if(pickedShape == -1)
	{
		if (is_scene_mode)
		{
			if(button == 1 )
			{			
				MyTranslate(glm::vec3(-1 * GetXRel() / 20.0f, 0, 0), 0);
				MyTranslate(glm::vec3(0, GetYRel() / 20.0f, 0), 0);
				WhenTranslate();
			}
			else
			{
				MyRotate(GetXRel() / 2.0f, glm::vec3(1,0,0), 0);
				MyRotate(GetYRel() / 2.0f, glm::vec3(0,0,1), 0);
				WhenRotate();
			}
		} 
		else{
			if(button == 1 )
			{			
				getBezier()->CurveUpdate(picked_point_indx, -1.0f * GetXRel() / 2.0f, GetYRel() / 2.0f, is_continuity);
				WhenTranslate();
			}
			else
			{
				getBezier()->CurveUpdateRotation(picked_point_indx, -1.0f * GetXRel() / 2.0f, GetYRel() / 2.0f, is_continuity);
				WhenRotate();
			}
		}
	}
}

void Game::increasePickedPoint()
{
	int max_point = getBezier()->GetSegmentsNum() * 3;
	picked_point_indx = min(picked_point_indx + 1, max_point);
}
void Game::decreasePickedPoint()
{
	picked_point_indx = max(picked_point_indx - 1, 0);
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
		bezier->MoveCube();
	}
}

Game::~Game(void)
{
}
