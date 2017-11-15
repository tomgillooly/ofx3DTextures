#include "ofApp.h"

#include "Perlin.h"

#include <iostream>

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
  
    shader.load("shaders/shader");

    ofDisableArbTex();

    imageSequence.init("colour_texture_split/colour_text",3,".tif", 0);
    int volWidth = imageSequence.getWidth();
    int volHeight = imageSequence.getHeight();
    int volDepth = imageSequence.getSequenceLength();

    cout << "setting up volume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth <<"\n";

    unsigned char *volumeData = new unsigned char[volWidth*volHeight*volDepth*4];

    // Read image slices in as 3D texture data    
    for(int z=0; z<volDepth; z++)
    {
        imageSequence.loadFrame(z);

        for(int x=0; x<volWidth; x++)
        {
            for(int y=0; y<volHeight; y++)
            {
                // convert from greyscale to RGBA, false color
                int i4 = ((x+volWidth*y)+z*volWidth*volHeight)*4;
                ofColor c = imageSequence.getPixels().getColor(x, y);

                volumeData[i4] = c.r;
                volumeData[i4+1] = c.g;
                volumeData[i4+2] = c.b;
                volumeData[i4+3] = 255;
            }
        }
    }

    colourTexture.allocate(volWidth, volHeight, volDepth, GL_RGBA);
    colourTexture.loadData(volumeData, volWidth, volHeight, volDepth, 0, 0, 0, GL_RGBA);
    
    // Bind the 3D texture and set the sampler variable in the fragment shader
    glActiveTexture(GL_TEXTURE1);
    ofxTextureData3d texture_data = colourTexture.getTextureData();
    if (!ofIsGLProgrammableRenderer()){
        glEnable(texture_data.textureTarget);
        glBindTexture(texture_data.textureTarget, texture_data.textureID);

        glDisable(texture_data.textureTarget);
    } else {
        glBindTexture(texture_data.textureTarget, texture_data.textureID);

    }
    shader.setUniform1i("myTexture", 1);
    glActiveTexture(GL_TEXTURE0);

    //Generate random heightmap
    if (!LoadHeightMapFromPerlinNoise(1024, 1024, 250.f, 0.5f))
    {
        ofExit();
    }

    shader.setUniform1f("maxHeight", maxHeight);
    
    // Make a texture out of the depth map
    depthImage.allocate(mapWidth, mapHeight);
    depthImage.setFromPixels(&heightMap[0], mapWidth, mapHeight);
    depthImage.setNativeScale(0, maxHeight);        // This needs to be here, at least for the image save step
    depthImage.updateTexture();


    // Save the generated depth map as an image so we can check it worked
    std::string DepthOutName = "DebugFiles/RawDepthImg.png";
    ofxCvFloatImage temp;
    temp.setFromPixels(depthImage.getFloatPixelsRef().getData(), mapWidth, mapHeight);
    temp.setNativeScale(depthImage.getNativeScaleMin(), depthImage.getNativeScaleMax());
    temp.convertToRange(0, 1);
    ofxCvGrayscaleImage temp2;
    temp2.setFromPixels(temp.getFloatPixelsRef());
    ofSaveImage(temp2.getPixels(), DepthOutName);

    // Set our mesh vertices
    std::vector<ofIndexType> index_buffer;
    std::vector<ofVec3f> vertex_buffer;
    std::vector<ofVec3f> normal_buffer;

    for (int y = 0; y<mapHeight-1; y++){
       for (int x=0; x<mapWidth-1; x++){
            vertex_buffer.push_back(ofVec3f(x, y, 0));

            index_buffer.push_back(x+y*mapWidth);               // 0
            index_buffer.push_back((x+1)+y*mapWidth);           // 1
            index_buffer.push_back(x+(y+1)*mapWidth);           // 10

            index_buffer.push_back((x+1)+y*mapWidth);           // 1
            index_buffer.push_back((x+1)+(y+1)*mapWidth);       // 11
            index_buffer.push_back(x+(y+1)*mapWidth);           // 10
        }
    }

    for (int y = 0; y<mapHeight; y++){
       for (int x = 0; x<mapWidth; x++){
            tex_coord_buffer.push_back(
                ofVec2f(
                    (1.0*x) / (mapWidth-1),
                    (1.0*y) / (mapHeight-1))
                    // x, y)                    // Magic sand just uses x and y. Why does it work?
                );                
        }
    }

    // If we want to use 3D vertices, draw with the terrainVbo instead
    // terrainVbo.setVertexData(&vertex_buffer[0], vertex_buffer.size(), GL_STATIC_DRAW);
    // terrainVbo.setIndexData(&index_buffer[0], index_buffer.size(), GL_STATIC_DRAW);
    // terrainVbo.setNormalData(&normal_buffer[0], normal_buffer.size(), GL_STATIC_DRAW);
    // terrainVbo.setTexCoordData(&tex_coord_buffer[0], tex_coord_buffer.size(), GL_DYNAMIC_DRAW);
    
    mesh.addVertices(vertex_buffer);
    mesh.addIndices(index_buffer);
    mesh.addTexCoords(tex_coord_buffer);

    camera.move(mapWidth/2, 0, 10);
    camera.tilt(60);
}

//--------------------------------------------------------------
void ofApp::update(){
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
    ofBackground(ofColor::black);
    
    
    // depthImage.getTexture().bind();      // Magic Sand just does this and it binds to tex0 - doesn't work for me though
    shader.begin();
    shader.setUniformTexture("depthTexture",depthImage.getTexture(), 2);    // Fails when bound to 0
    camera.begin();
    

    // We need to do this for the 3D texture coordinates for some reason
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // glTexCoordPointer(3, GL_FLOAT, sizeof(ofVec3f), &tex_coord_buffer[0]);
    
    mesh.draw();
    // terrainVbo.drawElements(GL_TRIANGLES, terrainVbo.getNumIndices());
    
    // if(terrainVbo.getTexCoordBuffer().size() && terrainVbo.getUsingTexCoords()){
    //     glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    // }
    
    camera.end();
    shader.end();
    // depthImage.getTexture().unbind();
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
    int ind = y * mapWidth + x;
    if (ind >= 0 && ind < depthImage.getFloatPixelsRef().getTotalBytes())
    {
        float z = depthImage.getFloatPixelsRef().getData()[ind];
        std::cout << "Kinect depth (x, y, z) = (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
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
        heightMap.clear();

        //Read values
        for (GLuint y = 0; y < this->mapHeight; y++)
        {
            for (GLuint x = 0 ; x < this->mapWidth; x++)
            {
                //Read value
                GLuint val;
                file >> val;
                if (val > this->maxHeight) this->maxHeight = val;

                // Insert into height map
                heightMap.push_back(static_cast<float>(val));
            }
        }

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
    heightMap.clear();
    
    //Generate values
    for (GLuint y = 0; y < this->mapHeight; y++)
    {
        for (GLuint x = 0; x < this->mapWidth; x++)
        {
            GLfloat height = PerlinNoise(x, y, zoom, p)*100;
            if (height > this->maxHeight) this->maxHeight = height;

            heightMap.push_back(static_cast<float>(height));
        }
    }

    printf("#Info: Generated map successfully!\n");

    return GL_TRUE;
}

