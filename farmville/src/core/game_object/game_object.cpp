#include "game_object.h"
#include <iostream>
#include <algorithm>

#include "component/input_component.h"
#include "component/render_component.h"
#include "component/physic_component.h"

#include <thsan/input/control_setting.h>
#include "thsan/scene/scene.h"

GameObject::GameObject(Scene &scene, InputComponent* inputComp, PhysicComponent* physicComp, RenderComponent* renderComp, std::string name):
    m_inputComponent(inputComp),
    m_physicComponent(physicComp),
    m_renderComponent(renderComp)
{
    m_ID = s_IDcounter++;
    m_name = name;

    if(!hasComponent(DataType::TRANSFORM))
        setComponent(DataType::TRANSFORM, new Transform());

    transform = getComponent<Transform>(DataType::TRANSFORM);

    if(m_inputComponent)
        m_inputComponent->init(scene);
    if(m_physicComponent)
        m_physicComponent->init(scene);
    if(m_renderComponent)
        m_renderComponent->init(scene);

    m_parent = nullptr;

    // impose a Transform comp so that every single
    // game object can fit in the scene graph in peace
    // and harmony.
}

GameObject::GameObject(const GameObject& other)
{

    m_inputComponent = new InputComponent();
    *m_inputComponent = *other.m_inputComponent;

    m_physicComponent = new PhysicComponent();
    *m_physicComponent = *other.m_physicComponent;

    m_renderComponent = new RenderComponent();
    *m_renderComponent = *other.m_renderComponent;

    m_componentDataHolder = other.m_componentDataHolder;
}

void GameObject::updateInput(Scene& scene, const float& delta, const std::vector<Thsan::InputAction>& inputActions)
{
    if(m_inputComponent)
        m_inputComponent->update( scene, delta, inputActions);
}

void GameObject::updatePhysic(Scene& scene, const float& delta)
{
    if(m_physicComponent)
        m_physicComponent->update( scene, delta);
}

void GameObject::updateRender(Scene& scene, const float& delta, Thsan::RendererTerrain& rendererTerrain)
{
    if (m_renderComponent) {
        static bool OnStart = true;
        if (OnStart) {
            OnStart = false;
            m_renderComponent->onStart( scene, rendererTerrain);
        }
        m_renderComponent->update( scene, delta, rendererTerrain);
        //m_renderComponent->callEvent( scene, delta);
    }
}

void GameObject::updateNativeScript(Scene& scene, const float& delta)
{
    if (m_nativeScripComponent)
        m_nativeScripComponent->update( scene, delta);
}

void GameObject::onDelete(Scene& scene)
{
    if (m_inputComponent)
        m_inputComponent->onDelete(scene);
    if (m_physicComponent)
        m_physicComponent->onDelete(scene);
    if (m_renderComponent)
        m_renderComponent->onDelete(scene);
    if (m_nativeScripComponent)
        m_renderComponent->onDelete(scene);
}

std::vector<std::string> GameObject::getGroupName()
{
    return groups;
}

void GameObject::setInput(InputComponent* inputComp, Scene& scene)
{
    m_inputComponent = inputComp;
    m_inputComponent->parent = this;
    m_inputComponent->init(scene);
}

InputComponent* GameObject::getInput()
{
    return m_inputComponent;
}

void GameObject::setPhysic(PhysicComponent* physicComp, Scene& scene)
{
    m_physicComponent = physicComp;
    m_physicComponent->parent = this;
    m_physicComponent->init(scene);
}

PhysicComponent* GameObject::getPhysic()
{
    return m_physicComponent;
}

void GameObject::setRender(RenderComponent* renderComp, Scene& scene)
{
    m_renderComponent = renderComp;
    m_renderComponent->parent = this;
    m_renderComponent->init(scene);
}

void GameObject::add_group(const std::string& name)
{
    if (groups_index.count(name) == 0) {
        groups.push_back(name);
        groups_index[name] = groups.end() - 1;
    }
}

void GameObject::remove_group(const std::string& name)
{
    if (groups_index.count(name) > 0) {
        groups.erase(groups_index[name]);
        groups_index.erase(name);
    }
}

bool GameObject::has_group(const std::string& name)
{
    if (groups_index.count(name) > 0)
        return true;
    else
        return false;
}

RenderComponent* GameObject::getRender()
{
    return m_renderComponent;
}

bool GameObject::isInputActive()
{
    return m_inputComponent->isActive();
}

void GameObject::disableInput()
{
    m_inputComponent->disable();
}

void GameObject::enableInput()
{
    m_inputComponent->enable();
}

