#version 120

uniform sampler2DRect depthTexture; // Sampler for the depth image-space elevation texture
uniform float maxHeight;

varying float depth;

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    // gl_TexCoord[1].z = 0.75;
    vec4 pos = gl_Vertex;
    // pos.z = gl_TexCoord[0].z*255.0;
    // gl_Position   = gl_ModelViewProjectionMatrix * pos;
	// float size    = gl_Normal.x;
 //    gl_PointSize  = size;
 //    gl_FrontColor = gl_Color;
    // gl_TexCoord[0] = gl_MultiTexCoord0;

    // vec4 position = gl_Vertex;
    vec2 texcoord = gl_TexCoord[0].xy;
    // pos.xy = gl_TexCoord[0].xy;
    // // copy position so we can work with it.
    // vec4 pos = position;
    // vec2 varyingtexcoord = pos.xy;//texcoord;
    
    // Set the vertex' depth image-space z coordinate from the texture:
    vec4 texel0 = texture2DRect(depthTexture, texcoord);  
    
    depth = texel0.x;
    // depth = 0;
    // gl_TexCoord[0].xy = texcoord;

    // pos.xy = texcoord;
    if (depth != 0)
    {
        pos.z = depth * maxHeight;
        pos.w = 1.0;
        
        gl_TexCoord[0].z = depth;
    }
    else
    {
        // If the depth texture is empty, we'll just see a teal quad
        gl_TexCoord[0].z = 0.5;
    }


    gl_Position = gl_ModelViewProjectionMatrix * pos; //gl_ModelViewProjectionMatrix * gl_Vertex;
}