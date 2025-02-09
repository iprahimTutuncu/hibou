#include <thsan.h>
#include <thsan/state/state_manager.h>
#include "state/farmville_state.h"
class FarmGame : public Thsan::Game
{
public:
	FarmGame()
	{

	}

	~FarmGame()
	{

	}

	void onCreate() override 
	{
		stateManager->registerState<FarmVilleState>("FarmvilleState");
		while (!stateManager->isStateRegistered("FarmvilleState"));
	}

	void onUICreate() override 
	{
	}

	void onUIRender () override 
	{

	}


private:
	
	
};

Thsan::Game* Thsan::create_game() 
{
	return new FarmGame();
}