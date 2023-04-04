#ifndef SWTOR_ANIMDATA_H
#define SWTOR_ANIMDATA_H

#include<../../glm/glm/glm.hpp>

struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset;

};

#endif