#version 400 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec4 ClipSpace;
    
    mat4 view;
    mat4 projection;
    mat4 invView;
    mat4 invprojection;
} fs_in;

uniform samplerCube skybox;
uniform sampler2D cameraDepthRenderPass;
uniform sampler2D cameraColorRenderPass;
uniform float waterDepthBlend;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D shadowMap;

uniform vec4 albedo_color; 
uniform float normal_scalar;
uniform float metallic_scalar;
uniform float roughness_scalar;
uniform float occlusion_scalar;

uniform vec3 lightPositions[128];
uniform vec3 lightColors[128];
uniform int  amountOfLights;
uniform vec3 viewPos;

uniform float _shadowIntensity;
uniform float _lightIntensity;
uniform vec3  _lightColor;
uniform float _spec;

uniform float shadowBias;

// forward declare // SSR
// vec3 PositionFromDepth(float depth);
// vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
// vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth);
// vec3 fresnelSchlick(float cosTheta, vec3 F0);
// vec3 hash(vec3 a);

const float rayStep = 0.25;
const float minRayStep = 0.1;
const float maxSteps = 200;
const float searchDist = 5;
const float searchDistInv = 0.2;
const int numBinarySearchSteps = 5;
const float maxDDepth = 1.0;
const float maxDDepthInv = 1.0;


const float reflectionSpecularFalloffExponent = 3.0;

//forward declare //Edge Blending
float CalculateWaterDepth();

//forward declare // cubemap reflections
vec4 reflectTex;

vec3 getNormalFromMap();
vec4 GetReflectionFromCubemap(vec3 N);


// forward declare // BRDF 

const float PI = 3.14159265359;
vec3 WorldPos = fs_in.FragPos;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
float ShadowCalculation(vec4 fragPosLightSpace);
vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);

vec3 BRDF();

// forward decalre colour correction
vec3 ColorCorrection();
vec3 ColorCorrection(vec3 color){
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    return pow(color, vec3(1.0/2.2)); 
}


bool isUsingCubemapReflections  = false;
bool isUsingWaterDepth          = false;
bool isUsingBRDF                = false;
bool isUsingSSR                 = true;

vec3 BinarySearch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    float depth;


    for(int i = 0; i < numBinarySearchSteps; i++)
    {
        vec4 projectedCoord = fs_in.projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;


        depth = texture2D(cameraDepthRenderPass, projectedCoord.xy).z;


        dDepth = hitCoord.z - depth;


        if(dDepth > 0.0)
            hitCoord += dir;


        dir *= 0.5;
        hitCoord -= dir;    
    }


    vec4 projectedCoord = fs_in.projection * vec4(hitCoord, 1.0); 
    projectedCoord.xy /= projectedCoord.w;
    projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;


    return vec3(projectedCoord.xy, depth);
}


vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    dir *= rayStep;


    float depth;


    for(int i = 0; i < maxSteps; i++)
    {
        hitCoord += dir;


        vec4 projectedCoord = fs_in.projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;


        depth = texture2D(cameraDepthRenderPass, projectedCoord.xy).z;


        dDepth = hitCoord.z - depth;


        if(dDepth < 0.0)
            return vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);
    }


    return vec4(0.0, 0.0, 0.0, 0.0);
}

void main()
{	
    //------------Skybox Reflection ------------
    reflectTex = vec4(1);
    if(isUsingCubemapReflections){
        vec3 N = getNormalFromMap();
        reflectTex = GetReflectionFromCubemap(N);
    }

    //------------BRDF-----------------
    vec3 color = albedo_color.rgb;
    if(isUsingBRDF){
        color = BRDF();
        color = ColorCorrection(color);
    }
    
    //------------ Edge Depth ----------------
    float alpha_depth = 1;
    if(isUsingWaterDepth){
        float waterDepth = CalculateWaterDepth();
        alpha_depth = clamp( waterDepth / waterDepthBlend, 0.0, 1.0);
    }
    vec4 out_color = vec4(vec3(color), alpha_depth);
    
    if(isUsingSSR){
        vec2 gTexCoord = gl_FragCoord.xy * vec2(1280,720);


    // Samples
    float specular = texture2D(roughnessMap, gTexCoord).a;
    specular = 0.5;

    if(specular == 0.0){
        out_color = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }


    vec3 viewNormal = texture2D(normalMap, gTexCoord).xyz;
    vec3 viewPos = texture2D(cameraDepthRenderPass, gTexCoord).xyz;


    // Reflection vector
    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));


    // Ray cast
    vec3 hitPos = viewPos;
    float dDepth;


    vec4 coords = RayCast(reflected * max(minRayStep, -viewPos.z), hitPos, dDepth);


    vec2 dCoords = abs(vec2(0.5, 0.5) - coords.xy);


    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);


    // Get color
    out_color = vec4(texture2D(cameraColorRenderPass, coords.xy).rgb,

        pow(specular, reflectionSpecularFalloffExponent) *
        screenEdgefactor * clamp(-reflected.z, 0.0, 1.0) *
        clamp((searchDist - length(viewPos - hitPos)) * searchDistInv, 0.0, 1.0) * coords.w);
    }
    out_color.a = 1.0;

    // out
    FragColor = out_color;
    //FragColor = texture(cameraColorRenderPass, fs_in.TexCoords); // check that they colour render pass is working (it is) 
  

}

