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
		glm_rotate(view, glm_rad(rot[0]), new float[3] { 1, 0, 0 });
		glm_rotate(view, glm_rad(rot[1]), new float[3] { 0, 1, 0 });
		glm_rotate(view, glm_rad(rot[2]), new float[3] { 0, 0, 1 });
		glm_perspective_default(640 / 480, projection);
	}

	void getView(mat4* v) { glm_mat4_copy(view, *v); }
	void getProjection(mat4* v) { glm_mat4_copy(projection, *v); }

	void move(vec3 vec, bool relative) 
	{
		if (relative) {
			mat4 rot;
			vec3 scale;
			vec3 a;
			glm_decompose_rs(view, rot, scale);
			glm_vec3_rotate_m4(rot, vec, a);
			glm_translate(view, a);
		}
		else {
			glm_translate(view, vec);
		}
	}
	void rot(vec3 vec) 
	{
		glm_rotate(view, glm_rad(vec[0]), new float[3] { 1, 0, 0 });
		glm_rotate(view, glm_rad(vec[1]), new float[3] { 0, 1, 0 });
		glm_rotate(view, glm_rad(vec[2]), new float[3] { 0, 0, 1 });
	}

private:
	mat4 view;
	mat4 projection;
};