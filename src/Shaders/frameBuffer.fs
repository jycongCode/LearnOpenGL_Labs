#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture0;
const float offset = 1.0/300.0;
void main(){
//     vec2 offsets[9] = vec2[](
//         vec2(-offset,  offset), // 左上
//         vec2( 0.0,    offset), // 正上
//         vec2( offset,  offset), // 右上
//         vec2(-offset,  0.0),   // 左
//         vec2( 0.0,    0.0),   // 中
//         vec2( offset,  0.0),   // 右
//         vec2(-offset, -offset), // 左下
//         vec2( 0.0,   -offset), // 正下
//         vec2( offset, -offset)  // 右下
//     );
//
//     float kernel[9] = float[](
//         1, 1, 1,
//         1, -8, 1,
//         1, 1, 1
//     );
//
//     vec3 sampleTex[9];
//     for(int i = 0;i<9;++i){
//         sampleTex[i] = texture(texture0,TexCoords + offsets[i]).rgb;
//     }
//
//     vec3 col = vec3(0.0);
//     for(int i = 0;i<9;++i){
//         col += sampleTex[i] * kernel[i];
//     }
//     FragColor = vec4(col,1.0);
       vec3 col = texture(texture0,TexCoords).rgb;
       FragColor = vec4(col,1.0);
}