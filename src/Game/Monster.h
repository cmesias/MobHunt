/*
 * Monster.h
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#ifndef GAME_MONSTER_H_
#define GAME_MONSTER_H_

class Monster {
public:

	float x2, y2;
	int radius;
	float x,y,w,h;
	float angle;
	float angleFacingTarget;
	float vX,vY;
	float speed;
	int type;
	float health;
	float healthDecay;	// used fro VFX of health slowly decaying
	float maxHealth;
	int damage;
	int timer;			// Used for shooting
	float distance;
	bool alive;
	bool collision;		// whether or not the monster is touching ANYTHING at THIS one time
	bool hasVision;			// has vision of target
	bool onScreen;

private:

	float sightRange;	// Range the Mob will see the player and follow them
	float atkRange;		// Range the Mob will attack

public:	// States

	bool alert;			// If true, slime sees the player (within 100 pixels)

public: // actions

	std::string jumpstate;
	int jumps;
	bool jump;

public: // animations
	double animSpeed;
	double animTimer;
	int animFrame;

public:	// Setter functions

	void setSightRange(float value) {
		sightRange = value;
	}

	void setAtkRange(float value) {
		atkRange = value;
	}

public:	// Getter functions

	float getSightRange() {
		return sightRange;
	}

	float getAtkRange() {
		return atkRange;
	}
};

#endif /* GAME_MONSTER_H_ */
