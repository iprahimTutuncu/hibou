#pragma once
#include <variant>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include "octree.h"
#include "../game_object/game_object.h"
#include "../Scene.h"

struct Volume{
	Transform* transform;
	HitBox* hitBox;
};

class GameObject;
class Scene;

class CollisionEngine
{
public:
	CollisionEngine();
	~CollisionEngine() = default;

	static void add(GameObject* go, Transform* transform, HitBox* hitBox = nullptr);
	static void add(const std::string& name, Transform* transform, HitBox* HitBox = nullptr);

	static void remove(GameObject* go);
	static void remove(const std::string& name);

	// QUERY CALL WITH TRANSFORM ONLY
	//name of function goes by: query, quantity, order, type, constraint [param]
	static GameObject* getOneClosestGameObjectWithinRadius(const Transform* transform, float radius, float bottom, float height, Collision_object object);
	static std::vector<GameObject*> getAllClosestGameObjectWithinRadius(const Transform* transform, float radius, float bottom, float height, Collision_object object);
	static std::vector<GameObject*> getAllOverlapingGameObjectWithGroups(GameObject* go, const std::initializer_list<std::string> groups);
	static std::vector<GameObject*> getAllOverlapingGameObjectWithGroups(const std::string& name, const std::initializer_list<std::string> groups);
	static bool overlap(GameObject* a, GameObject* b);
	static bool overlap(GameObject* a, const std::string& b);
	static bool overlap(const std::string& a, GameObject* b);
	static bool overlap(const std::string& a, const std::string& b);

private:
	static void bind(Scene* scene);
	static void remove(Scene* scene);
	static void update(float& dt);
	
	static std::unordered_map<Scene*, std::unordered_map<GameObject*, Volume>> go_volumes;
	static std::unordered_map<Scene*, std::unordered_map<std::string, Volume>> name_volumes;

	static Scene* curr_scene;
	friend class Scene;

};

