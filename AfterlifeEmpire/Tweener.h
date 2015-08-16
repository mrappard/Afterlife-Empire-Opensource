//
//  Tweener.h
//  After Life Empire
//
//  Created by Autobotika on 2015-01-24.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__Tweener__
#define __After_Life_Empire__Tweener__


class Tweener{

public:
    static double quadEaseIn(double t, double b, double c, double d);
    static double quadEaseOut(double t, double b, double c, double d);
    static double quadEaseBoth(double t, double b, double c, double d);
    static double cubicEaseIn(double t, double b, double c, double d);
    static double cubicEaseOut(double t, double b, double c, double d);
    static double cubicEaseBoth(double t, double b, double c, double d);
    static double quatricEaseIn(double t, double b, double c, double d);
    static double quatricEaseOut(double t, double b, double c, double d);
    static double quatricEaseBoth(double t, double b, double c, double d);
};



 #endif /* defined(__After_Life_Empire__Tweener__) */
 
 

 
 


