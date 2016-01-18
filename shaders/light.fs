#version 330 core

#extension GL_ARB_texture_query_lod : enable


// Interpolated values from the vertex shaders

// in vec3 fragmentColor;
in vec2 UV;
in vec2 tile;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 Normal_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Output data
// out vec4 color;
layout(location = 1) out int materialID;
layout(location = 4) out vec3 normal;
layout(location = 0) out vec4 color;
layout(location = 3) out vec3 position;
layout(location = 2) out vec4 specularColor;

// layout (depth_greater) out float gl_FragDepth;

// Values that stay constant for the whole mesh.

uniform sampler2DArray myTextureSampler;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat4 V;

void main()
{
  // Defaults
  gl_FragDepth = gl_FragCoord.z;
  
  vec3 LightPosition_worldspace = vec3(600, 1000, -160);
  
  vec4 materialColor;
  // Output color = color of the texture at the specified UV
  vec3 foo;
  foo.x = UV.x;
  foo.y = UV.y;
  foo.z = 32.0f*tile.y + tile.x;

  
  materialColor = texture( myTextureSampler, foo ).rgba;

  // if( materialColor.w < 0.1f) {
  //   gl_FragDepth = 10000.0f;
  // }
  
  //////////////////////////////////////////////////
  
  // Light emission properties
  // You probably want to put them as uniforms
  vec3 LightColor = vec3(1,1,1);
  float LightPower = 50.0f;
	
  // Material properties
  vec3 MaterialDiffuseColor = materialColor.rgb;
  vec3 MaterialAmbientColor = vec3(0.6,0.6,0.6) * MaterialDiffuseColor;
  vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

  // Distance to the light
  float distance = length( LightPosition_worldspace - Position_worldspace );
  //float distance = 1;

  // Normal of the computed fragment, in camera space
  vec3 n = normalize( Normal_cameraspace );
  // Direction of the light (from the fragment to the light)
  vec3 l = normalize( LightDirection_cameraspace );
  // Cosine of the angle between the normal and the light direction, 
  // clamped above 0
  //  - light is at the vertical of the triangle -> 1
  //  - light is perpendicular to the triangle -> 0
  //  - light is behind the triangle -> 0
  float cosTheta = clamp( dot( n,l ), 0,1 );
  // cosTheta = 0.5;
	
  // Eye vector (towards the camera)
  vec3 E = normalize(EyeDirection_cameraspace);
  // Direction in which the triangle reflects the light
  vec3 R = reflect(-l,n);
  // Cosine of the angle between the Eye vector and the Reflect vector,
  // clamped to 0
  //  - Looking into the reflection -> 1
  //  - Looking elsewhere -> < 1
  float cosAlpha = clamp( dot( E,R ), 0,1 );

  MaterialDiffuseColor = MaterialDiffuseColor * cosTheta * 0.4;
  // * LightColor * LightPower * cosTheta / (distance*distance)
     
  vec3 new_color = 
    // Ambient : simulates indirect lighting
    MaterialAmbientColor
    // Diffuse : "color" of the object
    + MaterialDiffuseColor
    // Specular : reflective highlight, like a mirror
    // MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance)
    ;

  color.xyz = new_color;
  color.w = materialColor.w;

}
