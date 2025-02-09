#pragma once

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

