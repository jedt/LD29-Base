//
//  Common.h
//  MyGame1
//
//  Created by Jed Tiotuico on 12/11/13.
//  Copyright (c) 2013 Jed Tiotuico. All rights reserved.
//

#ifndef MyGame1_Common_h
#define MyGame1_Common_h

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

#include "vector2.h"

#define MY_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define MY_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

extern std::string g_title;

void printV(sf::Vector2f vectorArg, std::string label) {
	std::cout << label << "[" << vectorArg.x << "," << vectorArg.y << std::endl;
}

std::string sprintV(sf::Vector2f vectorArg) {
	std::ostringstream buff;
	buff << "[" << vectorArg.x << "," << vectorArg.y << "]";
	return buff.str();
}

std::string ConvertToString(int number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();
}
std::string ConvertToString(float number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();
}
float angleForDestination(sf::Vector2f origin, sf::Vector2f dest) {
    VECTOR2 destPos = VECTOR2(dest.x, dest.y);
    //printV(paddlePos, "paddlePos");
    
    VECTOR2 originPos = VECTOR2(origin.x, origin.y);
    //printV(ballPos, "ballPos");
    
    VECTOR2 originToDest = originPos - destPos;
    //printV(originToDest.sf(), "originToDest");
    
    VECTOR2 selfToTargetNormalized = originToDest.norm();
    
	//sf::Vector2f normalized = selfToTargetNormalized.sf();
    
    //cout << "normalized" << endl;
    //printV(selfToTargetNormalized, "selfToTargetNormalized");

    VECTOR2 facing = VECTOR2(-1, 0).norm();

    float dot = facing.dot(selfToTargetNormalized);

	//std::cout << "dot: " << dot << std::endl;

    float angle = MY_RADIANS_TO_DEGREES(acosf(dot));

    //cout << "angle: " << angle << endl;

    return angle;
}
sf::Vector2f unitVelocity(sf::Vector2f origin, sf::Vector2f dest) {
    VECTOR2 paddlePos = VECTOR2(dest.x, dest.y);
    //printV(paddlePos, "paddlePos");
    
    VECTOR2 ballPos = VECTOR2(origin.x, origin.y);
    //printV(ballPos, "ballPos");
    
    VECTOR2 selfToTarget = ballPos - paddlePos;
    //printV(selfToTarget, "selfToTarget");
    
    VECTOR2 selfToTargetNormalized = selfToTarget.norm();
    
    return selfToTargetNormalized.sf();
    
	//    //cout << "normalized" << endl;
	//    //printV(selfToTargetNormalized, "selfToTargetNormalized");
	//
	//    VECTOR2 facing = VECTOR2(0, 1).norm();
	//
	//    float dot = facing.dot(selfToTargetNormalized);
	//
	//    //cout << "dot: " << dot << endl;
	//
	//    float angle = MY_RADIANS_TO_DEGREES(acosf(dot));
	//
	//    //cout << "angle: " << angle << endl;
	//
	//    return angle;
}

sf::Vector2f tileCoordForPosition(sf::Vector2f aPos, sf::Vector2f aTileSize) {
    float x = 0;
    if (aPos.x != 0) {
        x = aPos.x / aTileSize.x;
    }
    
    float y = 0;
    if (aPos.y != 0) {
        y = aPos.y / aTileSize.y;
    }
    
    return sf::Vector2f(x, y);
}

sf::Vector2f positionForTileCoord(sf::Vector2f aTileCoord, sf::Vector2f aTileSize) {
    int x = 0;
    int y = 0;
    
    x = aTileCoord.x * aTileSize.x;
    y = aTileCoord.y * aTileSize.y;
    
    return sf::Vector2f(x, y);
}

sf::Rect<float> detectingBoxFromOrigin(sf::Vector2f origin, sf::Vector2f size) {
	//create a rectangle based on a point of origin
	sf::Rect<float> detectingBox = sf::FloatRect(origin.x - size.x, origin.y,origin.x + size.x, origin.y + size.y);
	return detectingBox;
}

float getDistanceV(VECTOR2 v1, VECTOR2 v2) {
	
	return sqrtf(powf((v2.x - v1.x), 2) + powf((v2.y - v1.y), 2));
}

#endif
