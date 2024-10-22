#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H


#include "component.h"
#include "entity.h"
#include <vector>
class ComponentSystem
{
public:
    virtual void AddEntity(const Entity& entity, const ComponentManager& componentManager) {}
//    virtual void RemoveEntity(int entityId, const ComponentManager& componentManager);  // Uhhh... I don't necissarily *need* to do this (yet) :)
};

class MovementSystem : public ComponentSystem
{
    std::vector<int> positionIndices;
public:
    void AddEntity(const Entity &entity, const ComponentManager& componentManager) override
    {
        positionIndices.push_back(componentManager.components.at(entity.Id));
    }
    void Update(PositionComponent &positions)
    {
        for (int i = 0; i < positionIndices.size(); i++)
        {
            int positionIndex = positionIndices[i];
            positions.x[positionIndex] += positions.dx[positionIndex];
            positions.y[positionIndex] += positions.dy[positionIndex];
            positions.z[positionIndex] += positions.dz[positionIndex];
        }
    }
};

class DamageSystem : public ComponentSystem
{
    std::vector<int> healthIndices;
    std::vector<int> damageIndices;
public:
    void AddEntity(const Entity &entity, const ComponentManager& componentManager) override
    {
        healthIndices.push_back(componentManager.components.at(entity.Id));
        damageIndices.push_back(componentManager.components.at(entity.Id));
    }

    void Damage(const Entity &attacker, const Entity &receiver, DamageComponent& dmgComponent, HealthComponent& hpComponent)
    {
        int attackerIndex = damageIndices.at(attacker.Id);
        int receiverIndex = healthIndices.at(receiver.Id);

        if (dmgComponent.cooldown[attackerIndex] > 0.0f)
            return;

        hpComponent.health[receiverIndex] -= dmgComponent.damage[attackerIndex];
        dmgComponent.cooldown[attackerIndex] = 1.0f;
    }

    void Update(DamageComponent &damagers, float deltaTime)
    {
        for (int i = 0; i < damageIndices.size(); i++)
        {
            int damageIndex = damageIndices[i];
            if (damagers.cooldown[damageIndex] > 0.0f)
            damagers.cooldown[damageIndex] -= deltaTime;
        }
    }
};

class InventorySystem : public ComponentSystem
{
    std::vector<int> itemIndices;
    std::vector<int> invIndices;
public:
    void AddEntity(const Entity &entity, const ComponentManager& componentManager) override
    {
        itemIndices.push_back(componentManager.components.at(entity.Id));
    }
    void PickUp(const Entity &item, const Entity &inventory, InventoryComponent &invComp, ItemComponent &itemComp)
    {
        int itemIndex = itemIndices.at(item.Id);
        int invIndex = invIndices.at(inventory.Id);

        invComp.inventory[invIndex].item.push_back(itemComp.item[itemIndex]);
    }
};

class RenderSystem : public ComponentSystem
{

};

#endif // COMPONENTSYSTEM_H
