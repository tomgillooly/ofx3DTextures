#version 120

uniform sampler2DRect depthTexture; // Sampler for the depth image-space elevation texture
uniform float maxHeight;

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    vec4 pos = gl_Vertex;

    vec2 texcoord = gl_TexCoord[0].xy;

    
    // Set the vertex' depth image-space z coordinate from the texture:
    vec4 texel0 = texture2DRect(depthTexture, texcoord);  
    
    float depth = texel0.x;

    if (depth != 0)
    {
        // The texture is floating point 0.0-1.0
        pos.z = depth * maxHeight;
        pos.w = 1.0;
        
        gl_TexCoord[0].z = depth;
    }
    else
    {
        // If the depth texture is empty, we'll just see a flat teal quad
        gl_TexCoord[0].z = 0.5;
    }


    gl_Position = gl_ModelViewProjectionMatrix * pos;
}