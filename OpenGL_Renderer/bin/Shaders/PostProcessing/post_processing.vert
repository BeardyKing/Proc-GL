#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec4 fragColor;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform vec2 textureScale;

uniform float threshold;
uniform float strength;

uniform sampler2D rendertextureColor;


void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = vec2(aTexCoords.x * textureScale.x, aTexCoords.y * textureScale.y);
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    // gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 

    vec4 color = texture2D( rendertextureColor , aTexCoords);
    // convert rgb to grayscale/brightness
    float brightness = dot( color.rgb , vec3( 0.2126 , 0.7152 , 0.0722 ) );
    if ( brightness > threshold ) 
    {
        vs_out.fragColor = vec4( strength * color.rgb , 1.0 );
    }
    else {
        vs_out.fragColor = vec4( 0.0 , 0.0 , 0.0 , 1.0 );
    }

}
// BLOOM
// #version 330 core
// uniform float threshold;
// uniform float strength;
// uniform sampler2D colorTexture;
// in vec2 uv;
// out vec4 fragColor;
// void main()
// {
//     vec4 color = texture2D( colorTexture , uv );
//     // convert rgb to grayscale/brightness
//     float brightness = dot( color.rgb , vec3( 0.2126 , 0.7152 , 0.0722 ) );
//     if ( brightness > threshold ) fragColor = vec4( strength * color.rgb , 1.0 );
//     else fragColor = vec4( 0.0 , 0.0 , 0.0 , 1.0 );
// }
// BLOOM

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 aTexCoords;

// out vec2 TexCoords;

// void main()
// {
//     gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
//     TexCoords = aTexCoords;
// } 