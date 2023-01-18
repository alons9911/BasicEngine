#include "MovableGLM.h"
#include <stdio.h>
#include "../res/includes/glm/gtc/matrix_transform.hpp"
#include "../res/includes/glm/gtc/quaternion.hpp"


static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

MovableGLM::MovableGLM()
{
	ZeroTrans();
}

glm::mat4 MovableGLM::MakeTrans(const glm::mat4 &prevTransformations) const
{
	return prevTransformations * MakeTrans();
}

glm::mat4 MovableGLM::MakeTrans() const
{
	return  rot_not_in_place * trans * scl * rot_in_place;
}

void MovableGLM::MyTranslate(glm::vec3 delta,int mode)
{
	trans = glm::translate(trans,delta);
}

void  MovableGLM::MyRotate(float angle,const glm::vec3 &vec,int mode)
{
	if (mode == 0)
		rot_in_place = glm::rotate(rot_in_place,angle,vec);
	else
		rot_not_in_place = glm::rotate(rot_not_in_place,angle,vec);
}

glm::mat3 MovableGLM::GetTransposedRot() {
	glm::mat3 transposed_rot = glm::mat3(
		glm::vec3(rot_not_in_place[0][0], rot_not_in_place[0][1], rot_not_in_place[0][2]),
		glm::vec3(rot_not_in_place[1][0], rot_not_in_place[1][1], rot_not_in_place[1][2]),
		glm::vec3(rot_not_in_place[2][0], rot_not_in_place[2][1], rot_not_in_place[2][2])
    );
	return transposed_rot;
}


void MovableGLM::RotateRelative(float angle, const glm::vec3& pos, const glm::vec3& vec, int mode) {
	if (mode == 0)
	{
		MyTranslate(-1.0f * pos, 0);
		MyRotate(angle, vec, mode);
		MyTranslate(pos, 0);
	}

	if (mode == 1)
	{
		glm::mat4 t1 = glm::translate(glm::mat4(1), -pos);
		glm::mat4 r = glm::rotate(glm::mat4(1), angle, vec);
		glm::mat4 t2 = glm::translate(glm::mat4(1), pos);
		rot_not_in_place *= t2 * r * t1;

	}
	
}	
void  MovableGLM::MyScale(glm::vec3 scale)
{
	scl = glm::scale(scl,scale);
}

void MovableGLM::ZeroTrans()
{
	trans = glm::mat4(1);
	rot_in_place = glm::mat4(1);
	scl = glm::mat4(1);
	rot_not_in_place = glm::mat4(1);
}

void MovableGLM::ResetTrans()
{
	trans = glm::mat4(1);
	rot_in_place = glm::mat4(1);
	rot_not_in_place = glm::mat4(1);
}