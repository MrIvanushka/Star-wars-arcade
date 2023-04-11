#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;
layout (location = 4) in ivec4 BoneIDs;
layout (location = 5) in vec4 Weights;


out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;
flat out ivec4 vs_boneIDs;
out vec4 vs_weights;

const int MAX_BONE_INFLUENCE = 4;
const int MAX_BONES = 100;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 gBones[MAX_BONES];

void main()
{
	vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        vec4 localPosition = gBones[BoneIDs[i]] * vec4(vertex_position,1.0f);
        totalPosition += localPosition * Weights[i];
    }
    totalPosition = ModelMatrix * totalPosition;

	vs_position = totalPosition.xyz;
    
	vs_color = vertex_color;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
	vs_normal = mat3(ModelMatrix) * vertex_normal;

	gl_Position = ProjectionMatrix * ViewMatrix * totalPosition;
	
	vs_boneIDs = BoneIDs;
    vs_weights = Weights;
}