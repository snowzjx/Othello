#include "AppDelegate.h"
#include "./scene/MenuScene.h"
#include "./util/Singleton.h"
#include "./game/Othello.h"

USING_NS_CC;

typedef struct {
    Size size;
    std::string directory;
} Resource;

static const Resource smallResource = {Size(480, 320), "320p"};
static const Resource mediumResource = {Size(1024, 768), "768p"};
static const Resource largeResource = {Size(2048, 1536), "1536p"};
static const Size designResolutionSize = Size(480, 320);
static const std::string animationSearchPath = "animation";
static const std::string fontSearchPath = "fonts";

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
    Singleton<Othello>::destroyInstance();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();
    director->setOpenGLView(eglView);

    eglView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    
    std::vector<std::string> searchPath;
    Size frameSize = eglView->getFrameSize();

    if (frameSize.height > mediumResource.size.height) {
        searchPath.push_back(largeResource.directory);
        director->setContentScaleFactor(largeResource.size.height / designResolutionSize.height);
    } else if (frameSize.height > smallResource.size.height) {
        searchPath.push_back(mediumResource.directory);
        director->setContentScaleFactor(mediumResource.size.height / designResolutionSize.height);
    } else {
        searchPath.push_back(smallResource.directory);
        director->setContentScaleFactor(smallResource.size.height / designResolutionSize.height);
    }
	searchPath.push_back(animationSearchPath);
    searchPath.push_back(fontSearchPath);
    FileUtils::getInstance()->setSearchPaths(searchPath);
    
    Singleton<Othello>::getInstance()->setShouldShowMoveTip();
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = MenuLayer::createScene();
    
    // run
    director->runWithScene(scene);
        
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
