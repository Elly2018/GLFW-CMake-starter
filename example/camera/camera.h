#pragma once
#include <cglm/cglm.h>

class Camera 
{
public:
	Camera(vec3 pos, vec3 rot) 
	{
		glm_mat4_identity(view);
		glm_mat4_identity(projection);
		glm_translate(view, pos);
		glm_rotate(view, glm_deg(rot[0]), new float[3] { 1, 0, 0 });
		glm_rotate(view, glm_deg(rot[1]), new float[3] { 0, 1, 0 });
		glm_rotate(view, glm_deg(rot[2]), new float[3] { 0, 0, 1 });
		glm_perspective_default(640 / 480, projection);
	}

	void getView(mat4* v) { v = &view; }
	void getProjection(mat4* v) { v = &projection; }

	void move(vec3 vec, bool relative) 
	{
		if (relative) {

		}
		else {
			glm_translate(view, vec);
		}
	}
	void rot(vec3 vec) 
	{

	}

private:
	mat4 view;
	mat4 projection;
};