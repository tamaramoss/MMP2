/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include <memory>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>



#include "Animation.h"
#include "AnimationComponent.h"
#include "SpriteManager.h"
#include "GuiManager.h"
#include "GameObjectManager.h"
#include "NLTmxMap.h"
#include "SpriteRenderComponent.h"

class NLTmxMap;

class TileMapManager
{
public:
    static TileMapManager& getInstance();
	
    void loadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
        SpriteManager& spriteManager, GuiManager* guiManager = nullptr);


    void loadObjectLayers(NLTmxMap* tilemap, const std::string& resourcePath,
                          SpriteManager& spriteManager, GuiManager* guiManager = nullptr);

private:
	struct Player
	{
		NLTmxMapObject* PlayerBody;
		NLTmxMapObject* PlayerHandLeft;
		NLTmxMapObject* PlayerHandRight;
	};

    vector<NLTmxMapObject*> mSpitterBody;
    vector<NLTmxMapObject*> mSpitterTrigger;

    float degreesToRadians(float angleDegrees) { return (angleDegrees) * M_PI / 180.f; }
    float radiansToDegrees(float angleRadians) { return (angleRadians) * 180.f / M_PI; }
   
    GameObject::ptr loadDeadOverlay(SpriteManager& spriteManager);
    GameObject::ptr loadWinOverlay(SpriteManager& spriteManager);
    GameObject::ptr createHand(NLTmxMapObject* object, const std::string& layer, GameObject::ptr parent, int index,
        float speed, Vector2f startPos, float distanceFromStart, SpriteManager& spriteManager);

    GameObject::ptr loadCollider(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
                                 SpriteManager& spriteManager);
    GameObject::ptr loadGoalTrigger(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
        SpriteManager& spriteManager);
    GameObject::ptr loadTrigger(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
                                SpriteManager& spriteManager);
    GameObject::ptr loadRock(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
                             SpriteManager& spriteManager);
    GameObject::ptr loadLava(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
        SpriteManager& spriteManager);
    GameObject::ptr loadSpitter(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
                                SpriteManager& spriteManager);
    GameObject::ptr loadSpitterTrigger(NLTmxMapObject* object, const std::string& layer,
                                       const std::string& resourcePath,
                                       SpriteManager& spriteManager);
    void setupSpitter(const std::string& layer, const std::string& resourcePath, SpriteManager& spriteManager);
    void setupSpitter();
	
    GameObject::ptr loadButton(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
                               SpriteManager& spriteManager, GuiManager* guiManager);
    GameObject::ptr makePlayer(::TileMapManager::Player playerStruct, const std::string& layer,
        const std::string& resourcePath, SpriteManager& spriteManager);

    Animation::ptr makeAnimation(int rectLeft, int rectTop, int frameWidth, int frameHeight, std::string path);
    std::shared_ptr<AnimationComponent> makeAnimationComponent(NLTmxMapObject* object, std::shared_ptr<GameObject> gameObject, string layer, SpriteManager& spriteManager, std::vector
                                                               <std::string> animationNames, string ressourcePath, float frameTime = 0.2f, bool paused = false, bool looped = true, string
                                                               texturePathOptional = "");
    std::shared_ptr<SpriteRenderComponent> makeRenderComponent(NLTmxMapObject* object, std::shared_ptr<GameObject> gameObject, string layer, SpriteManager& spriteManager, string texturePathOptional = "");
    void makePhysics(GameObject::ptr gameObject, bool isKinematic, float sizeFactor = 1.0f, bool isStatic = false, bool isSensor = false, float density = 1.f, float friction = 0.f);
    void makePhysicsWithObject(NLTmxMapObject* object, GameObject::ptr gameObject, bool isSensor = false);
};

