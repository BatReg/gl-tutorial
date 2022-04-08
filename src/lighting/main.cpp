#include "lighting.h"

#include <entry_point.h>

Application* CreateApplication()
{
    ApplicationCreateInfo info{};
    info.title = "Lighting";

    return new LightingTutorial(info);
}
