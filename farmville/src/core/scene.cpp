#include "core/scene.h"
#include <thsan/input/control_setting.h>
#include "core/game_object/data/data.h"
#include "core/game_object/game_object.h"
#include "core/game_object/component/Input_component.h"
#include "core/game_object/component/physic_component.h"
#include "core/game_object/component/render_component.h"
#include "core/game_object/component/native_script_component.h"
#include "collision_engine/collision_engine.h"
#include <thsan/state/state.h>
#include <thsan/game.h>
#include <tsm/math/color.h>

Scene::Scene(Thsan::State* parent)
{
    this->parent = parent;
    CollisionEngine::bind(this);

}

void Scene::updateGameObjectComponent(GameObject* go)
{
    Transform* transform = nullptr;
    HitBox* hitBox = nullptr;
    if (go->hasComponent(DataType::TRANSFORM))
        transform = go->getComponent<Transform>(DataType::TRANSFORM);
    if (go->hasComponent(DataType::HITBOX))
        hitBox = go->getComponent<HitBox>(DataType::HITBOX);

    /*
        collisionEngine can only have one pair of Transform and HitBox
        for any GameObject. Don't worry that much. Delete the hitBox in
        specific component on the onDelete if you want to remove a
        previous collision or something when you set a new component.
    */
    collisionEngine->add(go, transform, hitBox);
}

void Scene::setInput(GameObject* go, InputComponent* comp)
{
    go->setInput(comp, *this);
}

void Scene::setPhysic(GameObject* go, PhysicComponent* comp)
{
    go->setPhysic(comp, *this);
}

void Scene::setRender(GameObject* go, RenderComponent* comp)
{
    go->setRender(comp, *this);
}

void Scene::callOnDelete(InputComponent* ic)
{
    ic->onDelete(*this);
}

void Scene::callOnDelete(PhysicComponent* pc)
{
    pc->onDelete(*this);
}

void Scene::callOnDelete(RenderComponent* rc)
{
    rc->onDelete(*this);
}


void Scene::input(const float& dt, std::vector<Thsan::InputAction> inputActions)
{
    //CollisionEngine::bind(this);

    for (int i = 0; i < m_inputComponents.size(); i++) {
        InputComponent* inputComp = m_inputComponents[i];
        if (inputComp) {
            if (inputComp->parent && inputComp->isActive())
                inputComp->update(*this, dt, inputActions);
        }
    }
}

void Scene::update(const float& dt){
    
    //CollisionEngine::bind(this);

    for (int i = 0; i < m_children.size(); i++) {
        GameObject* go = m_children[i];
        if (go->hasScript()) {
            NativeScriptComponent* script = go->getScriptInstance();

            if (!script) {
                script = go->createScriptInstance();
                script->parent = go;
                script->init(*this);
            }

            if (script->isActive()) {
                script->update(*this, dt);
            }
        }
    }

    for (int i = 0; i < m_physicComponents.size(); i++) {
        PhysicComponent* physicComp = m_physicComponents[i];
        if (physicComp)
            if (physicComp->parent && physicComp->isActive())
                physicComp->update(*this, dt);
    }

}

void Scene::render(const float& dt, Thsan::RendererTerrain& renderer) {
    CollisionEngine::bind(this);

    for (int i = 0; i < m_renderComponents.size(); i++) {
        RenderComponent* renderComp = m_renderComponents[i];
        if (renderComp)
            if (renderComp->parent && renderComp->isActive())
            {
                renderComp->update(*this, dt, renderer);
            }
    }
}

float Scene::getDeltaTime()
{
    return m_deltaTime;
}

std::vector<GameObject*> Scene::get_children()
{
    return m_children;
}


GameObject* Scene::createGameObject(const std::string& name)
{
    CollisionEngine::bind(this);

    GameObject* go = new GameObject(*this, nullptr, nullptr, nullptr, name);
    m_children.emplace_back(go);
    m_childrenMap[name] = go;

    return go;
}

std::variant<float, glm::vec3, tsm::Color> Scene::sampleTerrain(Thsan::TerrainDataType type, glm::vec2 position)
{
    return parent->getParent()->sampleTerrain(type, position);
}

