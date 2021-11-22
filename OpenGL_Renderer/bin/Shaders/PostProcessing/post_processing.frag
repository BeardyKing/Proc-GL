#version 330 core
out vec4 fragColor;
  
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec4 fragColor;

} fs_in;

uniform float resolution;
uniform float radius;
uniform vec2 direction;
uniform int isVertical;

uniform sampler2D rendertextureColor;
uniform sampler2D rendertextureDepth;
const float scaleFactors [7] = float [](0.006 , 0.061, 0.242, 0.383, 0.242, 0.061, 0.006);
void main(){ 
    //FragColor = texture(rendertextureColor, fs_in.TexCoords) * vec4(1,0,1,1);
    //FragColor = vec4(vec3(texture(rendertextureDepth, fs_in.TexCoords).r), 1) ;//* vec4(1,0,1,1);

    vec2 uv = fs_in.TexCoords;

    
    float blur = radius/resolution;
    
    float hstep = direction.x;
    float vstep = direction.y;

	vec4    sum = texture2D( rendertextureColor , vec2( uv.x - 4.0*blur*hstep , uv.y - 4.0*blur*vstep )) * 0.0162162162;
            sum += texture2D( rendertextureColor , vec2( uv.x - 3.0*blur*hstep , uv.y - 3.0*blur*vstep )) * 0.0540540541;
            sum += texture2D( rendertextureColor , vec2( uv.x - 2.0*blur*hstep , uv.y - 2.0*blur*vstep )) * 0.1216216216;
            sum += texture2D( rendertextureColor , vec2( uv.x - 1.0*blur*hstep , uv.y - 1.0*blur*vstep )) * 0.1945945946;
            sum += texture2D( rendertextureColor , vec2( uv.x , uv.y )) * 0.2270270270;
            sum += texture2D( rendertextureColor , vec2( uv.x + 1.0*blur*hstep , uv.y + 1.0*blur*vstep )) * 0.1945945946;
            sum += texture2D( rendertextureColor , vec2( uv.x + 2.0*blur*hstep , uv.y + 2.0*blur*vstep )) * 0.1216216216;
            sum += texture2D( rendertextureColor , vec2( uv.x + 3.0*blur*hstep , uv.y + 3.0*blur*vstep )) * 0.0540540541;
            sum += texture2D( rendertextureColor , vec2( uv.x + 4.0*blur*hstep , uv.y + 4.0*blur*vstep )) * 0.0162162162;

	fragColor = vec4( sum.rgb , 1.0 );
}

// for(int i = 0; i < 7; i++ ) {
//         for(int j = 0; j < 7; j++ ) {
//             // vec2 offset = vec2(dFdx(fs_in.TexCoords).x, dFdy(fs_in.TexCoords).y) * (i - 3);
//             vec2 offset = vec2(dFdx(fs_in.TexCoords).x * (i - 3), dFdy(fs_in.TexCoords).y * (j - 3));
//             vec4 tmp = texture2D(rendertextureColor , fs_in.TexCoords.xy + offset);
//             FragColor += tmp * scaleFactors[i];
//         }
//     }

// BLOOM
// #version 330 core
// uniform sampler2D colorTexture;
// uniform float resolution;
// uniform float radius;
// uniform vec2 direction;
// in vec2 uv;
// out vec4 fragColor;
// void main()
// {
// 	float blur = radius/resolution;
// 	float hstep = direction.x;
// 	float vstep = direction.y;

// 	vec4 sum = texture2D( colorTexture , vec2( uv.x - 4.0*blur*hstep , uv.y - 4.0*blur*vstep )) * 0.0162162162;
// 	sum += texture2D( colorTexture , vec2( uv.x - 3.0*blur*hstep , uv.y - 3.0*blur*vstep )) * 0.0540540541;
// 	sum += texture2D( colorTexture , vec2( uv.x - 2.0*blur*hstep , uv.y - 2.0*blur*vstep )) * 0.1216216216;
// 	sum += texture2D( colorTexture , vec2( uv.x - 1.0*blur*hstep , uv.y - 1.0*blur*vstep )) * 0.1945945946;
// 	sum += texture2D( colorTexture , vec2( uv.x , uv.y )) * 0.2270270270;
// 	sum += texture2D( colorTexture , vec2( uv.x + 1.0*blur*hstep , uv.y + 1.0*blur*vstep )) * 0.1945945946;
// 	sum += texture2D( colorTexture , vec2( uv.x + 2.0*blur*hstep , uv.y + 2.0*blur*vstep )) * 0.1216216216;
// 	sum += texture2D( colorTexture , vec2( uv.x + 3.0*blur*hstep , uv.y + 3.0*blur*vstep )) * 0.0540540541;
// 	sum += texture2D( colorTexture , vec2( uv.x + 4.0*blur*hstep , uv.y + 4.0*blur*vstep )) * 0.0162162162;

// 	fragColor = vec4( sum.rgb , 1.0 );
// }
// BLOOM
/*
	// the uniforms for the two gaussian blur passes
	glUniform1f( u_resolution , width );
	glUniform2f( u_direction , 1.0f , 0.0f );
	// draw horizontal
	glUniform1f( u_resolution , height );
	glUniform2f( u_direction , 0.0f , 1.0f );
	// draw vertical
*/
