#include "ofApp.h"

#include "Perlin.h"

#include <iostream>

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    // heightMapFile      = "";
    // gTerrain        = new Terrain();
    // gSkyVertices    = NULL;
    // gSkyIndices     = NULL;
    waterChange    = 0.6f;
    waterLevel     = 0.0f;
    terrainHeight  = 150.0f;

    angle = 0;

    sunApex = 50.0f * pi / 180.f;
    sunPos = glm::vec2(0.0f, sunApex);
    sunDir = glm::vec3();

    ofEnableAlphaBlending();
 //    int camWidth 		= 320;	// try to grab at this size. 
	// int camHeight 		= 240;
    
 //    vidGrabber.setVerbose(true);
	// vidGrabber.setup(camWidth,camHeight);
    
 //    fingerMovie.load("fingers.mov");
	// fingerMovie.play();
    
    if (!logoImg.load("colors.jpg"))
    {
        ofExit();
    }
 //    multimaskImg.load("mask.jpg");
    
    // fbo.allocate(camWidth,camHeight);
 //    maskFbo.allocate(camWidth,camHeight);
    
    //ofSetWindowShape(camWidth, camHeight*2);
    
    // There are 3 of ways of loading a shader:
    //
    //  1 - Using just the name of the shader and ledding ofShader look for .frag and .vert: 
    //      Ex.: shader.load( "myShader");
    //
    //  2 - Giving the right file names for each one: 
    //      Ex.: shader.load( "myShader.vert","myShader.frag");
    //
    //  3 - And the third one is passing the shader programa on a single string;
    //      In this particular example we are using STRINGIFY which is a handy macro
    // string shaderProgram = STRINGIFY(
    //                                  uniform sampler2DRect tex0;

    //                                  void main (void){
    //                                      vec2 pos = gl_TexCoord[0].st;
                                         
    //                                      vec4 rTxt = texture2DRect(tex0, pos);
                                         
    //                                      vec4 color = vec4(0,0,0,0);
    //                                      color = mix(color, rTxt, mask.r );
    //                                      color = mix(color, gTxt, mask.g );
    //                                      color = mix(color, bTxt, mask.b );
                                         
    //                                      gl_FragColor = color;
    //                                  }
    //                                  );
    shader.load("Shader.vs", "Shader.ps");
    // shader.load("SkyShader.vs", "SkyShader.ps");
    // shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);

    // shader.linkProgram(); 

    // camera.setFov(60.0f);
    // camera.setNearClip(1.0f);
    // camera.setFarClip(2000.0f);
    
    // Let's clear the FBOs
    // otherwise it will bring some junk with it from the memory    
    // fbo.begin();
    // ofClear(0,0,0,255);
    // fbo.end();
    
    // maskFbo.begin();
    // ofClear(0,0,0,255);
    // maskFbo.end();

    //Load/Generate heightmap
    if (!heightMapFile.empty())
    {
        //Load heightmap from file
        if (endsWith(heightMapFile, ".pnm"))
        {
            if (!LoadHeightMap(heightMapFile.c_str()))
            {
                printf("#ERROR: Load heightmap failed\n");
                ofExit();
            }
        }
        // else if (endsWith(heightMapFile, ".asc"))
        // {
        //     if (!gTerrain->LoadGeoHeightMap(heightMapFile.c_str()))
        //     {
        //         ofExit();
        //     }
        // }
        else
        {
            printf("#ERROR: Unrecognized file type %s!\n", heightMapFile.substr(heightMapFile.find_last_of('.')).c_str());

           ofExit();
        }
    }
    else
    {
        //Generate random heightmap
        if (!LoadHeightMapFromPerlinNoise(1024, 1024, 250.f, 0.5f))
        {
            ofExit();
        }
    }

    // //Compute terrain
    // if (!gTerrain->ComputeTerrain(gWaterLevel, gTerrainHeight, terrainVbo))
    {
        // ofExit();
    }


    // terrainVbo.setVertexData(heightMap, 3, mapWidth*mapHeight, GL_DYNAMIC_DRAW);

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.addVertices(heightMap);

    for (int i = 0; i < mapHeight*mapWidth; i++)
    {
        float depth = mesh.getVertex(i).z;
        uint8_t col = depth*2.5;
        mesh.addColor(ofColor(col, col, col, 255));
    }

    for (int y = 0; y<mapHeight-1; y++){
       for (int x=0; x<mapWidth-1; x++){
            mesh.addIndex(x+y*mapWidth);               // 0
            mesh.addIndex((x+1)+y*mapWidth);           // 1
            mesh.addIndex(x+(y+1)*mapWidth);           // 10

            mesh.addIndex((x+1)+y*mapWidth);           // 1
            mesh.addIndex((x+1)+(y+1)*mapWidth);       // 11
            mesh.addIndex(x+(y+1)*mapWidth);           // 10
        }
    }

    terrainVbo.setMesh(mesh, GL_DYNAMIC_DRAW);

    // for (int i = 0; i < mapHeight*mapWidth; i++)
    // {
    //     std::cout << i << ": " << mesh.getVertex(i) << std::endl;
    // }

    ofEnableDepthTest();

    camera.move(mapWidth/2, 0, 10);
    camera.tilt(60);
    // //Create sky
    // GLfloat* skyVertices = new GLfloat[8 * 3]
    // {
    //     -1, 1, 1,
    //     1, 1, 1,
    //     -1, 1, -1,
    //     1, 1, -1,

    //     -1, -1, 1,
    //     1, -1, 1,
    //     -1, -1, -1,
    //     1, -1, -1,
    // };

    // GLuint* skyIndices = new GLuint[36]
    // {
    //     0, 1, 2,
    //     2, 1, 3,

    //     4, 5, 6,
    //     6, 5, 7,

    //     0, 1, 4,
    //     4, 1, 5,

    //     2, 3, 6,
    //     6, 3, 7,

    //     1, 3, 5,
    //     5, 3, 7,

    //     0, 2, 4,
    //     4, 2, 6
    // };
}

