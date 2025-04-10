#include "kawa.h"

#include "example_project/stages/main_menu/logic.h"
#include "example_project/stages/shooter/logic.h"

int main()
{	
	kawa::application::create("kawa", 3)
		.scene_from_prefab(0, main_menu::prefab)
		.scene_from_prefab(1, shooter::prefab)
		.run();
}