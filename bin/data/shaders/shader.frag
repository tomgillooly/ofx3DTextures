#extension GL_ARB_texture_rectangle : enable
uniform sampler3D myTexture;
void main()
{
    gl_FragColor = texture3D(myTexture, gl_TexCoord[0].xyz);
    // gl_FragColor = vec4(gl_TexCoord[0].x, gl_TexCoord[0].y, gl_TexCoord[0].z, 1);
}