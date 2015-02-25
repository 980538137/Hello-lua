//
//  MySprite.h
//  Hello-lua
//
//  Created by Guoxing.song on 14/11/5.
//
//

#ifndef __Hello_lua__MySprite__
#define __Hello_lua__MySprite__

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class MySprite : public CCSprite
{
public:
    static MySprite* createMS(const char* fileName);
};
#endif /* defined(__Hello_lua__MySprite__) */
