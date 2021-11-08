#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <sstream>
#include <iostream>
using namespace std;
#include <time.h>
#include "../../../bin/collider.h"

GLuint vao;
GLuint vao2;
GLuint vaoSword;

GLuint vbo;
GLuint vbo2;
GLuint vboSword;

GLuint shader;

float x = -600.0;
float y = -200.0;

float bonusX = 0.0f;
float bonusY = 0.0f;

float maxX = 4.0f;
float maxY = 6.0f;

float dashX = 0.0f;
float dashY = 0.0f;

float swordX = 0.0f;
float swordY = 0.0f;

float cameraX = 0.0;

float swordOpaque = 0.0;
float swordRot = 0.0;
float swordLength = 30.0;
float swordAscending = true;
bool swordDone = false;

bool dashed = false;
bool won = false;

float angle = 0.0f;

float velX = 0.0;
float velY = 0.0;

clock_t begin_time;
clock_t jump_cd;
clock_t terrain_cd;
clock_t sword_cd;
const int sizeB = 50;
collider Solids[sizeB+1];


collider Interactable[5];


int main(void){
    Solids[0] = collider(200.0f, 10.0f, -600.0f, -250.0f);
    Solids[1] = collider(200.0f, 10.0f, -200.0f, -250.0f);
    Solids[2] = collider(200.0f, 10.0f, 100.0f, -180.0f);
    Solids[3] = collider(50.0f, 10.0f, 50.0f, -100.0f);
    Solids[4] = collider(50.0f, 10.0f, 150.0f, -20.0f);
    Solids[5] = collider(200.0f, 10.0f, 600.0f, -20.0f);
    Solids[6] = collider(100.0f, 10.0f, 800.0f, 50.0f, true, 800.0f, 50.0f, 1200.0f, 50.0f);
    Solids[7] = collider(200.0f, 10.0f, 1500.0f, 100.0f);
    Solids[8] = collider(100.0f, 10.0f, 1800.0f, 200.0f);
    Solids[9] = collider(15.0f, 350.0f, 1950.0f, 400.0f);
    Solids[10] = collider(100.0f, 10.0f, 2100.0f, 200.0f);
    Solids[11] = collider(15.0f, 200.0f, 2300.0f, 0.0f);
    Solids[12] = collider(15.0f, 200.0f, 2550.0f, -100.0f);
    Solids[13] = collider(15.0f, 200.0f, 2700.0f, -50.0f);
    Solids[14] = collider(15.0f, 700.0f, 3200.0f, 100.0f);
    Solids[15] = collider(200.0f, 10.0f, 3400.0f, -350.0f);
    Solids[16] = collider(200.0f, 10.0f, 3400.0f, -300.0f, true, 3400.0f, -300.0f, 3401.0f, -200.0f);
    Solids[17] = collider(50.0f, 10.0f, 3600.0f, -270.0f);
    Solids[18] = collider(50.0f, 10.0f, 3300.0f, -150.0f);
    Solids[19] = collider(100.0f, 100.0f, 4400.0f, 0.0f);
    Solids[20] = collider(20.0f, 350.0f, 4370.0f, -225.0f);

    //Solids[19] = collider(100.0f, 1000.0f, 3800.0f, 0.0f);

    Interactable[0] = collider(25.0f, 25.0f, 3400.0f, -100.0f);
    Interactable[1] = collider(25.0f, 25.0f, 3600.0f, -30.0f);

    //Initialize window   
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1600, 900, "Sword Scroller", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.5f, 0.75f, 1.0f, 0.0f);
    
    shader = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");


    do {
        while (((double)(clock()- begin_time)/CLOCKS_PER_SEC) < (1/120.0))  {
        }
        begin_time = clock();

        glClear(GL_COLOR_BUFFER_BIT);
        Physics();
        Move_block(Solids);
        if (x >= 4350.0f || cameraX <= -4700.0f) {
            won = true;
            cameraX = -4400.0f;
        }
        if (!won) {
            cameraX = cameraX - 1.5f;
        }
        else {
            glfwSetWindowTitle(window, "Won");
        }

        DrawPerson();

        DrawAllRect(Solids, 1);
        DrawAllRect(Interactable, 2);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}