void Scene::addGameObject(GameObject* go)
{
    CollisionEngine::bind(this);

    std::string name = go->getName();
    std::unordered_map<std::string, GameObject*>::iterator child_location = m_childrenMap.find(name);
    if (child_location == m_childrenMap.end()) {
        m_children.emplace_back(go);
        m_childrenMap[name] = go;

        m_inputComponents.push_back(go->getInput());
        m_physicComponents.push_back(go->getPhysic());
        m_renderComponents.push_back(go->getRender());
    }
}

GameObject* Scene::get_child(const std::string& name)
{
	if(m_childrenMap.find(name) != m_childrenMap.end())
	    return m_childrenMap.at(name);
	else
	    return nullptr;
}

void Scene::delete_child(const std::string& name)
{
    std::unordered_map<std::string, GameObject*>::iterator child_location = m_childrenMap.find(name);
    if (child_location != m_childrenMap.end()) {
        GameObject* go = m_childrenMap[name];
        m_inputComponents.erase(
            std::remove_if(
                m_inputComponents.begin(), m_inputComponents.end(),
                [&go](InputComponent* ic) { return ic == go->getInput(); }),
            m_inputComponents.end()
        );

        m_physicComponents.erase(
            std::remove_if(
                m_physicComponents.begin(), m_physicComponents.end(),
                [&go](PhysicComponent* pc) { return pc == go->getPhysic(); }),
            m_physicComponents.end()
        );

        m_renderComponents.erase(
            std::remove_if(
                m_renderComponents.begin(), m_renderComponents.end(),
                [&go](RenderComponent* rc) { return rc == go->getRender(); }),
            m_renderComponents.end()
        );

        m_childrenMap[name]->onDelete(*this);

        collisionEngine->remove(m_childrenMap[name]);

        m_children.erase(
            std::remove_if(
                m_children.begin(), m_children.end(),
                [&name](GameObject* go) { return go->getName() == name; }),
            m_children.end()
        );

        delete m_childrenMap[name];
        m_childrenMap.erase(child_location);
    }
}

void Scene::delete_all_child()
{
    for (auto child : m_children) {
        child->onDelete(*this);
        collisionEngine->remove(child);
        delete child;
    }

    m_children.clear();
    m_childrenMap.clear();
}

bool Scene::has_child(const std::string& name)
{
    if (m_childrenMap.find(name) != m_childrenMap.end())
        return true;
    else
        return false;
}


Thsan::State* Scene::getParent()
{
    return parent;
}

void Scene::setParent(Thsan::State* parent)
{
    this->parent = parent;
}

void Scene::addToGroups(GameObject* p_go, const std::initializer_list<std::string> groups)
{
    for (const std::string& gr_name : groups) {
        if (!p_go->has_group(gr_name)) {
            p_go->add_group(gr_name);
            groups_map[gr_name].push_back(p_go);
            map_for_groups_gameObject_index[gr_name][p_go] = groups_map[gr_name].end() - 1;
        }
    }
}

void Scene::removeFromGroups(GameObject* p_go, const std::initializer_list<std::string> groups)
{
    for (const std::string& gr_name : groups) {
        if (map_for_groups_gameObject_index.count(gr_name) > 0) {
            groups_map[gr_name].erase(map_for_groups_gameObject_index[gr_name][p_go]);
            p_go->remove_group(gr_name);
            map_for_groups_gameObject_index[gr_name].erase(p_go);
        }
    }
}

std::vector<GameObject*> Scene::getAllGameObjectFromGroup(const std::string& group_name)
{
    std::vector<GameObject*> gameObjects;
    if (groups_map.count(group_name) > 0)
        gameObjects = groups_map[group_name];
    return gameObjects;
}

std::vector<GameObject*> Scene::getAllGameObjectFromGroups(const std::initializer_list<std::string> groups)
{
    std::vector<GameObject*> gameObjects;
    for(std::string group_name: groups)
        if (groups_map.count(group_name) > 0) {
            std::vector<GameObject*> temp = groups_map[group_name];
            gameObjects.insert(std::end(gameObjects), std::begin(temp), std::end(temp));
        }
    return gameObjects;
}

Scene::~Scene()
{
    for(auto& child: m_children)
        delete child; 

    m_children.clear();
}
