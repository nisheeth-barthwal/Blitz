/*
 * Enemy.cpp
 *
 *  Created on: Sep 22, 2012
 *      Author: Nisheeth
 */
 
#include "Enemy.hpp"
#include <iostream>
namespace blitz {
	namespace unit {		
		Enemy::Enemy(const geometry::Triad &start)
		{			
			this->_state = new state::State();
			this->_state->current = this->_state->start = start;
			this->_state->box.set(-1.0f, 1.0f, 1.0f, -1.0f);
			this->_state->normal.set(0.0f, 0.0f, 1.0f);
			this->_state->velocity.set(0.0f, 0.0f, 10.f);
			this->_state->clear = *this->_state->color.set(0.0f, 1.0f, 0.0f, 1.0f);

			this->_health = 1.0f;
			this->_hitPlayer = false;
		}

		Enemy::~Enemy(void)
		{		
		}

		void Enemy::draw(void)
		{	
			geometry::Triad t = this->_state->current;
			geometry::Triad r = this->_state->rotation.direction;
			glPushMatrix();
				glEnable( GL_TEXTURE_2D );
				glEnable(GL_BLEND);
				glEnable(GL_ALPHA_TEST);
				glEnable(GL_DEPTH_TEST);

				glTranslatef(t.x, t.y, t.z);
				glRotatef(this->_state->angle, r.x, r.y, r.z);
				glColor4f(this->_state->color.a, this->_state->color.b, this->_state->color.c, this->_state->color.d);
				glBegin(GL_QUADS);          // Begin Drawing The Textured Quad
						glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
						glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
						glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
				glEnd();	

				glDisable( GL_TEXTURE_2D );
				glDisable(GL_BLEND);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_DEPTH_TEST);

			glPopMatrix();
		}

		void Enemy::tick(float delta)
		{
			this->_completed = this->updateState(delta);
			this->_hitPlayer = this->_state->current.z >= -3.0f;
		}

		bool Enemy::isComplete() const
		{
			return this->_completed;
		}

		void Enemy::hit(Object& obj)
		{			
			unit::DamageObject* d = dynamic_cast<unit::DamageObject*>(&obj);
			if(d)
			{
				this->reduceHealth(d->getDamage());
				if(this->_health <= 0.0f)
				{
					this->_state->velocity.set(0.0f, 0.0f, 0.0f);
				
					state::Animation* a = new state::ColorAnimation(this->_state, state::AnimationType::atColorBlink, geometry::Quad(1.0f, 1.0f, 0.0f, 1.0f), 1.0f);
					a->setTerminal(true);
					this->_animation.push_back(a);		
				}
				else
					this->_animation.push_back(new state::ColorAnimation(this->_state, state::AnimationType::atColorBlink, geometry::Quad(1.0f, 0.0f, 0.0f, 1.f), 0.2f));		
			}
		}


		// Private Methods

		void Enemy::reduceHealth(float damage)
		{			
			this->_health -= damage;
			std::cout << "Damage: " << damage << ", Health: " << this->_health << std::endl;
		}

		bool Enemy::hasHitPlayer()
		{
			return this->_hitPlayer;
		}
	}
}