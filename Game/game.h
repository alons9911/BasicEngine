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
	void MouseProccessing(int button);

	void increasePickedPoint();
	void decreasePickedPoint();

	void changeContinuity() {is_continuity = !is_continuity;}
	void changeSceneMode() {is_scene_mode = !is_scene_mode;}


private:
	Bezier1D *bezier;
	int picked_point_indx = 0;
	bool is_continuity = false;
	bool is_scene_mode = false;
};

