//
//  UpdateLayer.h
//  Hello-lua
//
//  Created by Guoxing.song on 14/11/6.
//
//

#ifndef __Hello_lua__UpdateLayer__
#define __Hello_lua__UpdateLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "AppDelegate.h"

using namespace std;
using namespace cocos2d;

class UpdateLayer : public CCLayer, public cocos2d::extension::AssetsManagerDelegateProtocol
{
public:
    UpdateLayer();
    ~UpdateLayer();
    
    virtual bool init();
    
    virtual void onError(cocos2d::extension::AssetsManager::ErrorCode errorCode);
    virtual void onProgress(int percent);
    virtual void onSuccess();
    
    void checkUpdate();
private:
    
    void executeScriptFile();
    void createDownloadedDir();
    
    
    AssetsManager* getAssetsManager();
    
    string pathToSave;
    
    CCLabelTTF* pProgressLabel;
    
    CCProgressTimer* m_pProgressTimer;
};

#endif /* defined(__Hello_lua__UpdateLayer__) */
