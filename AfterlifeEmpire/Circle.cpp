//
//  Tiles.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "Circle.h"
#include "ShaderManager.h"
#include "Tiles.h"
#include "ScreenTranslator.h"
#include "GlobalDebugger.h"
#include "LazyVars.h"
#include "Tweener.h"



std::vector<Circle*> Circle::theCircles;
std::map<std::string, Circle*> Circle::getByName;

Circle::Circle(){

    hold=false;
    inOut=true;
    tween=0.0;

}


Circle::~Circle(){
    
    if (_vertexArray){
        glDeleteVertexArraysCon(1, &_vertexArray);
        
    }
    
    if (_vertexBuffer){
        glDeleteBuffers(1, &_vertexBuffer);
    }


}



Circle::Circle(cpVect _center, GLfloat _radius, GLfloat * _color){

    center=_center;
    radius=_radius;
    color[0]=_color[0];
    color[1]=_color[1];
    color[2]=_color[2];
    color[3]=_color[3];
    
    hold=false;
    inOut=true;
    tween=0.0;
    
    _vertexArray=0;
    _vertexBuffer=0;
    _vertexSize=0;
}

void Circle::update(){

    std::vector<GLfloat> vertex =  createCircle();
    
    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*2, BUFFER_OFFSET(0));
    
    _vertexSize = (GLuint)vertex.size()/2;

    glBindVertexArrayCon(0);
}

void Circle::render(){


    ShaderManager::enableSolidColor(color);
    
    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_FAN, 0, _vertexSize);
        
    }


}

void Circle::addCircle(cpVect _center, GLfloat _radius, GLfloat * _color){
    Circle * newCircle = new Circle(_center,_radius,_color);
    theCircles.push_back(newCircle);
    
}

void Circle::addCircle(cpVect _center, GLfloat _radius, GLfloat * _color, std::string key){
    
    {
        std::map<std::string, Circle*>::iterator IT = getByName.find(key);
    
        if ( IT != getByName.end()){
            IT->second->center=_center;
            return;
        }
    }
    Circle * newCircle = new Circle(_center,_radius,_color);
    theCircles.push_back(newCircle);
    newCircle->theName = key;
    getByName[key]=newCircle;
    newCircle->hold=true;
}


void Circle::setCirclePosition(cpVect _center, std::string key){

    std::map<std::string, Circle*>::iterator IT = getByName.find(key);
    
    if ( IT != getByName.end()){
        IT->second->center=_center;
        return;
    }



}

void Circle::releaseCircle(std::string key){

    
    {
        std::map<std::string, Circle*>::iterator IT = getByName.find(key);
        
        if ( IT != getByName.end()){
            IT->second->hold = false;
            if(IT->second->tween<30){
                IT->second->tween= 30 +(30-IT->second->tween);
            }
            
            
        }
    }
}


std::vector<GLfloat> Circle::createCircle(){
    
    std::vector<GLfloat> values;
    
    values.push_back(center.x);
    values.push_back(center.y);
    
    
    double radiusT = Tweener::quadEaseOut(tween,0,1,30) * radius;

    
    
    double amountToTurn = M_PI/48.0;
    
    
        for (double turn = 0; turn < 2*M_PI; turn+=amountToTurn){
            cpVect theTarget =cpv(sin(turn)*radiusT,cos(turn)*radiusT);
            theTarget = cpvadd(theTarget, center);
            values.push_back(theTarget.x);
            values.push_back(theTarget.y);
        }
        
        {
            cpVect theTarget =cpv(sin(0)*radiusT,cos(0)*radiusT);
            theTarget = cpvadd(theTarget, center);
            values.push_back(theTarget.x);
            values.push_back(theTarget.y);
        }
    
    return values;
    
    
}

void Circle::tickAll(){
    std::vector<size_t> killer;
    
    
    for (std::vector<Circle*>::iterator IT = theCircles.begin(); IT != theCircles.end(); IT++){
        
        if ((*IT)->hold){
            
            (*IT)->tween=(*IT)->tween+1;
            if ((*IT)->tween>30){
            
                (*IT)->tween=30;
            }
            
        
        }else{
            
        (*IT)->tween=(*IT)->tween+1;
        }
        if ((*IT)->tween>60){
            killer.push_back(IT - theCircles.begin());
        }
        
        
    }
    
    while (killer.size()>0) {
        size_t index = killer.back();
        Circle * theCircle = theCircles[index];
        
        std::map<std::string, Circle*>::iterator IT;
        
        IT = getByName.find(theCircle->theName);
        
        if (IT != getByName.end()){
            getByName.erase(IT);
        }
        
        delete theCircle;
        
        theCircles.erase(theCircles.begin()+index);
        killer.pop_back();
    }
    
    
}

void Circle::updateAll(){
    for (std::vector<Circle*>::iterator IT = theCircles.begin(); IT != theCircles.end(); IT++){
        (*IT)->update();
    }
}


void Circle::renderAll(){
    for (std::vector<Circle*>::iterator IT = theCircles.begin(); IT != theCircles.end(); IT++){
        (*IT)->render();
    }
}