void Physics() {

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (velX < -2.0f)  velX = -2.0f;
        velX = velX + 0.2f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (velX > 2.0f)  velX = 2.0f;
        velX = velX - 0.2f;
    }
    else {
        if ((velX <= 0.2f && velX > 0.0f) || (velX >= -0.2f && velX < 0.0f)) velX = 0;
        else {
            velX = sgn(velX) * (fabs(velX) - 0.2f);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && ((double)(jump_cd - clock()) / CLOCKS_PER_SEC) < -(1 / 10.0) && ((double)(terrain_cd - clock()) / CLOCKS_PER_SEC) > -(1 / 20.0)) {
        jump_cd = clock();
        if (velX < 0.0f)   velX = 0.0f;
        velY = velY + 6.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //printf("S");
        velY = velY - 1.0f;
    }

    if (!dashed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && ((double)(sword_cd - clock()) / CLOCKS_PER_SEC) < -(1 / 2.0f)) {
        sword_cd = clock();
        dashed = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float xDiff = xpos-x-800.0f-cameraX;
        float yDiff = -ypos-y+450.0f;
        angle = atan2(yDiff, xDiff);

        angle = roundf(angle * 180 / 3.14159265 / 22.5f) * 22.5f / 180 * 3.14159265;

        bonusX = 2.0f * cos(angle);
        bonusY = 0;
        dashX = maxX * cos(angle) + bonusX;
        dashY = maxY * sin(angle) + bonusY;

        angle = angle * 180 / 3.14159265;


        swordRot = angle + 96.0f;

        swordOpaque = 1.0;
        swordLength = 15.0f;
        swordAscending = true;
        swordDone = false;
    }

    if ((double)(swordRot >= (angle - 90.0f) && !swordDone)) {
        swordRot = swordRot - 12.0f;

        if (swordAscending) {
            swordLength = swordLength + 4.0f;
            velX = dashX;
            velY = dashY;
        }
        else {
            swordLength = swordLength - 6.0f;
        }
        swordX = cos(swordRot / 180 * 3.14159265) * swordLength + x;
        swordY = sin(swordRot / 180 * 3.14159265) * swordLength + y;

        //DrawRectangle(10.0f, 10.0f, swordX, swordY, 1);
        if (Impact_sword(Interactable, swordX, swordY)) {
        }

        if (swordLength >= 51.0f) {
            swordAscending = false;
        }
    }
    else if(!swordDone) {
        swordOpaque = 0.0;
        swordRot = 90.0f;
        bonusX = 0.0f;
        bonusY = 0.0f;
        swordX = 0;
        swordY = 0;
        if (velY >= 6.0f)      velY = 6.0f;
        swordDone = true;
    }
    
    velY = velY - 0.2f;
    if (velX >= maxX + bonusX)      velX = maxX + bonusX;
    if (velX < -maxX + bonusX)      velX = -maxX + bonusX;
    if (velY >= maxY + bonusY)      velY = maxY + bonusY;
    if (velY < -maxY + bonusY)      velY = -maxY + bonusY;
    collider Body = collider(20.0f, 40.0f, x + velX, y + velY);
    

    if (velY < 0.0f) {
        if (Impact_top(Solids, Body) || Impact_top(Interactable, Body)) {
            dashed = false;
            terrain_cd = clock();
            velY = 0.0f;
        }
    }
    else if (velY > 0.0f) {
        if (Impact_bottom(Solids, Body) || Impact_bottom(Interactable, Body)) {
            velY = 0.0f;
        }
    }
    Body = collider(20.0f, 40.0f, x + velX, y + velY);
    if (velX > 0.0f) {
        if (Impact_left(Solids, Body) || Impact_left(Interactable, Body)) {
            velX = 0.0f;
        }
    }
    if (velX < 0.0f) {
        if (Impact_right(Solids, Body) || Impact_right(Interactable, Body)) {
            velX = 0.0f;
        }
    }
    x = x + velX;
    y = y + velY;
    if (y <= -675.0f) {
        velX = 0.0f;
        velY = 0.0f;
        if (won) {
            y = 300.0;
            x = 4400.0f;
        }
        else {
            x = -600.0;
            y = -200.0;
            cameraX = 0.0;
        }
    }
    if (x <= -1000.0f - cameraX) {
        velX = 0.0f;
        velY = 0.0f;
        x = -600.0;
        y = -200.0;
        cameraX = 0.0;
        won = false;
    }


}

