#pragma once

#include "lost/Application.h"

namespace lost
{
struct AnimTest : public Application
{
vector<LayerPtr> layers;

AnimTest();
void startup();
void update();
void shutdown();

};
}

