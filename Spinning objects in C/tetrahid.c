#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

float A=0, B=0, C=0; 
int height = 44, width = 160;
float K1=40;
int distanceFromCamera = 100;
float pi=20, pj=20, pk=20, z;
const float cubeWidth = 20, incspeed = 0.02;

char buffer[160*44];
float zbuffer[160*44];

float vertices [4][3] = {
    {cubeWidth, cubeWidth, cubeWidth},
    {cubeWidth, -cubeWidth, -cubeWidth},
    {-cubeWidth, cubeWidth, -cubeWidth}, 
    {-cubeWidth, -cubeWidth, cubeWidth}
};

float calculateX(int i, int j, int k){
        return j*sin(A)*sin(B)*cos(C) - k*cos(A)*sin(B)*cos(C)
                + j*cos(A)*sin(C) + k*sin(A)*sin(C) + i*cos(B)*cos(C);
    }

float calculateY(int i, int j, int k){
    return j*cos(A)*cos(C) + k*sin(A)*cos(C) - j*sin(A)*sin(B)*sin(C)
            + k*cos(A)*sin(B)*sin(C) - i*cos(B)*sin(C);
    }

float calculateZ(int i , int j, int k){
    return k*cos(A)*cos(B) - j*sin(A)*cos(B) + i*sin(B);
}

void calculateSurface(float cubeX, float cubeY, float cubeZ, int ch){

    float rx = calculateX(cubeX, cubeY, cubeZ);
    float ry = calculateY(cubeX, cubeY, cubeZ);
    float rz = calculateZ(cubeX, cubeY, cubeZ);

    float z = rz + distanceFromCamera;  
    float ooz = 1/z;

    int sx = (int)(width/2 + rx*K1*ooz*2);
    int sy = (int)(height/2 + ry*K1*ooz);

    int idx = sx + sy * width;

    if (idx >=0 && idx < height*width)
    {
        if (ooz > zbuffer[idx])
        {
            zbuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }

    
}

void drawFace(int v0, int v1, int v2, int ch){
    float Xofv0 = vertices[v0][0];
    float Yofv0 = vertices[v0][1];
    float Zofv0 = vertices[v0][2];

    float Xofv1 = vertices[v1][0];
    float Yofv1 = vertices[v1][1];
    float Zofv1 = vertices[v1][2];

    float Xofv2 = vertices[v2][0];
    float Yofv2 = vertices[v2][1];
    float Zofv2 = vertices[v2][2];

    for (float s = 0; s < 1; s+=incspeed)
    {
        for (float t = 0; t < 1-s; t+=incspeed)
        {
            float px = Xofv0 + s*(Xofv1-Xofv0) + t*(Xofv2-Xofv0);
            float py = Yofv0 + s*(Yofv1-Yofv0) + t*(Yofv2-Yofv0);
            float pz = Zofv0 + s*(Zofv1-Zofv0) + t*(Zofv2-Zofv0);

            calculateSurface(px, py, pz, ch);
        }
        
    }
    
}

int main(){
    printf("\x1b[2J"); //clears entire screen

    while (1)
    {
        memset(buffer, ' ', height*width);
        memset(zbuffer, 0, height*width*4);

        drawFace(0, 1, 2, '.');
        drawFace(0, 1, 3, '$');
        drawFace(0, 2, 3, '~');
        drawFace(1, 2, 3, '#');

        printf("\x1b[H"); //moves cursor to the top left corner  

        

        for (int i = 0; i < height*width; i++)
        {
            putchar(i%width ? buffer[i]:'\n');
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;

        usleep(30000);
    }

    return 0;
}