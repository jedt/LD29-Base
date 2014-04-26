#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "GameState.hpp"
#include "level.h"
#include <SFML/Graphics.hpp>

struct GameSprite {
	sf::Sprite *sprite;
	sf::Vector2f angularVelocity;
	float speed = 0.4;
	int deleted = 0;
	int moving = 1;
	float created;
};

class GameEngine;

class PlayState : public GameState
{
public:
	PlayState( GameEngine& game, bool replace = true );

	void pause();
	void resume();

	void update();
	void draw();

private:
	bool showDebug;
	sf::Texture m_bgTex;
	sf::Sprite m_bg;
	
	sf::Texture m_playerTex;
	sf::Sprite m_player;
	
	sf::Texture m_bulletTex;
	sf::Texture m_enemyTex;
	
	sf::Clock m_clock;
	
	Level m_tiledMap;
	
	sf::Vector2f m_mousePointerPos;
	sf::Vector2f m_velocityToMouse;
	sf::Vector2f m_velocityToLeftHand;
	sf::Vector2f m_velocityToRightHand;
	float m_playerAngle;
	
	std::vector<GameSprite> m_playerBullets;
	std::vector<GameSprite> m_enemies;
	
	sf::View m_view;
	sf::View m_debugView;
	
	sf::Font m_font;
	sf::Text m_label1;
	
	sf::VertexArray m_lineToMouse;
	sf::VertexArray m_lineToLeftHand;
	sf::VertexArray m_lineToRightHand;
	sf::CircleShape m_circleOfPlayer;
};

#endif // PLAYSTATE_HPP