void Move_block(collider Blocks[]) {
    for (int i = 0; i <= sizeB; i++) {
        if (Blocks[i].exists && Blocks[i].moving) {
            if (!Blocks[i].forward && (Blocks[i].posX <= Blocks[i].originX && Blocks[i].originX < Blocks[i].destinationX) || (Blocks[i].posX > Blocks[i].originX && Blocks[i].originX > Blocks[i].destinationX)) {
                Blocks[i].forward = true;
                Blocks[i].posX = Blocks[i].originX;
                Blocks[i].posY = Blocks[i].originY;
            }else if (Blocks[i].forward && (Blocks[i].posX <= Blocks[i].destinationX && Blocks[i].destinationX < Blocks[i].originX) || (Blocks[i].posX > Blocks[i].destinationX && Blocks[i].destinationX > Blocks[i].originX)){
                if (Blocks[i].colliding_top(collider(30.0f, 60.0f, x, y - 0.01f))) {
                    x = x - (Blocks[i].destinationX - Blocks[i].originX) / 200.0f;
                }
                cout << x;
                Blocks[i].forward = false;
                Blocks[i].posX = Blocks[i].destinationX;
                Blocks[i].posY = Blocks[i].destinationY;
            }
            else if (Blocks[i].forward) {
                if (Blocks[i].colliding_top(collider(30.0f, 60.0f, x, y - 0.01f))) {
                    x = x + (Blocks[i].destinationX - Blocks[i].originX) / 200.0f;
                    //velX = velX + (Blocks[i].destinationX - Blocks[i].originX) / 200.0f;
                }
                Blocks[i].posX = Blocks[i].posX + (Blocks[i].destinationX - Blocks[i].originX) / 200.0f;
                Blocks[i].posY = Blocks[i].posY + (Blocks[i].destinationY - Blocks[i].originY) / 200.0f;
            }
            else {
                if (Blocks[i].colliding_top(collider(30.0f, 60.0f, x, y - 0.01f))) {
                    x = x - (Blocks[i].destinationX - Blocks[i].originX) / 200.0f;
                    //velX = velX -(Blocks[i].destinationX - Blocks[i].originX) / 200.0f;
                }
                Blocks[i].posX = Blocks[i].posX - (Blocks[i].destinationX - Blocks[i].originX) / 200.0f;
                Blocks[i].posY = Blocks[i].posY - (Blocks[i].destinationY - Blocks[i].originY) / 200.0f;
            }
            //cout << Blocks[i].destinationX - Blocks[i].originX << endl;
            //cout << Blocks[i].destinationY - Blocks[i].originY << endl;
        }
    }
}

bool Impact_top(collider Solids[], collider Body) {
    for(int i = 0; i<= sizeB; i++){
        if (Solids[i].exists && Solids[i].colliding_top(Body)) {
            y = Solids[i].posY+Solids[i].lengthY/2+Body.lengthY/2;
            return true;
        }
    }
    return false;
}

bool Impact_bottom(collider Solids[], collider Body) {
    for (int i = 0; i <= sizeB; i++) {
        if (Solids[i].exists && Solids[i].colliding_bottom(Body)) {
            y = Solids[i].posY - Solids[i].lengthY / 2 - Body.lengthY / 2;
            return true;
        }
    }
    return false;
}

bool Impact_left(collider Solids[], collider Body) {
    for (int i = 0; i <= sizeB; i++) {
        if (Solids[i].exists && Solids[i].colliding_left(Body)) {
            x = Solids[i].posX - Solids[i].lengthX / 2 - Body.lengthX / 2;
            return true;
        }
    }
    return false;
}

bool Impact_right(collider Solids[], collider Body) {
    for (int i = 0; i <= sizeB; i++) {
        if (Solids[i].exists && Solids[i].colliding_right(Body)) {
            x = Solids[i].posX + Solids[i].lengthX / 2 + Body.lengthX / 2;
            return true;
        }
    }
    return false;
}

bool Impact_sword(collider Blocks[], float TipX, float TipY) {
    for (int i = 0; i <= 1; i++) {
        if (Blocks[i].exists && Blocks[i].colliding_sword(TipX, TipY)) {
            Solids[30+i] = collider(100.0f, 10.0f, 3600.0f+300.0f*i, 0.0f);
            return true;
        }
    }
    return false;
}

void DrawAllRect(collider List[], int type) {
    for (int i = 0; i <= sizeB; i++) {
        if (List[i].exists) {
            DrawRectangle(List[i].lengthX, List[i].lengthY, List[i].posX, List[i].posY, type);
        }
    }
}

