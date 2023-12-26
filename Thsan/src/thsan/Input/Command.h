#ifndef COMMAND_H
#define COMMAND_H

#include <SFML/System/Time.hpp>

#include "../scene/Scene.h"
#include "../gameObject/GameObject.h"


class GameObject;
class Scene;

class  Command
{
    public:
        Command() = default;
        virtual ~Command(){};

        virtual void execute(GameObject &parent, Scene &scene, const float& dt){};
        virtual void undo(GameObject &parent, Scene &scene, const float& dt){};

};

#endif // BASECOMMAND_H
