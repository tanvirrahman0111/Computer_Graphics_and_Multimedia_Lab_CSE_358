/*
 [1] Getting Started / Creating a Window  → GLFW + GLAD
 [2] Getting Started / Hello Triangle     → VAO, VBO
 [3] Getting Started / Shaders            → compile/link, uniforms
 [4] Getting Started / Textures           → procedural tex, glTexImage2D
 [5] Getting Started / Transformations    → glm::translate/scale/rotate
 [6] Getting Started / Coordinate Systems → glm::ortho projection
 [7] Advanced OpenGL / Blending           → transparent overlay
*/

/*
Team Members: 
1. Name: Md. Tanvir Rahman 
ID: 0432320005101116
2. Name: Faisal Ahmed
ID: 0432320005101129
3. Name: Md. Sayed
ID: 0432320005101148
*/

#include "glad.h"
#include "glfw3.h"

// [5][6] GLM  (learnopengl.com/Getting-started/Transformations)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

// ─── Window ──────────────────────────────────────────────────
const unsigned int SCR_WIDTH  = 500;
const unsigned int SCR_HEIGHT = 700;

// ─────────────────────────────────────────────────────────────
//  [6] Coordinate Systems — glm::ortho
//  learnopengl.com/Getting-started/Coordinate-Systems
//  pixel coords: (0,0) bottom-left, (500,700) top-right
// ─────────────────────────────────────────────────────────────
glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH,
                                   0.0f, (float)SCR_HEIGHT);

// ─── Game State ──────────────────────────────────────────────
enum GameState { MENU, PLAYING, PAUSED, GAME_OVER };
GameState gameState = MENU;

bool keyLeft  = false;
bool keyRight = false;

// ─── Player (pixel coords) ───────────────────────────────────
float playerX  = 250.0f;
float playerY  = 80.0f;
const float PW = 38.0f;
const float PH = 62.0f;

// ─── Enemy ───────────────────────────────────────────────────
struct Enemy { float x, y, speed, r, g, b; };
std::vector<Enemy> enemies;
float baseSpeed     = 3.5f;
float spawnTimer    = 0.0f;
float spawnInterval = 120.0f;
float laneXPos[3]   = {162.0f, 250.0f, 338.0f};

// ─── Lane marks ──────────────────────────────────────────────
float laneY[6]  = {0, 116, 232, 348, 464, 580};
float laneSpeed = 6.5f;

// ─── Score ───────────────────────────────────────────────────
int   score = 0, highScore = 0;
float scoreTimer = 0.0f;

// ─── Timing ──────────────────────────────────────────────────
double lastTime = 0, deltaTime = 0, now = 0;

// ─────────────────────────────────────────────────────────────
//  [3] Shaders  (learnopengl.com/Getting-started/Shaders)
//  sprite shader — handle flat colour and texture
//  uModel (mat4)      → [5] Transformations
//  uProjection (mat4) → [6] Coordinate Systems
//  uUseTexture (bool) → texture on/off toggle
// ─────────────────────────────────────────────────────────────
const char* spriteVert = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 uModel;
uniform mat4 uProjection;

void main(){
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* spriteFrag = R"(
#version 330 core
in  vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4      uColor;
uniform bool      uUseTexture;

void main(){
    if(uUseTexture)
        FragColor = texture(uTexture, TexCoord) * uColor;
    else
        FragColor = uColor;
}
)";

// ─── GL objects ──────────────────────────────────────────────
unsigned int spriteShader;
unsigned int quadVAO, quadVBO;

// Uniform locations
int uModel_loc, uProj_loc, uColor_loc, uUseTexture_loc;

// [4] Textures
unsigned int carPlayerTex, carEnemyTex;

// ─────────────────────────────────────────────────────────────
//  [3] Shader compile + link
//  learnopengl.com/Getting-started/Shaders
// ─────────────────────────────────────────────────────────────
unsigned int compileShader(GLenum type, const char* src){
    unsigned int s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    int ok; char log[512];
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if(!ok){ glGetShaderInfoLog(s, 512, NULL, log); std::cout << "Shader: " << log << "\n"; }
    return s;
}