void DrawPerson() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[] =
    {
        -10.0f, 20.0f, 1.0f, 0.1f, 0.1f, // top left
         10.0f, 20.0f, 1.0f, 0.5f, 0.5f,// top right
        -10.0f, -20.0f, 1.0f, 0.5f, 0.5f, // bottom left

         10.0f, 20.0f, 1.0f, 0.5f, 0.5f,// top right
         10.0f, -20.0f, 1.0f, 0.1f, 0.1f, // bottom right
        -10.0f, -20.0f, 1.0f, 0.5f, 0.5f, // bottom left
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shader);
    int offsetLocation = glGetUniformLocation(shader, "offset");
    glUniform2f(offsetLocation, x + cameraX, y);

    int opacityLocation = glGetUniformLocation(shader, "opacity");
    glUniform1f(opacityLocation, 0.0f);

    int rotationLocation = glGetUniformLocation(shader, "rotation");
    glUniform1f(rotationLocation, 0.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle
    glBindVertexArray(0);

    float sword[] =
    {
        0.0f, 5.00f, 0.5f, 0.5f, 0.5f, // top left
        0.0f, -5.00f, 0.5f, 0.5f, 0.5f, // bottom left
        swordLength, 0.0f, 0.2f, 0.2f, 0.2f,//  right
    };

    glGenBuffers(1, &vboSword);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vboSword);

    glBufferData(GL_ARRAY_BUFFER, sizeof(sword), sword, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shader);
    glUniform2f(offsetLocation, x+cameraX, y);
    glUniform1f(opacityLocation, 1.0f-swordOpaque);
    glUniform1f(rotationLocation, swordRot*3.14159265358979323846/180.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDeleteProgram(shader);
}

void DrawRectangle(float lenX, float lenY, float posX, float posY, int type) {
    float vertices2[] =
    {
        -lenX / 2, lenY / 2, 0.0f, 0.0f, 0.0f, // top left
        lenX / 2, lenY / 2, 0.0f,  0.0f, 0.0f,// top right
        -lenX / 2, -lenY / 2, 0.0f,  0.0f, 0.0f, // bottom left

        lenX / 2, lenY / 2, 0.0f,  1.0f, 0.0f,// top right
        lenX / 2, -lenY / 2, 0.0f, 0.0f, 0.0f, // bottom right
        -lenX / 2, -lenY / 2, 0.0f,  0.0f, 0.0f, // bottom left
    };
    if (type == 1) {
        float vertices3[] =
        {
            -lenX / 2, lenY / 2, 0.25f, 0.25f, 0.25f, // top left
            lenX / 2, lenY / 2, 0.5f,  0.5f, 0.5f,// top right
            -lenX / 2, -lenY / 2, 0.5f, 0.5f, 0.5f, // bottom left

            lenX / 2, lenY / 2, 0.5f,  0.5f, 0.5f,// top right
            lenX / 2, -lenY / 2, 0.25f, 0.25f, 0.25f, // bottom right
            -lenX / 2, -lenY / 2, 0.5f,  0.5f, 0.5f, // bottom left
        };
        for (int i = 0; i < 30; i++) {
            vertices2[i]=vertices3[i];
        }
    }
    else if (type == 2) {
        float vertices3[] =
        {
            -lenX / 2, lenY / 2, 0.5f, 1.0f, 0.5f, // top left
            lenX / 2, lenY / 2, 0.5f,  1.0f, 0.5f,// top right
            -lenX / 2, -lenY / 2, 0.5f,  1.0f, 0.5f, // bottom left

            lenX / 2, lenY / 2, 0.5f,  1.0f, 0.5f,// top right
            lenX / 2, -lenY / 2, 0.5f, 1.0f, 0.5f, // bottom right
            -lenX / 2, -lenY / 2, 0.5f,  1.0f, 0.5f, // bottom left
        };
        for (int i = 0; i < 30; i++) {
            vertices2[i] = vertices3[i];
        }
    }


    glGenVertexArrays(1, &vao2);
    glGenBuffers(1, &vbo2);

    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);


    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shader);
    int offsetLocation = glGetUniformLocation(shader, "offset");
    glUniform2f(offsetLocation, posX + cameraX, posY);

    int opacityLocation = glGetUniformLocation(shader, "opacity");
    glUniform1f(opacityLocation, 0.0f);

    int rotationLocation = glGetUniformLocation(shader, "rotation");
    glUniform1f(rotationLocation, 0.0f);

    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDeleteProgram(shader);
    //glDeleteBuffers(vao2);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
