#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture0;
void main(){
    float depthValue = texture(texture0,TexCoords).r;
    FragColor = vec4(vec3(depthValue),1.0);
//    FragColor = texture(texture0,TexCoords);
}