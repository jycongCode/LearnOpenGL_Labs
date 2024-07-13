#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture0;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    vec3 color = texture(texture0, fs_in.TexCoords).rgb;
    vec3 normal = texture(normalMap,fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 lightColor = vec3(0.3);
    // Ambient
    vec3 ambient = 0.3 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    vec3 lighting = (ambient + diffuse + specular) * color;

    FragColor = vec4(lighting, 1.0f);
}