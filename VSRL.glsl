#version 330
layout(location=0)in vec3 vertex_position;
layout(location=1)in vec3 vertex_normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 frag_position;
out vec3 frag_normal;
void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(vertex_position,1);

	frag_position = (modelMat * vec4(vertex_position,1)).xyz;
	frag_normal = (transpose(inverse(modelMat)) * vec4(vertex_normal,1)).xyz;
}