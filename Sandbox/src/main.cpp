#include <thsan.h>
#include <thsan/state/state_manager.h>
#include "state/cool_stuff_state.h"
#include "state/film_edit_state.h"

class SandBox : public Thsan::Game
{
public:
	SandBox() 
	{

	}

	~SandBox() 
	{
	}

	void onCreate() override 
	{
		stateManager->registerState<FilmEditState>("FilmEditState");
		while (!stateManager->isStateRegistered("FilmEditState"));
	}

	void onUICreate() override 
	{
		this->enableDebugUI();
	}

	void onUIRender () override 
	{

	}


private:
	
	
};


Thsan::Game* Thsan::create_game() 
{
	return new SandBox();
}