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
uniform float strength;
uniform bool blendPass;
uniform int renderPassCount;
uniform float blurStrength_0;
uniform float blurStrength_1;
uniform float blurStrength_2;

vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction);
vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction);
vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction);

uniform sampler2D rendertextureColor;
uniform sampler2D rendertextureDepth;
const float scaleFactors [7] = float [](0.006 , 0.061, 0.242, 0.383, 0.242, 0.061, 0.006);
void main(){ 
    //FragColor = texture(rendertextureColor, fs_in.TexCoords) * vec4(1,0,1,1);
    //FragColor = vec4(vec3(texture(rendertextureDepth, fs_in.TexCoords).r), 1) ;//* vec4(1,0,1,1);

   

	// vec4    sum = texture2D( rendertextureColor , vec2( uv.x - 4.0*blur*hstep , uv.y - 4.0*blur*vstep )) * 0.0162162162;
    //         sum += texture2D( rendertextureColor , vec2( uv.x - 3.0*blur*hstep , uv.y - 3.0*blur*vstep )) * 0.0540540541;
    //         sum += texture2D( rendertextureColor , vec2( uv.x - 2.0*blur*hstep , uv.y - 2.0*blur*vstep )) * 0.1216216216;
    //         sum += texture2D( rendertextureColor , vec2( uv.x - 1.0*blur*hstep , uv.y - 1.0*blur*vstep )) * 0.1945945946;
    //         sum += texture2D( rendertextureColor , vec2( uv.x , uv.y ))                                   * 0.2270270270;
    //         sum += texture2D( rendertextureColor , vec2( uv.x + 1.0*blur*hstep , uv.y + 1.0*blur*vstep )) * 0.1945945946;
    //         sum += texture2D( rendertextureColor , vec2( uv.x + 2.0*blur*hstep , uv.y + 2.0*blur*vstep )) * 0.1216216216;
    //         sum += texture2D( rendertextureColor , vec2( uv.x + 3.0*blur*hstep , uv.y + 3.0*blur*vstep )) * 0.0540540541;
    //         sum += texture2D( rendertextureColor , vec2( uv.x + 4.0*blur*hstep , uv.y + 4.0*blur*vstep )) * 0.0162162162;

	// fragColor = vec4( sum.rgb , 1.0 );
    vec4 out_blur = vec4(0);
    if(renderPassCount == 0){
        out_blur = blur5(rendertextureColor,fs_in.TexCoords, vec2(resolution), direction * blurStrength_0);
    }
    if(renderPassCount == 1){
        out_blur = blur9(rendertextureColor,fs_in.TexCoords, vec2(resolution), direction * blurStrength_1);
    }
    if(renderPassCount == 2){
        out_blur = blur13(rendertextureColor,fs_in.TexCoords, vec2(resolution), direction * blurStrength_2);
    }
    // if(blendPass){

    // }
    fragColor = out_blur;
    // fragColor = texture2D( rendertextureColor, fs_in.TexCoords) + (out_blur * strength);
    //fragColor = blur13(rendertextureColor,fs_in.TexCoords, vec2(resolution), direction);
}

vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.3333333333333333) * direction;
  color += texture2D(image, uv) * 0.29411764705882354;
  color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;
  color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;
  return color; 
}

vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.3846153846) * direction;
  vec2 off2 = vec2(3.2307692308) * direction;
  color += texture2D(image, uv) * 0.2270270270;
  color += texture2D(image, uv + (off1 / resolution)) * 0.3162162162;
  color += texture2D(image, uv - (off1 / resolution)) * 0.3162162162;
  color += texture2D(image, uv + (off2 / resolution)) * 0.0702702703;
  color += texture2D(image, uv - (off2 / resolution)) * 0.0702702703;
  return color;
}


vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
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
