-- cclog
cclog = function(...)
    print(string.format(...))
end

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
end

local function main()
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    require "../proto/mobileGame_pb"
    local msg = mobileGame_pb.ReqGameAccountLogin()
    msg.account = "123456"
    msg.accountType = 0
    msg.password = "123456"
    msg.terminal = 2
    msg.deviceNumber = "34324234"
    msg.gameId = 123
    msg.comeFrom = "ydmm"
    msg.token = ""

    local pb_data = msg:SerializeToString()
    print("create:", msg.account, msg.password, pb_data)

    local msg2 = mobileGame_pb.ReqGameAccountLogin()
    msg2:ParseFromString(pb_data)
    print("parser:", msg2.account, msg2.password, pb_data)

    local function initLayer()

    end
    

    -- run
    local sceneGame = CCScene:create()
    -- sceneGame:addChild(initLayer())
    if (CCDirector:sharedDirector():getRunningScene() == nil ) then 
        CCDirector:sharedDirector():runWithScene(sceneGame)
    else
        CCDirector:sharedDirector():replaceScene(sceneGame)
    end
    -- CCDirector:sharedDirector():runWithScene(sceneGame)
end

xpcall(main, __G__TRACKBACK__)
