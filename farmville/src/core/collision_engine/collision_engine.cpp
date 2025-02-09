#include "collision_engine.h"
#include <glm/glm.hpp>
#include <iostream>

CollisionEngine::CollisionEngine()
{
}

void CollisionEngine::add(GameObject* go, Transform* transform, HitBox* hitBox)
{
	if(curr_scene)
		go_volumes[curr_scene][go] = Volume{ transform, hitBox };

}

void CollisionEngine::add(const std::string& name, Transform* transform, HitBox* hitBox)
{
	if(curr_scene)
		name_volumes[curr_scene][name] = Volume{ transform, hitBox };
}

void CollisionEngine::remove(GameObject* go)
{
	if(curr_scene)
		go_volumes[curr_scene].erase(go);
}

void CollisionEngine::remove(const std::string& name)
{
	if(curr_scene)
		name_volumes[curr_scene].erase(name);
}

void CollisionEngine::bind(Scene* scene)
{
	curr_scene = scene;
}

void CollisionEngine::remove(Scene* scene)
{
	auto got = go_volumes.find(scene);
	if (got != go_volumes.end()) {
		got->second.clear();
		go_volumes.erase(got->first);
	}
}

bool interesct(const glm::vec3& point, const AABB& box) {
	return (point.x >= box.min.x + box.position.x && point.x <= box.max.x + box.position.x) &&
		(point.y >= box.min.y + box.position.y && point.y <= box.max.y + box.position.y) &&
		(point.z >= box.min.z + box.position.z && point.z <= box.max.z + box.position.z);
}

bool interesct(const glm::vec3& point, const Sphere& sphere) {
	// we are using multiplications because is faster than calling Math.pow
	float distance = glm::sqrt(
		(point.x - sphere.position.x) * (point.x - sphere.position.x) +
		(point.y - sphere.position.y) * (point.y - sphere.position.y) +
		(point.z - sphere.position.z) * (point.z - sphere.position.z)
	);

	return distance < sphere.radius;
}


bool interesct(const AABB& a,const AABB& b) {
	return (a.min.x + a.position.x <= b.max.x + b.position.x && a.max.x + a.position.x >= b.min.x + b.position.x) &&
		   (a.min.y + a.position.y <= b.max.y + b.position.y && a.max.y + a.position.y >= b.min.y + b.position.y) &&
		   (a.min.z + a.position.z <= b.max.z + b.position.z && a.max.z + a.position.z >= b.min.z + b.position.z);
}

bool interesct(const Sphere& sphere, const Sphere& other) {
	glm::vec3 s = sphere.position;
	glm::vec3 o = other.position;

	float distance = glm::sqrt(
		(s.x - o.x) * (s.x - o.x) +
		(s.y - o.y) * (s.y - o.y) +
		(s.z - o.z) * (s.z - o.z)
	);

	return distance < (sphere.radius + other.radius);
}


bool interesct(const Sphere& sphere,const AABB& box) {

	// get box closest point to sphere center by clamping
	float x = glm::max(box.min.x + box.position.x, glm::min(sphere.position.x, box.max.x + box.position.x));
	float y = glm::max(box.min.y + box.position.y, glm::min(sphere.position.y, box.max.y + box.position.y));
	float z = glm::max(box.min.z + box.position.z, glm::min(sphere.position.z, box.max.z + box.position.z));

	// this is the same as isPointInsideSphere
	float distance = glm::sqrt(
		(x - sphere.position.x) * (x - sphere.position.x) +
		(y - sphere.position.y) * (y - sphere.position.y) +
		(z - sphere.position.z) * (z - sphere.position.z)
	);

	return distance < sphere.radius;
}


void CollisionEngine::update(float& dt)
{

}

bool overlapsVerticalCylinder(float* distance, glm::vec3 position, float radius, float bottom, float height, glm::vec3 destination) {
	bool success = false;
	if (position.y + bottom < destination.y && position.y + height > destination.y) {
		float distanceXZ = glm::distance(glm::vec2(position.x, position.z), glm::vec2(destination.x, destination.z));
		if (distanceXZ < radius) {
			*distance = sqrt(distanceXZ * distanceXZ + (destination.y - position.y) * (destination.y - position.y));
			success = true;
		}
	}
	else
		*distance = -1;

	return success;
}

