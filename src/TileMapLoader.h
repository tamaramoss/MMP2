#pragma once

#include <string>

#include "SpriteManager.h"
#include "GameObjectManager.h"

class NLTmxMap;

void loadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
    SpriteManager& spriteManager);

void loadObjectLayers(NLTmxMap* tilemap, const std::string& resourcePath,
    SpriteManager& spriteManager);
