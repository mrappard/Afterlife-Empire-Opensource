//
//  ScreenTranslator.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-10-09.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "ScreenTranslator.h"

#include "LazyVars.h"
#include "GlobalDebugger.h"

double rotationMatrix[4][4];
double inputMatrix[4][4];
double outputMatrix[4][4];

void multiplyMatrix()
{
    for(int i = 0; i < 4; i++ ){
        for(int j = 0; j < 1; j++){
            //outputMatrix[i][j] = 0;
            for(int k = 0; k < 4; k++){
                outputMatrix[i][j] += rotationMatrix[i][k] * inputMatrix[k][j];
            }
        }
    }
}
void setUpRotationMatrix(float angle, float u, float v, float w)
{
    float L = (u*u + v * v + w * w);
    //angle = angle * M_PI / 180.0; //converting to radian value
    float u2 = u * u;
    float v2 = v * v;
    float w2 = w * w;
    
    rotationMatrix[0][0] = (u2 + (v2 + w2) * cos(angle)) / L;
    rotationMatrix[0][1] = (u * v * (1 - cos(angle)) - w * sqrt(L) * sin(angle)) / L;
    rotationMatrix[0][2] = (u * w * (1 - cos(angle)) + v * sqrt(L) * sin(angle)) / L;
    rotationMatrix[0][3] = 0.0;
    
    rotationMatrix[1][0] = (u * v * (1 - cos(angle)) + w * sqrt(L) * sin(angle)) / L;
    rotationMatrix[1][1] = (v2 + (u2 + w2) * cos(angle)) / L;
    rotationMatrix[1][2] = (v * w * (1 - cos(angle)) - u * sqrt(L) * sin(angle)) / L;
    rotationMatrix[1][3] = 0.0;
    
    rotationMatrix[2][0] = (u * w * (1 - cos(angle)) - v * sqrt(L) * sin(angle)) / L;
    rotationMatrix[2][1] = (v * w * (1 - cos(angle)) + u * sqrt(L) * sin(angle)) / L;
    rotationMatrix[2][2] = (w2 + (u2 + v2) * cos(angle)) / L;
    rotationMatrix[2][3] = 0.0;
    
    rotationMatrix[3][0] = 0.0;
    rotationMatrix[3][1] = 0.0;
    rotationMatrix[3][2] = 0.0;
    rotationMatrix[3][3] = 1.0;
}














cpVect translater(cpVect control){
    
    control.y *=2.0;
    
    
    /*
     double rotation = M_PI/4.0;
     
    cpVect output;
    
    output.x = control.x*cos(rotation)+control.y*sin(rotation);
    output.y = -control.x*sin(rotation)+control.y*cos(rotation);

    return output;
    */
    cpVect returnValue = cpvrotate(control, cpv(1.0,1.0));
    returnValue = cpvrotate(returnValue, cpv(1.0,1.0));
    returnValue = cpvrotate(returnValue, cpv(1.0,1.0));
    
    
    return returnValue;
    
}