#version 330 core
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflect1;
    float shininess;
};
uniform Material material;
out vec4 FragColor;

in vec2 TexCoords;

void main()
{
    FragColor = texture(material.texture_diffuse1,TexCoords);
}




