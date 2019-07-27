#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 texCoordinate_01;

out vec4 out_vertexColor;
out vec2 out_texCoordinate_01;

// matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
    gl_Position = projection * view * model * vec4(vertexPosition.xyz, 1.0);
    out_vertexColor = vertexColor;
    out_texCoordinate_01 = texCoordinate_01;
    gl_PointSize = 5;
}
