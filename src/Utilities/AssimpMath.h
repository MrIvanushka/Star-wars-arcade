#ifndef SWTOR_ASSIMPMATH_H
#define SWTOR_ASSIMPMATH_H

#include "../../glm/glm/glm.hpp"
#include "assimp/scene.h"

class AssimpMath
{
public:
    static void convert_aimatrix_to_glm(glm::mat4& to, const aiMatrix4x4& from) {
	    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    }

    static void convert_aimatrix_to_glm(glm::mat4& to, const aiMatrix3x3& from) {
	    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3;
		
	    // The rest would be zero, other than the 4,4.
	    to[0][3] = 0.0f;
	    to[1][3] = 0.0f;
	    to[2][3] = 0.0f;

	    to[3][0] = 0.0f;
	    to[3][1] = 0.0f;
	    to[3][2] = 0.0f;

	    to[3][3] = 1.0f;
    }

	static glm::quat getGLMQuat(const aiQuaternion& pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}
};


#endif