bool GameObject::isPhysicActive()
{
    return m_physicComponent->isActive();
}

void GameObject::disablePhysic()
{
    m_physicComponent->disable();
}

void GameObject::enablePhysic()
{
    m_physicComponent->enable();
}

bool GameObject::isRenderActive()
{
    return m_renderComponent->isActive();
}

void GameObject::disableRender()
{
    m_renderComponent->disable();
}

void GameObject::enableRender()
{
    m_renderComponent->enable();
}

bool GameObject::isNativeScriptActive()
{
    return m_nativeScripComponent->isActive();
}

void GameObject::disableNativeScriptRender()
{
    m_nativeScripComponent->disable();
}

void GameObject::enableNativeScriptRender()
{
    m_nativeScripComponent->enable();
}



bool GameObject::hasComponent(DataType type)
{
    return m_componentDataHolder.hasComponent(type);
}

void GameObject::setComponent(DataType target, Data* comp)
{
    m_componentDataHolder.setComponent(target, *comp);
}

std::string GameObject::getName()
{
    return m_name;
}

void GameObject::setName(std::string name)
{
    m_name = name;
}

GameObject* GameObject::getParent()
{
    return m_parent;
}

Transform GameObject::getWorldTransform()
{
    Transform world_transform;

    world_transform.position = transform->position;
    world_transform.scale = transform->scale;
    world_transform.rotation = transform->rotation;

    Transform* curr_parent = nullptr;
    if (transform->parent)
        curr_parent = transform->parent;

    while (curr_parent->parent) {
        world_transform.position += curr_parent->position;
        world_transform.scale = curr_parent->scale;
        world_transform.rotation = curr_parent->rotation;
        curr_parent = curr_parent->parent;
    }

    return world_transform;
}
bool GameObject::hasChild(GameObject* go)
{
    if (std::find(m_children.begin(), m_children.end(), go) != m_children.end())
        return true;
    else
        for (GameObject* child : m_children)
            hasChild(child);
    return false;
}
void GameObject::addChild(GameObject* go)
{
    if (!hasChild(go)) {
        Transform* child_transform = go->getComponent<Transform>(DataType::TRANSFORM);
        child_transform->parent = this->getComponent<Transform>(DataType::TRANSFORM);
        if (go->getParent() != nullptr) {
            go->getParent()->removeChild(go->getName());
        }
        go->setParent(this);
        m_children.emplace_back(go);
        m_childrenMap[go->getName()] = go;
    }
}

GameObject* GameObject::getChild(const std::string& name)
{
    if (m_childrenMap.find(name) != m_childrenMap.end())
        return m_childrenMap.at(name);
    else
        return nullptr;
}

std::vector<GameObject*> GameObject::getChildren()
{
    return m_children;
}

void GameObject::removeChild(const std::string& name)
{
    std::unordered_map<std::string, GameObject*>::iterator child_location = m_childrenMap.find(name);
    if (child_location != m_childrenMap.end()) {
        GameObject* child = m_childrenMap[name];
        Transform* child_transform = child->getComponent<Transform>(DataType::TRANSFORM);
        child_transform->parent = nullptr;
        child->setParent(nullptr);

        m_children.erase(
            std::remove_if(
                m_children.begin(), m_children.end(),
                [&name](GameObject* go) { return go->getName() == name; }),
            m_children.end()
        );
        m_childrenMap.erase(child_location);
    }
}

void GameObject::removeAllChild(std::string name)
{
    for (GameObject* child : m_children) {
        Transform* child_transform = child->getComponent<Transform>(DataType::TRANSFORM);
        child_transform->parent = nullptr;
        child->setParent(nullptr);
    }
    m_children.clear();
    m_childrenMap.clear();
}

void GameObject::setParent(GameObject* go)
{
    if (this != go && getChild(go->getName()) == nullptr) {
        m_parent = go;
    }
}

bool GameObject::hasScript()
{
    return ownScript;
}

NativeScriptComponent* GameObject::createScriptInstance()
{
    m_nativeScripComponent = InstantiateScript();
    return m_nativeScripComponent;
}

NativeScriptComponent* GameObject::getScriptInstance()
{
    return m_nativeScripComponent;
}


GameObject::~GameObject()
{
    if(m_inputComponent)
        delete m_inputComponent;
    if(m_physicComponent)
        delete m_physicComponent;
    if(m_renderComponent)
        delete m_renderComponent;
}

bool GameObject::hasRenderComponent()
{
    if (m_renderComponent)
        return true;
    else
        return false;
}

uint32_t GameObject::s_IDcounter = 0;
