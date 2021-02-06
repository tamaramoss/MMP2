#pragma once

#include <memory>
#include <string>


#include "Animation.h"
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

	struct Spitter
	{
		NLTmxMapObject* SpitterBody;
		NLTmxMapObject* SpitterTrigger;
		int index;
	};


    vector<NLTmxMapObject*> mSpitters;
    vector<NLTmxMapObject*> mSpitterTriggers;

    Animation::ptr makeAnimation(int rectLeft, int rectTop, int frameWidth, std::string path);
    std::shared_ptr<SpriteRenderComponent> makeRenderComponent(NLTmxMapObject* object,
        std::shared_ptr<GameObject> gameObject, string layer,
        SpriteManager& spriteManager,
        string texturePathOptional);
    GameObject::ptr loadDeadOverlay(SpriteManager& spriteManager);
    GameObject::ptr loadWinOverlay(SpriteManager& spriteManager);
    GameObject::ptr createHand(NLTmxMapObject* object, const std::string& layer, GameObject::ptr parent, int index,
        float speed, Vector2f startPos, float distanceFromStart, SpriteManager& spriteManager);
    void makePhysics(GameObject::ptr gameObject, bool isKinematic, float sizeFactor);
    GameObject::ptr loadCollider(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
        SpriteManager& spriteManager);
    GameObject::ptr loadGoalTrigger(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
        SpriteManager& spriteManager);
    GameObject::ptr loadRock(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
        SpriteManager& spriteManager);
    GameObject::ptr loadLava(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
        SpriteManager& spriteManager);
    void loadSpitter(const std::string& layer, const std::string& resourcePath, SpriteManager& spriteManager);
    GameObject::ptr loadButton(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
        SpriteManager& spriteManager, GuiManager* guiManager);
    GameObject::ptr makePlayer(::TileMapManager::Player playerStruct, const std::string& layer,
        const std::string& resourcePath, SpriteManager& spriteManager);
};

