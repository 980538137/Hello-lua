//
//  UpdateLayer.cpp
//  Hello-lua
//
//  Created by Guoxing.song on 14/11/6.
//
//

#include "UpdateLayer.h"
#include "CCLuaEngine.h"
#include "Lua_extensions_CCB.h"
#include "Lua_web_socket.h"
#include "lua_pb.h"

UpdateLayer::UpdateLayer()
:pProgressLabel(NULL)
{
    init();
}

UpdateLayer::~UpdateLayer()
{
    AssetsManager* pAssetsManager = getAssetsManager();
    CC_SAFE_DELETE(pAssetsManager);
}

bool UpdateLayer::init()
{
    CCLayer::init();
    
    createDownloadedDir();
    
    
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerColor *layer = CCLayerColor::create(ccc4(255, 255, 255, 255));
    layer->setAnchorPoint(ccp(0.5, 0.5));
    layer->setPosition(ccp(0,0));
    addChild(layer);
    
    pProgressLabel = CCLabelTTF::create("UpdateLayer", "Arial", 50);
    pProgressLabel->setPosition(ccp(size.width/2, size.height/2));
    pProgressLabel->setColor(ccc3(0, 0, 0));
    addChild(pProgressLabel);
    
    CCSprite* pProgressBg = CCSprite::create("image_loading_bar_bg.png");
    pProgressBg->setPosition(ccp(size.width/2, 200));
    pProgressBg->setScale(2.0f);
    addChild(pProgressBg);
    
    CCSprite* pProgressSprite = CCSprite::create("image_loading_bar.png");
    m_pProgressTimer = CCProgressTimer::create(pProgressSprite);
    m_pProgressTimer->setType(kCCProgressTimerTypeBar);
    m_pProgressTimer->setScale(pProgressBg->getScale());
    m_pProgressTimer->setMidpoint(ccp(0, 0));
    m_pProgressTimer->setBarChangeRate(ccp(1, 0));
    m_pProgressTimer->setPosition(pProgressBg->getPosition());
    addChild(m_pProgressTimer);
    
    return true;
}


AssetsManager* UpdateLayer::getAssetsManager()
{
    static AssetsManager* pAssetsManager = NULL;
    
    if (!pAssetsManager)
    {
        pAssetsManager = new AssetsManager("https://raw.githubusercontent.com/980538137/AssetsManagerTest/master/package.zip",
                                           "https://raw.githubusercontent.com/980538137/AssetsManagerTest/master/version",
                                           pathToSave.c_str());
        pAssetsManager->setDelegate(this);
        pAssetsManager->setConnectionTimeout(3);
    }
    
    return pAssetsManager;
}

void UpdateLayer::createDownloadedDir()
{
    pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
//    pathToSave += "sgx";
    CCLog("PathToSave:%s",pathToSave.c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;
    
    pDir = opendir(pathToSave.c_str());
    if (!pDir) {
        mkdir(pathToSave.c_str(),S_IRWXU | S_IRWXG | S_IRWXO);
    }
#else
    if (GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES) {
        CreateDirectoryA(pathToSave.c_str(),0);
    }
    
#endif
}


void UpdateLayer::onError(cocos2d::extension::AssetsManager::ErrorCode errorCode)
{
    CCLog("onError");
    if (errorCode == AssetsManager::kNoNewVersion)
    {
        pProgressLabel->setString("no new version");
    }
    else if (errorCode == AssetsManager::kNetwork)
    {
        pProgressLabel->setString("network error");
    }
    
}

void UpdateLayer::onProgress(int percent)
{
    CCLog("onProgress");
    char progress[20];
    snprintf(progress, sizeof(progress), "downloading %d%%",percent);
    pProgressLabel->setString(progress);
    m_pProgressTimer->setPercentage(percent);
}

void UpdateLayer::onSuccess()
{
    CCLog("onSuccess");
    pProgressLabel->setString("download ok");
    
    //资源下载完成，执行脚本
    executeScriptFile();
}

void UpdateLayer::executeScriptFile()
{
    
    
    // register lua engine
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    pathToSave += "package/";
    CCLog("Path:%s",pathToSave.c_str());
//    pEngine->addSearchPath(pathToSave.c_str());
    CCFileUtils::sharedFileUtils()->addSearchPath(pathToSave.c_str());
    const vector<string>& paths = CCFileUtils::sharedFileUtils()->getSearchPaths();
    for (vector<string>::const_iterator it = paths.begin(); it != paths.end(); it++)
    {
        string path = *it;
        CCLog("Path:%s",path.c_str());
    }
//    string path2 = CCFileUtils::sharedFileUtils()->fullPathForFilename("hello2.lua");
//    pEngine->addSearchPath(path2.substr(0,path2.find_last_of("/")).c_str());
    
    CCLuaStack *pStack = pEngine->getLuaStack();
    lua_State *tolua_s = pStack->getLuaState();
    tolua_extensions_ccb_open(tolua_s);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    pStack = pEngine->getLuaStack();
    tolua_s = pStack->getLuaState();
    tolua_web_socket_open(tolua_s);
#endif
    
//    //开启json支持
//    pStack = pEngine->getLuaStack();
//    tolua_s = pStack->getLuaState();
//    luaopen_cjson(tolua_s);
    
    //开启protobuf支持
    pStack = pEngine->getLuaStack();
    tolua_s = pStack->getLuaState();
    luaopen_pb(tolua_s);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    CCFileUtils::sharedFileUtils()->addSearchPath("script");
#endif
    
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("hello.lua");
    CCLog("HelloPath:%s",path.c_str());
    pEngine->executeScriptFile(path.c_str());
    
}

void UpdateLayer::checkUpdate()
{
    if (getAssetsManager()->checkUpdate())
    {   //有新版本更新
        getAssetsManager()->update();
    }
    else
    {
        //TODO加载游戏资源
        executeScriptFile();
        
    }
}