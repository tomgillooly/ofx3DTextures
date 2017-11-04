void main() {

    gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;
	// float size    = gl_Normal.x;
 //    gl_PointSize  = size;
 //    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;

}