#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

float A=0, B=0, C=0; 
int height = 44, width = 160;
float K1=40;
int distanceFromCamera = 100;
float pi=20, pj=20, pk=20, z;
float cubeWidth = 20, incspeed = 0.6;

char buffer[160*44];
float zbuffer[160*44];

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

int main(){
    printf("\x1b[2J"); //clears entire screen

    while (1)
    {
        memset(buffer, ' ', height*width);
        memset(zbuffer, 0, height*width*4);

        // float rx = calculateX(pi, pj, pk);
        // float ry = calculateY(pi, pj, pk);
        // float rz = calculateZ(pi, pj, pk);

        // float z = rz + distanceFromCamera;  
        // float ooz = 1/z;

        // int sx = (int)(width/2 + rx*K1*ooz*2);
        // int sy = (int)(height/2 + ry*K1*ooz);

        // int idx = sx + sy * width;

        // if (idx >=0 && idx < area)
        // {
        //     if (ooz > zbuffer[idx])
        //     {
        //         zbuffer[idx] = ooz;
        //         buffer[idx] = '@';
        //     }
        // }

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX+= incspeed)
        {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY+=incspeed)
            {
                calculateSurface(cubeX, cubeY, cubeWidth, '@');
                calculateSurface(cubeX, cubeY, -cubeWidth, '#');
                calculateSurface(cubeY, cubeWidth, cubeX, '+');
                calculateSurface(cubeY, -cubeWidth, cubeX, ';');
                calculateSurface(-cubeWidth, cubeX, cubeY, '~');
                calculateSurface(+cubeWidth, cubeX, cubeY, '$');

            }
        }

        printf("\x1b[H"); //moves cursor to the top left corner  

        

        for (int i = 0; i < height*width; i++)
        {
            putchar(i%width ? buffer[i]:'\n');
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;

        usleep(25000);
    }

    return 0;
}