#pragma once

#include <string>

#include "SpriteManager.h"
#include "TGuiWrapper.h"
#include "GameObjectManager.h"

class NLTmxMap;

void loadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
    SpriteManager& spriteManager, TGuiWrapper* guiManager = nullptr);

void loadObjectLayers(NLTmxMap* tilemap, const std::string& resourcePath,
    SpriteManager& spriteManager, TGuiWrapper* guiManager = nullptr);