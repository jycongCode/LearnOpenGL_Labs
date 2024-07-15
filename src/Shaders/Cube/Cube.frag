#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light{
    vec3 Position;
    vec3 Color;
};
uniform Light lights[16];

uniform sampler2D texture0;

uniform vec3 viewPos;

void main(){
    vec3 color = texture(texture0, fs_in.TexCoords).rgb;
    vec3 normal = fs_in.Normal;

    vec3 lighting = vec3(0.0);
    for(int i = 0;i<16;++i){
        // Ambient
//        vec3 ambient = 0.3 * color;
        // Diffuse
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lights[i].Color * color;

        vec3 result = diffuse;
        float distance = length(fs_in.FragPos - lights[i].Position);
        result *= 1.0 / (distance * distance);
        // Specular
//        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//        vec3 reflectDir = reflect(-lightDir, normal);
//        float spec = 0.0;
//        vec3 halfwayDir = normalize(lightDir + viewDir);
//        spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
//        vec3 specular = spec * lightColor;
        lighting += result;
    }
    FragColor = vec4(lighting, 1.0f);
}