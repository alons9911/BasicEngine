#pragma once
#include "scene.h"
#include "Bezier1D.h"

class Game : public Scene
{
public:
	
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);

	void AddBezier1DShape(Shape* bezier_1D, int parent);
	Bezier1D *getBezier(){return bezier;}

private:
	Bezier1D *bezier;

};

