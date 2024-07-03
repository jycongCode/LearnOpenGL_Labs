#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
uniform mat4 model;
void main()
{
    vec4 pos = vec4(aPos.x,aPos.y,0.0,1.0);
    gl_Position = model * pos;
    TexCoords = aTexCoords;
}