unsigned int linkProgram(const char* vs, const char* fs){
    unsigned int v = compileShader(GL_VERTEX_SHADER,   vs);
    unsigned int f = compileShader(GL_FRAGMENT_SHADER, fs);
    unsigned int p = glCreateProgram();
    glAttachShader(p, v); glAttachShader(p, f);
    glLinkProgram(p);
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

//  [2] VAO / VBO  (learnopengl.com/Getting-started/Hello-Triangle)
//  Unit quad: (0,0) → (1,1)
//  Vertex layout: pos(x,y) + texcoord(u,v) — 4 floats per vertex

void buildGeometry(){
    float verts[] = {
        // pos    // texcoord
        0,0,      0,0,
        1,0,      1,0,
        1,1,      1,1,
        0,0,      0,0,
        1,1,      1,1,
        0,1,      0,1,
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    // position  (location = 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texcoord  (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
}

//  [4] Procedural car texture
//  learnopengl.com/Getting-started/Textures

unsigned int makeCarTexture(bool isPlayer){
    const int W = 16, H = 24;
    unsigned char px[H][W][4];

    unsigned char br = isPlayer ? 26  : 200;
    unsigned char bg = isPlayer ? 90  : 30;
    unsigned char bb = isPlayer ? 235 : 30;

    for(int y = 0; y < H; y++) for(int x = 0; x < W; x++){
        unsigned char r = br, g = bg, b = bb, a = 255;

        // Wheels
        bool lw = (x < 4), rw = (x >= 12);
        bool tw = (y >= H-5), bw = (y < 5);
        if((lw || rw) && (tw || bw)){ r=20; g=20; b=20; }

        // Window Screen
        if(x >= 4 && x < 12 && y >= H-10 && y < H-5){ r=140; g=210; b=255; }

        // Player → Yellow headlights (front), Enemy → Red brake lights (rear)
        if( isPlayer && (x < 3 || x >= 13) && y >= H-3){ r=255; g=255; b=100; }
        if(!isPlayer && (x < 3 || x >= 13) && y < 3)   { r=255; g=40;  b=40;  }

        px[y][x][0]=r; px[y][x][1]=g; px[y][x][2]=b; px[y][x][3]=a;
    }

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    // [4] pixel data → GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}


//  [5] Draw sprite  (learnopengl.com/Getting-started/Transformations)
//  glm::translate + glm::scale + glm::rotate → mat4 → GPU uniform

void drawSprite(float cx, float cy, float hw, float hh,
                glm::vec4 color, unsigned int tex = 0, float angleDeg = 0.0f)
{
    // [5] create model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(cx - hw, cy - hh, 0.0f));
    if(angleDeg != 0.0f){
        model = glm::translate(model, glm::vec3(hw, hh, 0.0f));
        model = glm::rotate(model, glm::radians(angleDeg), glm::vec3(0, 0, 1));
        model = glm::translate(model, glm::vec3(-hw, -hh, 0.0f));
    }
    model = glm::scale(model, glm::vec3(hw*2.0f, hh*2.0f, 1.0f));

    glUniformMatrix4fv(uModel_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(uColor_loc, color.r, color.g, color.b, color.a);

    if(tex){
        glUniform1i(uUseTexture_loc, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
    } else {
        glUniform1i(uUseTexture_loc, 0);
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    if(tex) glUniform1i(uUseTexture_loc, 0);
}

// flat colour shorthand
void drawRect(float cx, float cy, float hw, float hh,
              float r, float g, float b, float a = 1.0f){
    drawSprite(cx, cy, hw, hh, {r, g, b, a});
}

// textured car  ([4] Textures + [5] Transformations)
void drawCar(float cx, float cy, float hw, float hh,
             unsigned int tex, float angleDeg = 0.0f){
    drawSprite(cx, cy, hw, hh, {1,1,1,1}, tex, angleDeg);
}

// ─────────────────────────────────────────────────────────────
//  7-segment digit rendering (no font needed)
//  Each number is drawn using small drawRect blocks
// ─────────────────────────────────────────────────────────────
void drawDigit(int d, float ox, float oy, float sz,
               float r, float g, float b)
{
    bool s[7] = {};
    switch(d){
        case 0: s[0]=s[1]=s[2]=s[4]=s[5]=s[6]=1; break;
        case 1: s[2]=s[5]=1; break;
        case 2: s[0]=s[2]=s[3]=s[4]=s[6]=1; break;
        case 3: s[0]=s[2]=s[3]=s[5]=s[6]=1; break;
        case 4: s[1]=s[2]=s[3]=s[5]=1; break;
        case 5: s[0]=s[1]=s[3]=s[5]=s[6]=1; break;
        case 6: s[0]=s[1]=s[3]=s[4]=s[5]=s[6]=1; break;
        case 7: s[0]=s[2]=s[5]=1; break;
        case 8: s[0]=s[1]=s[2]=s[3]=s[4]=s[5]=s[6]=1; break;
        case 9: s[0]=s[1]=s[2]=s[3]=s[5]=s[6]=1; break;
        default: return;
    }
    float w=sz*.5f, h=sz, th=sz*.10f, tw=sz*.08f;
    if(s[0]) drawRect(ox,       oy+h,          w,  th,          r,g,b);
    if(s[1]) drawRect(ox-w+tw,  oy+h*.5f,      tw, h*.5f-th,   r,g,b);
    if(s[2]) drawRect(ox+w-tw,  oy+h*.5f,      tw, h*.5f-th,   r,g,b);
    if(s[3]) drawRect(ox,       oy,            w,  th,          r,g,b);
    if(s[4]) drawRect(ox-w+tw,  oy-h*.5f+th,   tw, h*.5f-th,   r,g,b);
    if(s[5]) drawRect(ox+w-tw,  oy-h*.5f+th,   tw, h*.5f-th,   r,g,b);
    if(s[6]) drawRect(ox,       oy-h,          w,  th,          r,g,b);
}

void drawNumber(int num, float ox, float oy, float sz,
                float r, float g, float b)
{
    std::string s = std::to_string(num);
    float step = sz * 1.3f;
    float startX = ox - (s.size()-1) * step * 0.5f;
    for(int i = 0; i < (int)s.size(); i++)
        drawDigit(s[i]-'0', startX + i*step, oy, sz, r, g, b);
}

// ─────────────────────────────────────────────────────────────
//  Road scene
// ─────────────────────────────────────────────────────────────
void drawRoad(){
    float W = SCR_WIDTH, H = SCR_HEIGHT;
    // Sky
    drawRect(W*.5f, H*.72f,  W*.5f,  H*.28f, 0.05f,0.07f,0.18f);
  // Grass on the left
    drawRect(W*.11f, H*.5f,  W*.11f, H*.5f,  0.10f,0.48f,0.10f);
    // Grass on the right
    drawRect(W*.89f, H*.5f,  W*.11f, H*.5f,  0.10f,0.48f,0.10f);
    // Road
    drawRect(W*.5f,  H*.5f,  W*.38f, H*.5f,  0.20f,0.20f,0.20f);
  // White border
    drawRect(W*.24f, H*.5f,  6,      H*.5f,  1,1,1);
    drawRect(W*.76f, H*.5f,  6,      H*.5f,  1,1,1);
    // Scrolling lane markings
    for(int i = 0; i < 6; i++)
        drawRect(W*.5f, laneY[i]+20, 4, 20, 1,0.85f,0);
    // Tree
    auto tree = [&](float tx, float ty){
        drawRect(tx, ty,     5,  20, 0.32f,0.18f,0.05f);
        drawRect(tx, ty+35, 20,  22, 0.05f,0.38f,0.05f);
    };
    tree(55,130); tree(55,360); tree(55,560);
    tree(445,200); tree(445,420); tree(445,610);
}

// ─────────────────────────────────────────────────────────────
//  [7] Blending — dark overlay
//  learnopengl.com/Advanced-OpenGL/Blending
// ─────────────────────────────────────────────────────────────
void drawOverlay(float alpha){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawRect(SCR_WIDTH*.5f, SCR_HEIGHT*.5f,
             SCR_WIDTH*.5f, SCR_HEIGHT*.5f, 0,0,0, alpha);
    glDisable(GL_BLEND);
}

// ─────────────────────────────────────────────────────────────
//  HUD — 7-segment score + speed bar
// ─────────────────────────────────────────────────────────────
void drawHUD(){
    float W = SCR_WIDTH, H = SCR_HEIGHT;
    // HUD bar
    drawRect(W*.5f, H-28, W*.5f, 28, 0.05f,0.05f,0.05f);

    // Speed bar
    float spd = (baseSpeed-3.5f)/12.0f;
    if(spd > 1) spd = 1;
    drawRect(W*.5f, H-18, 55, 7, 0.25f,0.25f,0.25f);
    if(spd > 0)
        drawRect(W*.5f - 55 + 55*spd*2, H-18, 55*spd, 7,
                 0.2f+spd*.8f, 1.0f-spd, 0.0f);

    // Score (left) + High Score (right) — 7-segment
    drawNumber(score,     155, H-20, 13, 1.0f,0.9f,0.2f);
    drawNumber(highScore, 345, H-20, 13, 0.3f,1.0f,0.4f);
}

// ─────────────────────────────────────────────────────────────
//  Game logic
// ─────────────────────────────────────────────────────────────
bool checkCollision(const Enemy& e){
    return fabsf(playerX-e.x) < PW+26 && fabsf(playerY-e.y) < PH+45;
}

void spawnEnemy(){
    Enemy e;
    e.x     = laneXPos[rand()%3];
    e.y     = SCR_HEIGHT + 80.0f;
    e.speed = baseSpeed + (rand()%8)*0.3f;
    e.r = 0.5f+(rand()%5)*.10f;
    e.g = 0.05f+(rand()%4)*.08f;
    e.b = 0.05f+(rand()%4)*.08f;
    enemies.push_back(e);
}

void resetGame(){
    playerX = SCR_WIDTH*.5f;
    enemies.clear();
    score=0; scoreTimer=0;
    baseSpeed=3.5f; laneSpeed=6.5f;
    spawnTimer=0; spawnInterval=120;
    keyLeft=keyRight=false;
}

// ─────────────────────────────────────────────────────────────
//  Callbacks
// ─────────────────────────────────────────────────────────────
void keyCallback(GLFWwindow* win, int key, int, int action, int){
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
    if(key==GLFW_KEY_LEFT)  keyLeft  = (action != GLFW_RELEASE);
    if(key==GLFW_KEY_RIGHT) keyRight = (action != GLFW_RELEASE);
    if(key==GLFW_KEY_SPACE && action==GLFW_PRESS)
        if(gameState==MENU || gameState==GAME_OVER)
            { resetGame(); gameState=PLAYING; }
    if(key==GLFW_KEY_P && action==GLFW_PRESS){
        if(gameState==PLAYING)     gameState=PAUSED;
        else if(gameState==PAUSED) gameState=PLAYING;
    }
}

void framebuffer_size_callback(GLFWwindow*, int w, int h){
    glViewport(0, 0, w, h);
}

// ─────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────
int main(){
    srand((unsigned)time(nullptr));

    // ── [1] Creating a Window ────────────────────────────────
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Obstacle Dodge Drive", NULL, NULL);
    if(!window){ std::cout<<"Window failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyCallback);

    // ── [1] GLAD ─────────────────────────────────────────────
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"GLAD failed\n"; return -1;
    }

    // ── [7] Global blend state ───────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ── [3] Shaders ──────────────────────────────────────────
    spriteShader    = linkProgram(spriteVert, spriteFrag);
    glUseProgram(spriteShader);
    uModel_loc      = glGetUniformLocation(spriteShader, "uModel");
    uProj_loc       = glGetUniformLocation(spriteShader, "uProjection");
    uColor_loc      = glGetUniformLocation(spriteShader, "uColor");
    uUseTexture_loc = glGetUniformLocation(spriteShader, "uUseTexture");
    glUniform1i(glGetUniformLocation(spriteShader,"uTexture"), 0);

    // ── [6] Send ortho projection once ───────────────────────
    glUniformMatrix4fv(uProj_loc, 1, GL_FALSE, glm::value_ptr(projection));

    // ── [2] Build geometry ───────────────────────────────────
    buildGeometry();

    // ── [4] Procedural textures ──────────────────────────────
    carPlayerTex = makeCarTexture(true);
    carEnemyTex  = makeCarTexture(false);

    lastTime = glfwGetTime();

    // ─────────────────────────────────────────────────────────
    //  Render Loop
    // ─────────────────────────────────────────────────────────
    while(!glfwWindowShouldClose(window)){
        now       = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime  = now;

        glfwPollEvents();

        // ── Update ───────────────────────────────────────────
        if(gameState == PLAYING){
            float dt60 = (float)(deltaTime * 60.0);

            if(keyLeft  && playerX > 138) playerX -= 11*dt60;
            if(keyRight && playerX < 362) playerX += 11*dt60;

            for(int i = 0; i < 6; i++){
                laneY[i] -= laneSpeed * dt60;
                if(laneY[i] < -40) laneY[i] += 740;
            }

            spawnTimer += dt60;
            if(spawnTimer >= spawnInterval){
                spawnEnemy();
                spawnTimer = 0;
                if(spawnInterval > 45) spawnInterval -= 0.4f;
            }

            for(int i = (int)enemies.size()-1; i >= 0; i--){
                enemies[i].y -= enemies[i].speed * dt60;
                if(checkCollision(enemies[i])){
                    gameState = GAME_OVER;
                    if(score > highScore) highScore = score;
                }
                if(enemies[i].y < -100)
                    enemies.erase(enemies.begin()+i);
            }

            scoreTimer += dt60;
            if(scoreTimer >= 30){
                score++;
                scoreTimer = 0;
                baseSpeed = 3.5f + score*0.065f;
                laneSpeed = 6.5f + score*0.11f;
                if(laneSpeed > 28) laneSpeed = 28;
            }
        }

        // ── Draw ─────────────────────────────────────────────
        glClearColor(0.05f,0.07f,0.18f,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(spriteShader);

        // ── MENU ─────────────────────────────────────────────
        if(gameState == MENU){
            drawRoad();
            drawOverlay(0.55f);

            // Title box
            drawRect(SCR_WIDTH*.5f, SCR_HEIGHT*.60f, 200,52, 0.05f,0.05f,0.28f);
            // Decorative cars
            drawCar(145, SCR_HEIGHT*.56f, 34,58, carPlayerTex);
            drawCar(250, SCR_HEIGHT*.62f, 34,58, carEnemyTex);
            drawCar(355, SCR_HEIGHT*.56f, 34,58, carPlayerTex);

            // "CAR RACING" — 7-segment 
            
            drawNumber(8888, SCR_WIDTH*.5f, SCR_HEIGHT*.615f, 18,
                       1.0f,0.9f,0.2f);   // placeholder decoration

            // High score box
            drawRect(SCR_WIDTH*.5f, SCR_HEIGHT*.40f, 140,36, 0.08f,0.08f,0.20f);
            drawNumber(highScore, SCR_WIDTH*.5f, SCR_HEIGHT*.40f, 14,
                       0.4f,1.0f,0.4f);

            // Blinking SPACE indicator 
            float blink = (float)(sin(now*3)*.5+.5);
            drawRect(SCR_WIDTH*.5f-40, SCR_HEIGHT*.24f, 35, 8,
                     blink, blink*.8f, 0, blink);
            drawRect(SCR_WIDTH*.5f+40, SCR_HEIGHT*.24f, 35, 8,
                     blink, blink*.8f, 0, blink);

            // Controls hint — small rect arrow
            drawRect(SCR_WIDTH*.5f-80, SCR_HEIGHT*.14f, 18,8, 0.7f,0.7f,0.7f);
            drawRect(SCR_WIDTH*.5f+80, SCR_HEIGHT*.14f, 18,8, 0.7f,0.7f,0.7f);

        // ── PLAYING / PAUSED / GAME_OVER ─────────────────────
        } else {
            drawRoad();

            // Enemy cars  [4] texture
            for(auto& e : enemies)
                drawCar(e.x, e.y, 30, 52, carEnemyTex);

            // Player car  [4] texture + [5] transform
            drawCar(playerX, playerY, PW, PH, carPlayerTex);

            drawHUD();

            // ── PAUSED overlay ───────────────────────────────
            if(gameState == PAUSED){
                drawOverlay(0.50f);
                // Pause box
                drawRect(SCR_WIDTH*.5f, SCR_HEIGHT*.52f, 160,60,
                         0.05f,0.05f,0.25f);
                // Two pause bars
                drawRect(SCR_WIDTH*.5f-22, SCR_HEIGHT*.52f, 14,35, 0.8f,0.8f,1.0f);
                drawRect(SCR_WIDTH*.5f+22, SCR_HEIGHT*.52f, 14,35, 0.8f,0.8f,1.0f);
                // P indicator
                drawRect(SCR_WIDTH*.5f, SCR_HEIGHT*.40f, 90,10, 0.3f,0.3f,0.6f);
            }

            // ── GAME OVER overlay ────────────────────────────
            if(gameState == GAME_OVER){
                drawOverlay(0.65f);
                drawRect(SCR_WIDTH*.5f, SCR_HEIGHT*.53f, 200,125,
                         0.18f,0.02f,0.02f);

                // Crashed car — [5] tilted using glm::rotate
                drawCar(SCR_WIDTH*.5f, SCR_HEIGHT*.55f, 44,70,
                        carPlayerTex, 18.0f);

                // Score
                drawNumber(score,     SCR_WIDTH*.5f-55, SCR_HEIGHT*.43f,
                           14, 1.0f,1.0f,0.3f);
                // Best
                drawNumber(highScore, SCR_WIDTH*.5f+55, SCR_HEIGHT*.43f,
                           14, 0.3f,1.0f,0.4f);

                // Blinking restart indicator
                float blink = (float)(sin(now*3)*.5+.5);
                drawRect(SCR_WIDTH*.5f, SCR_HEIGHT*.28f, 90,8,
                         blink, blink*.8f, 0, blink);
            }
        }

        glfwSwapBuffers(window);
    }

    // ── Cleanup ───────────────────────────────────────────────
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteTextures(1, &carPlayerTex);
    glDeleteTextures(1, &carEnemyTex);
    glDeleteProgram(spriteShader);
    glfwTerminate();
    return 0;
}