GameObject* CollisionEngine::getOneClosestGameObjectWithinRadius(const Transform* transform, float radius, float bottom, float height, Collision_object object)
{
	glm::vec3 origin = transform->position;
	glm::vec3 destination;
	std::pair<float, GameObject*> closest;

	if (curr_scene) {
		auto& curr_go_volumes = go_volumes[curr_scene];

		closest.first = radius;
		for (auto& pair : curr_go_volumes) {
			if (transform != pair.second.transform && pair.second.hitBox != nullptr){
				if (pair.second.hitBox->collision_object == object) {
					destination = pair.second.transform->position + pair.second.hitBox->position;
					float distance = -1;
					if(overlapsVerticalCylinder(&distance, origin, radius, bottom, height, destination))
						if (closest.first >= distance) {
							closest.first = distance;
							closest.second = pair.first;
						}
				}
				
			}
		}
	}
	return closest.second;
}

std::vector<GameObject*> CollisionEngine::getAllClosestGameObjectWithinRadius(const Transform* transform, float radius, float bottom, float height, Collision_object object)
{
	glm::vec3 origin = transform->position;
	glm::vec3 destination;
	std::vector<GameObject*> closests;

	if (curr_scene) {
		auto& curr_go_volumes = go_volumes[curr_scene];
		for (auto& pair : curr_go_volumes) {
			if (transform != pair.second.transform && pair.second.hitBox != nullptr) {
				if (pair.second.hitBox->collision_object == object) {
					destination = pair.second.transform->position + pair.second.hitBox->position;
					float distance = -1;
					if (overlapsVerticalCylinder(&distance, origin, radius, bottom, height, destination))
						closests.push_back(pair.first);
				}
			}
		}
	}

	return closests;
}

std::vector<GameObject*> CollisionEngine::getAllOverlapingGameObjectWithGroups(GameObject* go, const std::initializer_list<std::string> groups)
{
	std::vector<GameObject*> overlapsed;
	std::vector<GameObject*> gameObjects = curr_scene->getAllGameObjectFromGroups(groups);

	for (GameObject* other_go : gameObjects) {
		if (overlap(go, other_go))
			overlapsed.push_back(other_go);
	}

	return overlapsed;
}

std::vector<GameObject*> CollisionEngine::getAllOverlapingGameObjectWithGroups(const std::string& name, const std::initializer_list<std::string> groups)
{
	std::vector<GameObject*> overlapsed;
	std::vector<GameObject*> gameObjects = curr_scene->getAllGameObjectFromGroups(groups);

	for (GameObject* other_go : gameObjects) {
		if (overlap(name, other_go))
			overlapsed.push_back(other_go);
	}

	return overlapsed;
}


