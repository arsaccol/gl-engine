#pragma once

#include <unordered_set>
#include <entt/entt.hpp>

struct SceneNode
{
	entt::entity parent {entt::null};
	// use set instead
	std::unordered_set<entt::entity> children;


	// TODO: make the registry a singleton so we don't have to pass it around like this
	static void addChild(entt::registry& registry, entt::entity parent, entt::entity child)
	{
		SceneNode& parentNode = registry.get<SceneNode>(parent);
		SceneNode& childNode = registry.get<SceneNode>(parent);

		parentNode.children.insert(child);
		childNode.parent = parent;
	}

};

