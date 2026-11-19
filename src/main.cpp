#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

static constexpr int GRAPE_LEVEL_ID = 152277985;

static void addGrapeFace(FLAlertLayer* alert) {
    auto face = CCSprite::create("grape.png"_spr);
    if (!face || !alert->m_mainLayer) return;
    face->setScale(60.f / face->getContentHeight());
    auto win = CCDirector::get()->getWinSize();
    face->setPosition({win.width / 2.f, win.height / 2.f + 110.f});
    alert->m_mainLayer->addChild(face, 10);
}

static void goToLevel() {
    auto glm = GameLevelManager::sharedState();

    bool alreadyDone = Mod::get()->getSavedValue<bool>("auto-liked", false);
#ifdef GEODE_IS_WINDOWS
    alreadyDone = alreadyDone ||
        glm->hasLikedItemFullCheck(LikeItemType::Level, GRAPE_LEVEL_ID, true, 0) ||
        glm->hasLikedItemFullCheck(LikeItemType::Level, GRAPE_LEVEL_ID, false, 0);
#endif

    if (!alreadyDone) {
        glm->likeItem(LikeItemType::Level, GRAPE_LEVEL_ID, true, 0);
        Mod::get()->setSavedValue("auto-liked", true);
    }

    auto level = GJGameLevel::create();
    level->m_levelID = GRAPE_LEVEL_ID;
    auto scene = LevelInfoLayer::scene(level, false);
    CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
}

static void showSecondDialog() {
    auto alert = createQuickPopup(
        "Grape", "Now play Do Better by Hahee now.", "Okay", nullptr,
        [](FLAlertLayer*, bool) { goToLevel(); }, false
    );
    addGrapeFace(alert);
    alert->show();
}

static void showFirstDialog() {
    auto alert = createQuickPopup(
        "Grape", "Greetings human...", "Next", nullptr,
        [](FLAlertLayer*, bool) { showSecondDialog(); }, false
    );
    addGrapeFace(alert);
    alert->show();
}

class $modify(GrapeMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto spr = CCSprite::create("grape.png"_spr);
        if (!spr) return true;
        spr->setScale(45.f / spr->getContentHeight());

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(GrapeMenu::onGrape)
        );
        btn->setID("grape-button"_spr);

        if (auto menu = this->getChildByID("bottom-menu")) {
            menu->addChild(btn);
            menu->updateLayout();
        }
        return true;
    }

    void onGrape(CCObject*) {
        FMODAudioEngine::sharedEngine()->playEffect("chestOpen01.ogg", 1.f, 1.f, 1.f);
        showFirstDialog();
    }
};
