#version 120

#extension GL_ARB_texture_rectangle : enable
uniform sampler3D myTexture;

varying float depth;

void main()
{
    gl_FragColor = texture3D(myTexture, gl_TexCoord[0].xyz);
    // gl_FragColor = vec4(gl_TexCoord[0].x, gl_TexCoord[0].y, gl_TexCoord[0].z, 1);
    // gl_FragColor = vec4(gl_TexCoord[0].x, gl_TexCoord[0].x, gl_TexCoord[0].x, 1);
    // gl_FragColor = vec4(depth+0.1, depth+0.1, depth+0.1, 1);
    // gl_FragColor = vec4(1, 1, 1, 1);
}