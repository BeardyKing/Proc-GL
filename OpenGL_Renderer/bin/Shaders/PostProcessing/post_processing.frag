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
uniform vec2 screen_resolution;
uniform int isVertical;
uniform float strength;
uniform int renderPassCount;
uniform float blurStrength_0;
uniform float blurStrength_1;
uniform float blurStrength_2;

uniform float vignette_pow_intensity;
uniform float vignette_multiply_intensity;

uniform bool isColorCorrection;
uniform bool isVignette;
uniform bool isDepthOfField;
uniform bool isFlipImage;

vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction);
vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction);
vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction);

void PostProcess_Vignette();
void PostProcess_Depth_Of_Field();

uniform sampler2D rendertextureColor;
uniform sampler2D rendertextureDepth;

uniform sampler2D rendertextureFinalColour;
uniform sampler2D rendertextureFinalBlur;
const float scaleFactors [7] = float [](0.006 , 0.061, 0.242, 0.383, 0.242, 0.061, 0.006);

void main(){ 
  if(isDepthOfField){
    PostProcess_Depth_Of_Field();
  }
  if(isVignette){
     PostProcess_Vignette();
  }
  if(isColorCorrection){

  }
  if(isFlipImage){
    fragColor = texture(rendertextureFinalBlur, vec2(fs_in.TexCoords.x, 1.0 - fs_in.TexCoords.y));
  }
}

void PostProcess_Vignette(){
  vec2 uv = gl_FragCoord.xy / screen_resolution.xy;
  
  uv *=  1.0 - uv.yx;   //vec2(1.0)- uv.yx; -> 1.-u.yx; Thanks FabriceNeyret !

  float vig = uv.x*uv.y * vignette_multiply_intensity; // multiply with sth for intensity

  vig = pow(vig, vignette_pow_intensity); // change pow for modifying the extend of the  vignette
  vig = clamp(vig,0,1);

  fragColor = texture(rendertextureFinalBlur, fs_in.TexCoords);
  fragColor *= vec4(vig);
}

void PostProcess_Depth_Of_Field(){
  vec4 out_blur = texture(rendertextureColor, fs_in.TexCoords);
  vec2 direction_amm = direction;
  float dist = (texture(rendertextureDepth, fs_in.TexCoords).r);
  dist+= radius;
  dist = clamp(dist,0 , 1);

  direction_amm.x = mix(direction_amm.x * strength, 0, dist);
  direction_amm.y = mix(direction_amm.y * strength, 0, dist);

  if(renderPassCount == 0){
    out_blur = blur5(rendertextureColor,fs_in.TexCoords, vec2(resolution), blurStrength_0 * direction_amm);
  }
  if(renderPassCount == 1){
    out_blur = blur9(rendertextureColor,fs_in.TexCoords, vec2(resolution), blurStrength_1 * direction_amm );
  }
  if(renderPassCount == 2){
    out_blur = blur13(rendertextureColor,fs_in.TexCoords, vec2(resolution), blurStrength_2 * direction_amm);
  }
  fragColor = out_blur; 
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