bool overlap_volumes(const Volume& volume, const Volume& other_volume) {

	bool isOverlapping = false;
	HitBox* p_hitbox = volume.hitBox;
	HitBox* p_other_hitbox = other_volume.hitBox;

	HitBox hitbox;
	HitBox other;

	if (p_hitbox)
		hitbox = *p_hitbox;

	if (p_other_hitbox)
		other = *p_other_hitbox;

	hitbox.position += volume.transform->position;
	other.position  += other_volume.transform->position;
	glm::vec2 scale = volume.transform->scale;
	glm::vec2 other_scale = other_volume.transform->scale;
	// make everything go to absolute value instead of relative
	for (AABB& box : hitbox.AABBs) {
		box.position += hitbox.position;
		box.min.x *= scale.x;
		box.min.y *= scale.y;
		box.max.x *= scale.x;
		box.max.y *= scale.y;
	}
	for (AABB& box : other.AABBs) {
		box.position += other.position;
		box.min.x *= other_scale.x;
		box.min.y *= other_scale.y;
		box.max.x *= other_scale.x;
		box.max.y *= other_scale.y;
	}

	float s = glm::length(scale);
	for (Sphere& sphere : hitbox.spheres) {
		sphere.position += hitbox.position;
		sphere.radius *= s;
	}
	
	s = glm::length(other_scale);
	for (Sphere& sphere : other.spheres) {
		sphere.position += other.position;
		sphere.radius *= s;
	}

	//check collision

	if (hitbox.AABBs.size() == 0 && hitbox.spheres.size() == 0 &&
		other.AABBs.size() == 0 && other.spheres.size() == 0) {
		if (hitbox.position == other.position)
			isOverlapping = true;
	}
	else if (hitbox.AABBs.size() == 0 && hitbox.spheres.size() == 0) {
		for (const AABB& obox : other.AABBs)
			if (interesct(hitbox.position, obox))
				isOverlapping = true;

		for (const Sphere& osphere : other.spheres) {
			if (interesct(hitbox.position, osphere))
				isOverlapping = true;
		}
	}
	else if (other.AABBs.size() == 0 && other.spheres.size() == 0) {
		for (const AABB& hbox : hitbox.AABBs)
			if (interesct(other.position, hbox))
				isOverlapping = true;

		for (const Sphere& hsphere : hitbox.spheres) {
			if (interesct(other.position, hsphere))
				isOverlapping = true;
		}
	}
	else {

		for (const AABB& hbox : hitbox.AABBs) {
			for (const AABB& obox : other.AABBs)
				if (interesct(hbox, obox))
					isOverlapping = true;

			for (const Sphere& osphere : other.spheres) {
				if (interesct(osphere, hbox))
					isOverlapping = true;
			}
		}

		for (const Sphere& hsphere : hitbox.spheres) {
			for (const AABB& obox : other.AABBs)
				if (interesct(hsphere, obox))
					isOverlapping = true;

			for (const Sphere& osphere : other.spheres) {
				if (interesct(hsphere, osphere))
					isOverlapping = true;
			}
		}
	}

	return isOverlapping;
}

bool CollisionEngine::overlap(GameObject* a, GameObject* b)
{
	bool isOverlapping = false;

	if (curr_scene) {
		if (go_volumes.contains(curr_scene)) {
			std::unordered_map<GameObject*, Volume>& gos = go_volumes[curr_scene];
			if (gos.contains(a) && gos.contains(b))
				if (overlap_volumes(gos[a], gos[b]))
					isOverlapping = true;
		}
	}

	return isOverlapping;
}

bool CollisionEngine::overlap(GameObject* a, const std::string& b)
{
	bool isOverlapping = false;

	if (curr_scene) {
		if (go_volumes.contains(curr_scene) && name_volumes.contains(curr_scene)) {

			std::unordered_map<std::string, Volume>& names = name_volumes[curr_scene];
			std::unordered_map<GameObject*, Volume>& gos = go_volumes[curr_scene];

			if (gos.contains(a) && names.contains(b))
				if (overlap_volumes(gos[a], names[b]))
					isOverlapping = true;
		}
	}

	return isOverlapping;
}

bool CollisionEngine::overlap(const std::string& a, GameObject* b)
{
	bool isOverlapping = false;

	if (curr_scene) {
		if (go_volumes.contains(curr_scene) && name_volumes.contains(curr_scene)) {

			std::unordered_map<GameObject*, Volume>& gos = go_volumes[curr_scene];
			std::unordered_map<std::string, Volume>& names = name_volumes[curr_scene];

			if (gos.contains(b) && names.contains(a))
				if (overlap_volumes(gos[b], names[a]))
					isOverlapping = true;
		}
	}

	return isOverlapping;
}

bool CollisionEngine::overlap(const std::string& a, const std::string& b)
{
	bool isOverlapping = false;

	if (curr_scene) {
		if (name_volumes.contains(curr_scene)) {
			std::unordered_map<std::string, Volume>& names = name_volumes[curr_scene];
			if (names.contains(a) && names.contains(b))
				if (overlap_volumes(names[a], names[b]))
					isOverlapping = true;
		}
	}

	return isOverlapping;
}

std::unordered_map<Scene*, std::unordered_map<GameObject*, Volume>> CollisionEngine::go_volumes;
std::unordered_map<Scene*, std::unordered_map<std::string, Volume>> CollisionEngine::name_volumes;

Scene* CollisionEngine::curr_scene;