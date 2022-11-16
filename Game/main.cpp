#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include "../Engine3D/stb_image.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


static void printMatToFile(const unsigned char* arr, const int height, const int width)
{
	ofstream myfile ("lena.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				myfile<<static_cast<unsigned>(arr[i * width + j])<<", ";
			}
			myfile<<"\n";
		}
		myfile.close();
	}
  	else cout << "Unable to open file";
	
}

static void printVectorMat(std::vector<std::vector<unsigned char>> mat)
{
	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat[i].size(); j++)
		{
			cout << int(mat[i][j]) << ",";
		}
		cout << endl;
	}
}

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 800;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
	
	scn->Init();

	display.SetScene(scn);

	while(!display.CloseWindow())
	{
		
		scn->SetShapeTex(0,0);
		scn->Draw(1,0,scn->BACK,true,false, 1);
		scn->SetShapeTex(0,1);
		scn->Draw(1,0,scn->BACK,false,false, 2);
		scn->SetShapeTex(0,2);
		scn->Draw(1,0,scn->BACK,false,false, 3);
		scn->SetShapeTex(0,3);
		scn->Draw(1,0,scn->BACK,false,false, 4);

		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();		
	}
	delete scn;
	return 0;
}
