//
//  MySprite.cpp
//  Hello-lua
//
//  Created by Guoxing.song on 14/11/5.
//
//

#include "MySprite.h"

MySprite* MySprite::createMS(const char *fileName)
{
    MySprite* sp = new MySprite;
    if (sp && sp->initWithFile(fileName))
    {
        sp->setPosition(ccp(200, 200));
        sp->autorelease();
        return sp;
    }
    
    CC_SAFE_DELETE(sp);
    return NULL;
}