//SSR

// vec3 PositionFromDepth(float depth) {
//     float z = depth * 2.0 - 1.0;

//     vec4 clipSpacePosition = vec4(fs_in.TexCoords * 2.0 - 1.0, z, 1.0);
//     vec4 viewSpacePosition = fs_in.invprojection * clipSpacePosition;

//     // Perspective division
//     viewSpacePosition /= viewSpacePosition.w;

//     return viewSpacePosition.xyz;
// }

// vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
// {
//     float depth;

//     vec4 projectedCoord;
 
//     for(int i = 0; i < numBinarySearchSteps; i++)
//     {

//         projectedCoord = fs_in.projection * vec4(hitCoord, 1.0);
//         projectedCoord.xy /= projectedCoord.w;
//         projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
//         depth = textureLod(cameraDepthRenderPass, projectedCoord.xy, 2).z;
//         // depth = textureLod(gPosition, projectedCoord.xy, 2).z;

 
//         dDepth = hitCoord.z - depth;

//         dir *= 0.5;
//         if(dDepth > 0.0)
//             hitCoord += dir;
//         else
//             hitCoord -= dir;    
//     }

//         projectedCoord = fs_in.projection * vec4(hitCoord, 1.0);
//         projectedCoord.xy /= projectedCoord.w;
//         projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
//     return vec3(projectedCoord.xy, depth);
// }

// vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth)
// {

//     dir *= step;
 
 
//     float depth;
//     int steps;
//     vec4 projectedCoord;

 
//     for(int i = 0; i < maxSteps; i++)
//     {
//         hitCoord += dir;
 
//         projectedCoord = fs_in.projection * vec4(hitCoord, 1.0);
//         projectedCoord.xy /= projectedCoord.w;
//         projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
//         depth = textureLod(cameraDepthRenderPass, projectedCoord.xy, 2).z;
//         // depth = textureLod(gPosition, projectedCoord.xy, 2).z;
//         if(depth > 1000.0)
//             continue;
 
//         dDepth = hitCoord.z - depth;

//         if((dir.z - dDepth) < 1.2)
//         {
//             if(dDepth <= 0.0)
//             {   
//                 vec4 Result;
//                 Result = vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);

//                 return Result;
//             }
//         }
        
//         steps++;
//     }
 
    
//     return vec4(projectedCoord.xy, depth, 0.0);
// }


// vec3 hash(vec3 a)
// {
//     a = fract(a * Scale);
//     a += dot(a, a.yxz + K);
//     return fract((a.xxy + a.yxx)*a.zyx);
// }

// REFLECTION CUBEMAP
vec4 GetReflectionFromCubemap(vec3 N){
    vec3 I = normalize(fs_in.FragPos - viewPos); 
    vec3 R = reflect(I, normalize(fs_in.Normal));
    // vec4 reflectTex = vec4(texture(skybox, R).rgb, 1.0);
    return vec4(texture(skybox, normalize(N + R)).rgb, 1.0);
}

// EDGE BLENDING
float CalculateWaterDepth(){
    vec4 clip_pos = fs_in.ClipSpace;
    vec2 ndc = (clip_pos.xy / clip_pos.w) / 2 + 0.5;

    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    float near = 0.5; // TODO GET DATA FROM CAMERA IN UNIFROM
    float far = 800.0;// TODO GET DATA FROM CAMERA IN UNIFORM

    float depth = texture(cameraDepthRenderPass,refractTexCoords).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    return floorDistance - waterDistance;
}

// BRDF

vec3 BRDF(){
    
    vec3 N = getNormalFromMap();
    vec4 v4_albedo  = reflectTex * albedo_color;
    vec3 albedo     = pow(v4_albedo.rgb, vec3(2.2));
    float metallic  = texture(metallicMap, fs_in.TexCoords).r;
    float roughness = texture(roughnessMap, fs_in.TexCoords).r;
    float ao        = texture(aoMap, fs_in.TexCoords).r;

    metallic  = metallic * metallic_scalar;
    roughness = roughness * roughness_scalar;
    ao        = ao * occlusion_scalar;

    vec3 V = normalize(viewPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < amountOfLights; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        float shadow = ShadowCalculation(fs_in.FragPosLightSpace);   
        Lo += (kD * albedo / PI + specular) * ((radiance +(1.0 - shadow)) * NdotL) ;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao * _lightIntensity;
    
    return ambient + (Lo);
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, fs_in.TexCoords).xyz * 2.0 - 1.0 ;
    
    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.Normal) * vec3(normal_scalar);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// ----------------------------------------------------------------------------
// TODO FOR EACH SHADOW MAP IN SCENE
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPositions[0] - fs_in.FragPos); // TODO FOR EACH SHADOW MAP IN SCENE
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? (1.0 + _shadowIntensity) : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
// ----------------------------------------------------------------------------

