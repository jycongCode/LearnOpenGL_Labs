#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture0;
uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main(){
    vec2 texOffset = 1.0 / textureSize(texture0,0);
    vec3 color = texture(texture0,TexCoords).rgb * weight[0];
    for(int i = 1;i<5;++i){
        if(horizontal){
            color += texture(texture0,TexCoords + vec2(texOffset.x * i,0.0)).rgb * weight[i];
            color += texture(texture0,TexCoords - vec2(texOffset.x * i,0.0)).rgb * weight[i];
        }else{
            color += texture(texture0,TexCoords + vec2(0.0,texOffset.y * i)).rgb * weight[i];
            color += texture(texture0,TexCoords - vec2(0.0,texOffset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(color,1.0);
}