//--------------------------------------------------------------
void ofApp::update(){
    angle += 0.5;
    // vidGrabber.update();
    // fingerMovie.update();
        
    // // This just 
    // maskFbo.begin();
    // ofClear(255, 0, 0,255);
    // multimaskImg.draw( mouseX-multimaskImg.getWidth()*0.5, 0 );
    // maskFbo.end();
    
    // GLint uniformLight = shader.getUniformLocation("LightDir");
    shader.setUniform3fv("LightDir", (float *)&sunDir);
    shader.setUniform1f("MaxZ", (1.0f - waterLevel) * terrainHeight);

//      GLuint uniformView = gShader->GetUniformLocation("View");
//      glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(viewMatrix));

    // // MULTITEXTURE MIXING FBO
    // //
    // fbo.begin();
    // ofClear(0, 0, 0,255);
    // shader.begin();
    // Pass the video texture
    // shader.setUniformTexture("tex0", vidGrabber.getTexture() , 1 );
    // // Pass the image texture
    // shader.setUniformTexture("tex0", logoImg, 1);
    // // Pass the movie texture
    // shader.setUniformTexture("tex2", fingerMovie.getTexture() , 3 );
    // // Pass the mask texture
    // shader.setUniformTexture("maskTex", maskFbo.getTexture() , 4 );
    
    // // We are using this image just as a frame where the pixels can be arrange
    // // this could be a mesh also. 
    // // Comment "shader.setUniformTexture("maskTex", maskFbo.getTexture() , 4 );" to se how there is two ways
    // // of passing a texture to the shader
    // // 
    // maskFbo.draw(0,0);
    
    // mesh.drawWireframe();

    // shader.end();
    // fbo.end();

    if (move_forward || move_back)
    {
        ofVec3f look = camera.getLookAtDir();
        look.z = 0;

        look *= (move_back ? -1 : 1)*10;

        camera.move(look);
    }
    if (move_left)
    {
        camera.truck(-10);
    }
    if (move_right)
    {
        camera.truck(10);
    }
    if (move_up)
    {
        camera.boom(10);
    }
    if (move_down)
    {
        camera.boom(-10);
    }
    
    ofSetWindowTitle( ofToString( ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::gray);
    
    // // draw everything
    // ofSetColor(255);
    // vidGrabber.draw(5,5,320,240);
    // ofSetColor(ofColor::red);
    // ofDrawBitmapString("RED", 5+30, 5+30);
    
    // ofSetColor(255);
    // logoImg.draw(0, 0);
    // ofSetColor(ofColor::green);
    // ofDrawBitmapString("GREEN", 320+10+30,5+30);

    
    // ofSetColor(255);
    // fingerMovie.draw(320*2+15,5,320,240);
    // ofSetColor(ofColor::blue);
    // ofDrawBitmapString("BLUE", 320*2+5+30,5+30);
    
    
    // ofSetColor(255);
    // maskFbo.draw(320+10,240+10,320,240);
    // ofDrawBitmapString("RGB MASK", 320+10+30,240+10+30);
    
    // fbo.draw(320+10,240*2+15,320,240);
    // ofDrawBitmapString("Final FBO", 320+10+30,240*2+15+30);
    // fbo.draw(0, 0);

    // shader.begin();
    camera.begin();
    // ofScale(2, -2, 2); // flip the y axis and zoom in a bit
    // ofRotateZ(angle);
    // ofRotateY(90);
    // ofTranslate(-mapWidth / 2, -mapHeight / 2);

    // camera.roll(1);
    // std::cout << "Camera: " << camera.getPosition() << std::endl;
    
    // mesh.draw();
    terrainVbo.drawElements(GL_TRIANGLES, terrainVbo.getNumIndices());
    camera.end();
    // shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key)
    {
        case 'w':
        case OF_KEY_UP:
            move_forward = true;
            break;
        case 's':
        case OF_KEY_DOWN:
            move_back = true;
            break;
        case 'a':
        case OF_KEY_LEFT:
            move_left = true;
            break;
        case 'd':
        case OF_KEY_RIGHT:
            move_right = true;
            break;
        case 'e':
            move_up = true;
            break;
        case 'f':
            move_down = true;
            break;
        default:
            break;
    }\
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key)
    {
        case 'w':
        case OF_KEY_UP:
            move_forward = false;
            break;
        case 's':
        case OF_KEY_DOWN:
            move_back = false;
            break;
        case 'a':
        case OF_KEY_LEFT:
            move_left = false;
            break;
        case 'd':
        case OF_KEY_RIGHT:
            move_right = false;
            break;
        case 'e':
            move_up = false;
            break;
        case 'f':
            move_down = false;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

GLboolean ofApp::LoadHeightMap(const char* path)
{
    printf("#Info: Attempting to open file %s\n", path);
    ifstream file(path, ios::in);

    //Check if successful
    if (file.good())
    {
        //Read magic number
        string magic;
        file >> magic;

        //Check if correct magic number
        if (magic != "P2")
        {
            printf("#ERROR: %s is not a valid pnm file!\n", path);

            return GL_FALSE;
        }

        //Read width, height and max value
        GLuint maxVal;
        file    >> this->mapWidth
                >> this->mapHeight
                >> maxVal;

        this->maxHeight = 0.0f;

        if (maxVal > 65535)
        {
            printf("#ERROR: Invalid max value in header!\n");

            return GL_FALSE;
        }

        //Prepare buffer
        // if (this->heightMap) delete[] this->heightMap;
        // this->heightMap = new ofVec3f[this->mapWidth * this->mapHeight];
        heightMap.clear();
        // if (!this->heightMap)
        {
            // printf("#ERROR: Failed to allocate memory for height map!\n");

            // return GL_FALSE;
        }

        //Read values
        for (GLuint y = 0; y < this->mapHeight; y++)
        {
            for (GLuint x = 0 ; x < this->mapWidth; x++)
            {
                //Read value
                GLuint val;
                file >> val;
                if (val > this->maxHeight) this->maxHeight = val;

                //Insert into height map
                // heightMap[y * this->mapWidth + x] = ofVec3f(x, y, val);
                heightMap.push_back(ofVec3f(x, y, val));
            }
        }

        // this->fullUpdate = GL_TRUE;

        printf("#Info: Loaded successfully!\n");

        return GL_TRUE;
    }
    else
    {
        printf("#ERROR: Unable to open file %s!\n", path);

        return GL_FALSE;
    }
}

GLboolean ofApp::LoadHeightMapFromPerlinNoise(GLuint width, GLuint height, GLfloat zoom, GLfloat p)
{
    printf("#Info: Generating random height map\n");

    this->mapWidth  = width;
    this->mapHeight = height;
    this->maxHeight = 0.0f;

    //Prepare buffer
    // if (this->heightMap) delete[] this->heightMap;
    // this->heightMap = new GLfloat[this->mapWidth * this->mapHeight];
    heightMap.clear();
    // if (!this->heightMap)
    // {
        // printf("#ERROR: Failed to allocate memory for height map!\n");

        // return GL_FALSE;
    // }

    //Generate values
    for (GLuint y = 0; y < this->mapHeight; y++)
    {
        for (GLuint x = 0; x < this->mapWidth; x++)
        {
            GLfloat height = PerlinNoise(x, y, zoom, p);
            if (height > this->maxHeight) this->maxHeight = height;

            // this->heightMap[y * this->mapWidth + x] = height;
            heightMap.push_back(ofVec3f(x, y, height*100));
        }
    }

    // this->fullUpdate = GL_TRUE;

    printf("#Info: Generated map successfully!\n");

    return GL_TRUE;
}

