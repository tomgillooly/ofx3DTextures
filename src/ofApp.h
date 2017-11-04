#pragma once

#define WEBCAM

#include "ofMain.h"

#include "ofxVolumetrics.h"

// #include "Terrain.h"
#include <vector>
#include "Common.h"

class ofApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    GLboolean LoadHeightMap(const char* path);
    GLboolean LoadHeightMapFromPerlinNoise(GLuint width, GLuint height, GLfloat zoom, GLfloat p);

    std::vector<ofVec3f>    heightMap;
    GLuint      mapWidth;
    GLuint      mapHeight;
    GLfloat     maxHeight;

	
    ofImage     logoImg;
    // ofImage     multimaskImg;
    // ofVideoPlayer 		fingerMovie;
    // ofVideoGrabber 		vidGrabber;

    ofEasyCam                   camera;
    ofMesh                      mesh;
    
    ofxImageSequencePlayer imageSequence;
    ofxTexture3d colourTexture;
    std::vector<ofVec3f> tex_coord_buffer;
    ofTexture texture;

    ofVbo           terrainVbo;
    ofFbo       fbo;
    // ofFbo       maskFbo;
    // ofCamera cam;
    float angle;
    ofShader    shader;
    // ofShader    sky_shader;

    string                      heightMapFile;

    // Terrain*                    gTerrain;
    GLuint                      gSkyVertices;
    GLuint                      gSkyIndices;
    GLfloat                     waterChange;
    GLfloat                     waterLevel;
    GLfloat                     terrainHeight;

    GLfloat                     sunApex;
    glm::vec2                   sunPos;
    glm::vec3                   sunDir;

    bool move_forward;
    bool move_back;
    bool move_left;
    bool move_right;
    bool move_up;
    bool move_down;
};
