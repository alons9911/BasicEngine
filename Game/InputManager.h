#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include "scene.h"
#include <iostream>
using namespace std;


	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
			scn->Picking((int)x2,(int)y2);
		}
		
		
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		scn->MyTranslate(glm::vec3(0,0,xoffset),0);
		
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;
				case GLFW_KEY_SPACE:
					if(scn->IsActive()){
						scn->Deactivate();
						scn->getBezier()->ResetCubePosition();
					}else
						scn->Activate();
				break;
				case GLFW_KEY_2:
					scn->getBezier()->ResetCurve(2);
					break;
				case GLFW_KEY_3:
					scn->getBezier()->ResetCurve(3);
					break;
				case GLFW_KEY_4:
					scn->getBezier()->ResetCurve(4);
					break;
				case GLFW_KEY_5:
					scn->getBezier()->ResetCurve(5);
					break;
				case GLFW_KEY_6:
					scn->getBezier()->ResetCurve(6);
					break;
				case GLFW_KEY_R:
					scn->MoveCamera(0, scn->xTranslate, 0.5f);
					break;
				case GLFW_KEY_L:
					scn->MoveCamera(0, scn->xTranslate, -0.5f);
					break;
				case GLFW_KEY_U:
					scn->MoveCamera(0, scn->yTranslate, 0.5f);
					break;
				case GLFW_KEY_D:
					scn->MoveCamera(0, scn->yTranslate, -0.5f);
					break;
				case GLFW_KEY_B:
					scn->MoveCamera(0, scn->zTranslate, 0.5f);
					break;
				case GLFW_KEY_F:
					scn->MoveCamera(0, scn->zTranslate, -0.5f);
					break;
				case GLFW_KEY_RIGHT:
					scn->increasePickedPoint();
					break;
				case GLFW_KEY_LEFT:
					scn->decreasePickedPoint();
					break;
				case GLFW_KEY_C:
					scn->changeContinuity();
					break;
				case GLFW_KEY_UP:
					scn->MoveCamera(0,scn->zTranslate,0.4f);
					break;
				case GLFW_KEY_DOWN:
					//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
					//cout<< "down: "<<endl;
					scn->MoveCamera(0,scn->zTranslate,-0.4f);
					break;

			default:
				break;
			}
		}
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->UpdatePosition((float)xpos,(float)ypos);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->Resize(width,height);
		
	}

	void Init(Display &display)
	{
		display.AddKeyCallBack(key_callback);
		display.AddMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.AddResizeCallBack(window_size_callback);
	}
