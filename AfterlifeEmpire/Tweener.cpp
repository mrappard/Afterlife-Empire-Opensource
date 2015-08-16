//
//  Tweener.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-01-24.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include "Tweener.h"

double Tweener::quadEaseIn(double t, double b, double c, double d){
    t/=d;
    return c*t*t+b;
}

double Tweener::quadEaseOut(double t, double b, double c, double d){
    t/=d;
    return -c * t*(t-2)+b;
    
}

double Tweener::quadEaseBoth(double t, double b, double c, double d){
    t/=d;
    t--;
    return c*(t*t*t+1)+b;
    
}

double Tweener::cubicEaseIn(double t, double b, double c, double d){
    t/=d;
    return c*t*t*t+b;
    
}

double Tweener::cubicEaseOut(double t, double b, double c, double d){
    t/=d;
    t--;
    return c*(t*t*t + 1)+b;
}

double Tweener::cubicEaseBoth(double t, double b, double c, double d){
    t/=d/2;
    
    if (t<1) return c/2*t*t*t+b;
    t-= 2;
    return c/2*(t*t*t +2)+ b;
    
    
    
}

double Tweener::quatricEaseIn(double t, double b, double c, double d){
    t /=d;
    return c*t*t*t*t+b;
    
    
}

double Tweener::quatricEaseOut(double t, double b, double c, double d){
    t /=d;
    t--;
    return -c * (t*t*t*t-1)+b;
    
}


double Tweener::quatricEaseBoth(double t, double b, double c, double d){
    t /= d/2;
    if (t<1) return c/2*t*t*t*t+b;
    t-=2;
    return -c/2 *(t*t*t*t-2) + b;
    
}