#include <thsan.h>
#include "state/cool_stuff_state.h"

class SandBox : public Thsan::Game
{
public:
	SandBox() {
	}

	~SandBox() {
		logManager.close();
	}

	void onCreate() override {
		state = new CoolStuffState(this);
	}

	void onUICreate() override {
		this->enableDebugUI();
	}

	void onUIRender () override {

	}


private:
	
	
};


Thsan::Game* Thsan::create_game() 
{
	return new SandBox();
}