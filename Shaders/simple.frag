#version 330 core

// global time (swings)
uniform float ourTime;

uniform sampler2D ourTexture_0; // image texture bound at 0
uniform sampler2D ourTexture_1; // image texture bound at 1
uniform sampler2D ourComputeShaderTexture; // image texture bound at 3

uniform bool render_box = true;

// from vertex shader
in vec4 out_vertexColor;
in vec2 out_texCoordinate_01;

// output
out vec4 FragColor;

void main(){
    vec4 textureColor_0 = texture(ourTexture_0, out_texCoordinate_01);
    vec4 textureColor_1 = texture(ourTexture_1, out_texCoordinate_01);
    vec4 computeTextureColor = texture(ourComputeShaderTexture, out_texCoordinate_01 );

    if( render_box ){
        FragColor = mix(textureColor_0, textureColor_1, ourTime ) * computeTextureColor;
    }
    else{

        FragColor = out_vertexColor;
    }
}
