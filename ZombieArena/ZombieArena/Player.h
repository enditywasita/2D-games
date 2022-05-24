#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	// Where is the player
	Vector2f m_Position;

	// Sprite
	Sprite m_Sprite;

	// Texture
	Texture m_Texture;

	// Screen resolution
	Vector2f m_Resolution;
	
	// Size the current arena
	IntRect m_Arena;

	// How big is each tile of the arena
	int m_TileSize;

	// Which direction is the player currently
	bool m_UpPressed, m_DownPressed, m_LeftPressed, m_RightPressed;

	// How much health have the player got
	int m_Health;

	// Maximum health the player can have
	int m_MaxHealth;

	// When was the player last hit
	Time m_LastHit;

	// Speed in pixel per s
	float m_Speed;

public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	// Call this at the end of every game
	void resetPlayerStats();

	// Handle the player getting hit by a zombie
	bool hit(Time timeHit);

	// How long ago was the player last hit
	Time getLastHitTime();

	// Where is the player
	FloatRect getPosition();

	// Where is the center of the player
	Vector2f getCenter();
	
	// Which angle is the player facing
	float getRotation();
	// send a copy of the sprite to main
	Sprite getSprite();

	// The next four function move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	// Stop the player in specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	// We will call this function once every frame
	void update(float elapsedTime, Vector2i mousePosition);

	// Give player a speed boost
	void upgradeSpeed();

	// Give the player some health
	void upgradeHealth();

	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);

	// How much health has the player currently got
	int getHealth();
};