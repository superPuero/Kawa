#include "kawa.h"

#include "example_project/stages/main_menu/logic.h"
#include "example_project/stages/shooter/logic.h"
#include "example_project/stages/3d_test/logic.h"
#include "example_project/stages/swarm/swarm_scene.h"

int main()
{	
	kawa::app::create
	({
	   .name = "Kawa",
	   .width = 1200,
	   .height = 800,
	   .vsync = true	
	})
	.with_scene("swarm", swarm::prefab)
	//.with_scene("3dtest", test3d::prefab)
	.run();
}