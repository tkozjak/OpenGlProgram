#version 330 core

uniform vec4 ourColor;

uniform sampler2D ourTexture_0; // image texture bound at 0
uniform sampler2D ourTexture_1; // image texture bound at 1

uniform sampler2D ourComputeShaderTexture;

uniform bool render_box = true;

in vec4 out_vertexColor;
in vec2 out_texCoordinate_01;

out vec4 FragColor;

void main(){
    vec4 textureColor_0 = texture(ourTexture_0, out_texCoordinate_01);
    vec4 textureColor_1 = texture(ourTexture_1, out_texCoordinate_01);
    vec4 computeTextureColor = texture(ourComputeShaderTexture, out_texCoordinate_01 );

    if( render_box){
        FragColor = mix(textureColor_0, textureColor_1, ourColor.g ) * computeTextureColor;
    }
    else{

        FragColor = out_vertexColor;
    }
}
