#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <glm/fwd.hpp>
#include <thsan/graphics/renderer/renderer_terrain.h>


class GameObject;
class InputComponent;
class PhysicComponent;
class RenderComponent;
class CollisionEngine;

namespace Thsan
{
    enum InputAction;
    class State;
}

namespace tsm
{
    class Color;
}

class Scene
{
    public:
        Scene(Thsan::State* parent);

        virtual ~Scene();
        Scene (const Scene& scene) = delete;

        void input(const float& dt, std::vector<Thsan::InputAction> inputActions);
        void update(const float& dt);
        void render(const float& dt, Thsan::RendererTerrain& renderer);

        //scene graph related method
        std::vector<GameObject*> get_children();
        GameObject* get_child(const std::string& name);
        void delete_child(const std::string& name);
        void delete_all_child();
        bool has_child(const std::string& name);

        float getDeltaTime();
        GameObject* createGameObject(const std::string& name);

        template<typename T, typename... Args>
        void setGameObjectInputComponent(GameObject* go, Args... args) {
            static_assert(std::is_base_of<InputComponent, T>::value, "T must derive from InputComponent");

            InputComponent* inputComp = new T(args...);
            setInput(go, inputComp);

            if (m_inputComponentsMap.count(go) > 0){
                int position_in_vector = m_inputComponentsMap[go];
                InputComponent* prev_inputComp = m_inputComponents[position_in_vector];
                m_inputComponents[position_in_vector] = inputComp;
                callOnDelete(prev_inputComp);
                delete prev_inputComp;
            }
            else {
                m_inputComponents.push_back(inputComp);
                m_inputComponentsMap[go] = m_inputComponents.size() - 1;
            }

            updateGameObjectComponent(go);
        }

        template<typename T, typename... Args>
        void setGameObjectPhysicComponent(GameObject* go, Args... args) {
            static_assert(std::is_base_of<PhysicComponent, T>::value, "T must derive from PhysicComponent");

            PhysicComponent* physicComp = new T(args...);
            setPhysic(go, physicComp);

            if (m_physicComponentsMap.count(go) > 0) {
                int position_in_vector = m_physicComponentsMap[go];
                PhysicComponent* prev_physicComp = m_physicComponents[position_in_vector];
                m_physicComponents[position_in_vector] = physicComp;
                callOnDelete(prev_physicComp);
                delete prev_physicComp;
            }
            else {
                m_physicComponents.push_back(physicComp);
                m_physicComponentsMap[go] = m_physicComponents.size() - 1;
            }

            updateGameObjectComponent(go);
        }

        template<typename T, typename... Args>
        void setGameObjectRenderComponent(GameObject* go, Args... args) {
            static_assert(std::is_base_of<RenderComponent, T>::value, "T must derive from RenderComponent");

            RenderComponent* renderComp = new T(args...);
            setRender(go, renderComp);

            if (m_renderComponentsMap.count(go) > 0) {
                int position_in_vector = m_renderComponentsMap[go];
                RenderComponent* prev_renderComp = m_renderComponents[position_in_vector];
                m_renderComponents[position_in_vector] = renderComp;
                callOnDelete(prev_renderComp);
                delete prev_renderComp;
            }
            else {
                m_renderComponents.push_back(renderComp);
                m_renderComponentsMap[go] = m_renderComponents.size() - 1;
            }

            updateGameObjectComponent(go);
        }
        
        template<typename T, typename... Args>
        void setGameObjectInputComponent(const std::string& name, Args... args) {
            setGameObjectInputComponent<T>(m_childrenMap[name], args...);
        }

        template<typename T, typename... Args>
        void setGameObjectPhysicComponent(const std::string& name, Args... args) {
            setGameObjectPhysicComponent<T>(m_childrenMap[name], args...);
        }

        template<typename T, typename... Args>
        void setGameObjectRenderComponent(const std::string& name, Args... args) {
            setGameObjectRenderComponent<T>(m_childrenMap[name], args...);
        }

        template <typename T>
        void setGameObjectNativeScriptComponent(const std::string& name) {
            setGameObjectNativeScriptComponent<T>(m_childrenMap[name]);
        }

        std::variant<float, glm::vec3, tsm::Color> sampleTerrain(Thsan::TerrainDataType type, glm::vec2 position);

        void addGameObject(GameObject* go);

        Thsan::State* getParent();
        void setParent(Thsan::State* parent);

        void addToGroups(GameObject* p_go, const std::initializer_list<std::string> groups);
        void removeFromGroups(GameObject* p_go, const std::initializer_list<std::string> groups);
        std::vector<GameObject*> getAllGameObjectFromGroup(const std::string& group_name);
        std::vector<GameObject*> getAllGameObjectFromGroups(const std::initializer_list<std::string> groups);

    private:
        void updateGameObjectComponent(GameObject* go);
        void setInput(GameObject* go, InputComponent* comp);
        void setPhysic(GameObject* go, PhysicComponent* comp);
        void setRender(GameObject* go, RenderComponent* comp);

        void callOnDelete(InputComponent* ic);
        void callOnDelete(PhysicComponent* pc);
        void callOnDelete(RenderComponent* rc);

        std::vector<std::string> to_delete_names;

        std::vector<GameObject*> m_children;
        std::unordered_map<std::string, std::vector<GameObject*>> groups_map;
        std::unordered_map<std::string, std::unordered_map<GameObject*, std::vector<GameObject*>::iterator>> map_for_groups_gameObject_index;

        std::vector<InputComponent*> m_inputComponents;
        std::vector<PhysicComponent*> m_physicComponents;
        std::vector<RenderComponent*> m_renderComponents;

        std::unordered_map<GameObject*, int> m_inputComponentsMap;
        std::unordered_map<GameObject*, int> m_physicComponentsMap;
        std::unordered_map<GameObject*, int> m_renderComponentsMap;
        std::unordered_map<GameObject*, int> m_nativeScriptComponentsMap;

        std::unordered_map<std::string, GameObject*> m_childrenMap;

        Thsan::State* parent;
        float m_deltaTime{0.0};

        std::unique_ptr<CollisionEngine> collisionEngine;
};
