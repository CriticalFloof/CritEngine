#include "SceneManager.h"

namespace Engine {

	SceneManager::SceneManager()
		: baseScene(std::make_shared<Scene>())
	{
	}
}