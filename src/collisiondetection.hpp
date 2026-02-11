#pragma once

#include <box2d/box2d.h>
#include <iostream>
#include "object.hpp"
#include "userdata.hpp"



/**
 * @brief The Box2D (physics) world calls this class when two things collide
 * Manages taking damage
 * 
 */

class CollisionListener : public b2ContactListener {
    public:
        /**
         * @brief default constructor so that a default CollisionListener can be initialized in LevelState
         * 
         */
        CollisionListener() {};
        /**
         * @brief constructor with a clock to track time from the creation of the world
         * 
         * @param clockForWorld clock to track time
         */
        CollisionListener(sf::Clock& clockForWorld) : clockForWorld_(clockForWorld) {} 


        /**
         * @brief Called when two fixtures start to collide
         * Manage taking damage
         * Marking which bodies to remove from the physics world
         * Increase the player's score
         * 
         */
        void BeginContact(b2Contact* contact) override {
            if (contactCount_ == 0)
            {
                // std::cout << "nyt" << std::endl;
                clockForWorld_.restart(); // kello alkaa kulkea ensimmäisestä törmäyksestä
            }
            
            contactCount_++;
            float elapsedTime = clockForWorld_.getElapsedTime().asSeconds();
            // Retrieve the two fixtures involved in the collision
            b2Fixture* fixtureA = contact->GetFixtureA();
            b2Fixture* fixtureB = contact->GetFixtureB();

            // Get the bodies from the fixtures
            b2Body* bodyA = fixtureA->GetBody();
            b2Body* bodyB = fixtureB->GetBody();

            // Get the user data from the bodies to identify them
            Userdata* dataA = reinterpret_cast<Userdata*>(bodyA->GetUserData().pointer);
            Userdata* dataB = reinterpret_cast<Userdata*>(bodyB->GetUserData().pointer);
            

            if (dataA->object && dataB->object && elapsedTime > 0.5f) // check that the game started at least 0.5 second ago
            {
                // std::cout << "time: " << elapsedTime << std::endl;
                dataA->object->incrementSpeak();
                dataB->object->incrementSpeak();
                // std::cout << "Collision between " << dataA->objecttype << " and " << dataB->objecttype << std::endl;
                // Ground does not speak dataA->objecttype != "ground" && dataB->objecttype != "ground"
            

                if (dataA->objecttype != "ground")
                {
                    // std::cout << dataA->objecttype << " speaks" << std::endl;
                    dataA->object->speak();
                }
                if (dataB->objecttype != "ground")
                {
                    // std::cout << dataB->objecttype << " speaks" << std::endl;
                    dataB->object->speak();
                }
                // Birds do not rotate before impact. Makes them rotate after impact
                if (dataA->objecttype == "bird")
                {
                    if (dataA->object->isFlying())
                    {
                        dataA->object->land();
                        dataA->object->resetFixedRotation();
                    }
                }
                if (dataB->objecttype == "bird")
                {
                    if (dataB->object->isFlying())
                    {
                        dataB->object->land();
                        dataB->object->resetFixedRotation();
                    }
                }
                
                // Damage based on the kinectic energy of both objects
                double keA = 0.5 * dataA->object->getBody()->GetMass() * dataA->object->getSpeed() * dataA->object->getSpeed();
                double keB = 0.5 * dataB->object->getBody()->GetMass() * dataB->object->getSpeed() * dataB->object->getSpeed();
                double dmgDealt = 20 * (keA + keB);
                if (dataA->objecttype != "ground" && (bodyA->GetType() == b2_dynamicBody || dataA->objecttype == "star")) // ground doesn't take damage
                {
                    dataA->object->takeDamage(dmgDealt);
                    // std::cout << dataA->objecttype << " hp: " << dataA->object->getHp() << std::endl;                 
                }
                if (dataB->objecttype != "ground" && (bodyB->GetType() == b2_dynamicBody || dataB->objecttype == "star"))
                {
                    dataB->object->takeDamage(dmgDealt);
                    // std::cout << dataB->objecttype << " hp: " << dataB->object->getHp() << std::endl;  
                }

                // increase score when hitting obstacles 
                if ((dataA->objecttype == "obstacle" && dataB->objecttype == "bird") || (dataB->objecttype == "obstacle" && dataA->objecttype == "bird")) {
                    score_ += dmgDealt * 10;
                    // std::cout << "score is now " << score_ << std::endl;
                }

                // If hp of an object drops to zero they will be added to a list of bodies to remove from the b2World
                // Removing is done in the game loop
                if (dataA->object->getHp() <= 0)
                {
                    if (std::find(removedBodies_.begin(), removedBodies_.end(), dataA->object->getBody()) == removedBodies_.end()) //check if the b2Body has already been removed
                    {
                        bodiesToRemove_.push_back(dataA->object->getBody()); // add the b2Body to a vector of bodies that should be deleted
                        removedBodies_.push_back(dataA->object->getBody()); // add the b2Body to a vector of deleted bodies
                    }
                }
                if (dataB->object->getHp() <= 0)
                {
                    if (std::find(removedBodies_.begin(), removedBodies_.end(), dataB->object->getBody()) == removedBodies_.end()) //check if the b2Body has already been removed
                    {
                        bodiesToRemove_.push_back(dataB->object->getBody()); // add the b2Body to a vector of bodies that should be deleted
                        removedBodies_.push_back(dataB->object->getBody()); // add the b2Body to a vector of deleted bodies
                    }
                }
            }
        }

        /**
         * @brief Get the bodies to remove from the physics world
         * 
         */

        const std::vector<b2Body*>& getBodiesToRemove() const {
            return bodiesToRemove_;
        }

        // Clear the list of bodies to remove after they are destroyed
        void clearBodiesToRemove() {
            bodiesToRemove_.clear();
        }

        int getScore() {
            return score_;
        }

    private:
        std::vector<b2Body*> removedBodies_;
        std::vector<b2Body*> bodiesToRemove_;
        sf::Clock clockForWorld_;
        int contactCount_ = 0;
        int score_ = 0;
};