#include "kawa.h"

#include "test_project/scripts/main_menu/logic.h"
//#include "test_project/scripts/game/logic.h"
#include "test_project/scripts/shooter/logic.h"

int main()
{	
	kawa::application::create("kawa", 3)
		.scene_from_prefab(0, main_menu::prefab)
		.scene_from_prefab(1, shooter::prefab)
		.run();
}