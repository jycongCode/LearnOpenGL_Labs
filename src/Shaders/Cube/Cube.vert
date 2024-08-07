#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform bool inverse_Normals;
void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vs_out.Normal = normalize(normalMatrix * aNormal);
    if(inverse_Normals)
            vs_out.Normal *= -1.0;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPos = vec3(model * vec4(aPos,1.0));
}