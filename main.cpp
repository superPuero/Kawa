#include "kawa.h"

#include "test_project/scripts/main_menu/logic.h"
#include "test_project/scripts/game/logic.h"

int main()
{	
	kawa::application& app = kawa::application::create("test", 3);

	app.scene_from_prefab(0, main_menu::prefab);
	app.scene_from_prefab(1, game::prefab);

	app.run();
}