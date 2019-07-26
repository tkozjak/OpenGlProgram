#version 330 core

uniform vec4 ourColor;

uniform sampler2D ourTexture_0;
uniform sampler2D ourTexture_1;

uniform sampler2D ourComputeShaderTexture;

in vec4 out_vertexColor;
in vec2 out_texCoordinate_01;

out vec4 FragColor;

void main(){
    vec4 textureColor_0 = texture(ourTexture_0, out_texCoordinate_01);
    vec4 textureColor_1 = texture(ourTexture_1, out_texCoordinate_01);
    FragColor = mix(textureColor_0, textureColor_1, ourColor.g ) *  out_vertexColor;

    // compute shader texture
    FragColor = texture( ourComputeShaderTexture, out_texCoordinate_01);

    // compute shader texture
    FragColor = out_vertexColor;
}
