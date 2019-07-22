#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 texCoordinate_01;

out vec4 out_vertexColor;
out vec2 out_texCoordinate_01;


void main(){
    gl_Position = vec4(vertexPosition.xyz, 1.0);
    out_vertexColor = vertexColor;
    out_texCoordinate_01 = texCoordinate_01;
}
