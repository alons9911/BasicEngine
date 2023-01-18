#pragma once
#include "../res/includes/glm/glm.hpp"

class MovableGLM
{
public:
	MovableGLM();
	glm::mat4 MakeTrans(const glm::mat4 &prevTransformations) const;
	glm::mat4 MakeTrans() const;
	void MyTranslate(glm::vec3 delta, int mode);
	void MyRotate(float angle, const glm::vec3 &vec, int mode);
	void MyScale(glm::vec3 scale);
	void ZeroTrans();
	void ResetTrans();


	glm::mat4 GetTranslate() { return rot_not_in_place * trans; }
	glm::mat4 GetRotate() { return rot_in_place; }
	glm::mat4 GetScale() { return scl; }

	void SetTranslate(glm::mat4 trans) { this->trans = trans; }
	void SetRotateInPlace(glm::mat4 rot) { this->rot_in_place = rot; }
	void SetRotateNotInPlace(glm::mat4 rot) { this->rot_not_in_place = rot; }
	void SetScale(glm::mat4 scl) { this->scl = scl; }

	glm::mat3 GetTransposedRot();
	void RotateRelative(float angle, const glm::vec3 &pos, const glm::vec3 &vec, int mode);

private:
	glm::mat4 trans;
	glm::mat4 rot_in_place;
	glm::mat4 rot_not_in_place;
	glm::mat4 scl;
};