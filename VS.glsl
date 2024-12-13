#version 330

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_color;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec4 baccolor;
uniform bool useCustomColor;

out vec4 frag_color;
void main()
{
    gl_Position = projMat * viewMat * modelMat * vertex_position;

    if (useCustomColor) {
        frag_color = baccolor; // Use custom color
    } else {
        frag_color = vertex_color; // Use vertex color
    }
}