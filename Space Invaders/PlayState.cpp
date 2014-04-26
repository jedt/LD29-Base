#include <memory>
#include <iostream>

#include "GameEngine.hpp"
#include "PlayState.hpp"
#include "MenuState.hpp"
#include "Resource.h"
#include "Common.h"

PlayState::PlayState( GameEngine& game, bool replace ) : GameState( game, replace ) {
	sf::Vector2u wsize = game.screen.getSize();
	m_bgTex.loadFromFile( Resource::resourcePath() + "bg.png" );
	m_bg.setTexture( m_bgTex);
	
	m_font.loadFromFile(Resource::resourcePath() + "sansation.ttf");
	
	m_tiledMap.LoadFromFile(Resource::resourcePath() + "tiled.tmx");
	m_tiledMap.SetDrawingBounds(sf::FloatRect(0, 0, 1536, 864));
	
	m_playerTex.loadFromFile(Resource::resourcePath() + "player.png");
	m_player.setTexture(m_playerTex);
	m_player.setPosition(wsize.x/2 - m_player.getLocalBounds().width, wsize.y - m_player.getLocalBounds().height - 32);
	
	m_bulletTex.loadFromFile(Resource::resourcePath() + "bullet.png");
	
	sf::Vector2f halfSize = sf::Vector2f(m_player.getLocalBounds().width/2, m_player.getLocalBounds().height/2);
	m_player.setOrigin(halfSize);
	//adjust
	m_player.setPosition(m_player.getPosition() + halfSize);
	
	m_enemyTex.loadFromFile(Resource::resourcePath() + "enemy_1.png");
	
	float w = wsize.x;
	float h = wsize.y;
	
	sf::View m_view(sf::Vector2f(w/2, h/2), sf::Vector2f(w, h));
	m_view.setCenter(sf::Vector2f(floorf(m_player.getPosition().x), floorf(m_player.getPosition().y)));
	m_game.screen.setView(m_view);
	
	m_label1 = sf::Text("Hello, world.", m_font, 18);
	m_label1.setColor(sf::Color::White);
	m_label1.setPosition(0, 0);
	
	m_debugView.reset(sf::FloatRect(0,0, wsize.x, wsize.y));
	m_debugView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	
	
	std::cout << "PlayState Init" << std::endl;
}

void PlayState::pause()
{
	std::cout << "PlayState Pause" << std::endl;
}

void PlayState::resume()
{
	std::cout << "PlayState Resume" << std::endl;
}

void PlayState::update()
{
	float t = -(m_clock.getElapsedTime().asMilliseconds());
	
	m_clock.restart();
	
	sf::Vector2u wsize = m_game.screen.getSize();
	sf::Vector2f centerPos = sf::Vector2f(wsize.x/2, wsize.y/2);

	sf::Vector2f newPos;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		newPos = m_player.getPosition() + (m_velocityToMouse * (t * 0.18f));
		m_player.setPosition(sf::Vector2f(floorf(newPos.x), floorf(newPos.y)));
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		newPos = m_player.getPosition() - (m_velocityToMouse * (t * 0.10f));
		m_player.setPosition(sf::Vector2f(floorf(newPos.x), floorf(newPos.y)));
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		newPos = m_player.getPosition() + (m_velocityToLeftHand * (t * 0.14f));
		m_player.setPosition(sf::Vector2f(floorf(newPos.x), floorf(newPos.y)));
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		newPos = m_player.getPosition() + (-m_velocityToLeftHand * (t * 0.14f));
		m_player.setPosition(sf::Vector2f(floorf(newPos.x), floorf(newPos.y)));
	}
		

	
	sf::View view = m_game.screen.getView();
	sf::Vector2f position = m_player.getPosition();
	view.setCenter(position);
	m_game.screen.setView(view);

	sf::Event event;
	while( m_game.screen.pollEvent( event ) )
	{
		switch( event.type )
		{
			case sf::Event::Closed:
				m_game.quit();
				break;

			case sf::Event::KeyPressed:
				switch( event.key.code )
				{
					case sf::Keyboard::Escape:
						m_game.quit();
						break;
					case sf::Keyboard::Q:
						if (event.key.system) {
							m_game.quit();
						}
					case sf::Keyboard::M:
						m_next = m_game.build<MenuState>( false );
						break;
					case sf::Keyboard::C:
						if (event.key.shift) {
							for (int i=1; i <= 12; i++) {
								sf::Sprite *newSprite = new sf::Sprite(m_enemyTex);
								GameSprite newGameSprite;
								newGameSprite.sprite = newSprite;
								
								newGameSprite.deleted = 0;
								newGameSprite.created = m_clock.getElapsedTime().asMilliseconds();
								newGameSprite.speed = 0.04;
								newGameSprite.angularVelocity = sf::Vector2f(0, -1);
								sf::FloatRect enemyBox = newGameSprite.sprite->getLocalBounds();
								float x = (i * enemyBox.width) + (i * 16) + 32;
								newGameSprite.sprite->setPosition(x, 32);
															
								m_enemies.push_back(newGameSprite);
							}
						}
						break;
				}
				break;
				
		}
		
		if (event.type == sf::Event::MouseMoved) {
			m_mousePointerPos = sf::Vector2f(floorf(event.mouseMove.x), floorf(event.mouseMove.y));
			sf::Vector2f playerPos = centerPos;
			
			if (m_mousePointerPos.y < playerPos.y) {
				m_playerAngle = angleForDestination(playerPos, m_mousePointerPos);
				m_playerAngle = 0 - m_playerAngle;

				m_velocityToMouse = unitVelocity(playerPos, m_mousePointerPos);
				m_player.setRotation(90); // absolute angle
				m_player.rotate(m_playerAngle);
			}
			else {
				m_playerAngle = angleForDestination(playerPos, m_mousePointerPos);
				m_velocityToMouse = unitVelocity(playerPos, m_mousePointerPos);
				m_player.setRotation(90); // absolute angle
				m_player.rotate(m_playerAngle);
			}
			
			m_lineToMouse = sf::VertexArray(sf::Lines, 2);
			m_lineToMouse[0].position = m_mousePointerPos;
			m_lineToMouse[1].position = centerPos;
			
			m_lineToMouse[0].color = sf::Color(180, 180, 180, 80);
			m_lineToMouse[1].color = sf::Color(180, 180, 180, 80);
			
			VECTOR2 mousePos = VECTOR2(m_mousePointerPos.x, m_mousePointerPos.y);
			VECTOR2 center = VECTOR2(centerPos.x, centerPos.y);

			sf::Vector2f a = center.sf();
			
			float radius = 80.f;
			
			sf::Vector2f velocity = unitVelocity(mousePos.sf(), center.sf());			
			sf::Vector2f b = a + (velocity * radius);
			

			std::cout << "velocity: " << sprintV(velocity) << std::endl;
			
			sf::Vector2f c = sf::Vector2f(a.x + (b.y - a.y), (a.y + (a.x - b.x)));
			
			m_lineToLeftHand = sf::VertexArray(sf::LinesStrip, 4);
			m_lineToLeftHand[0].position = a;
			m_lineToLeftHand[1].position = b;
			m_lineToLeftHand[2].position = c;
			m_lineToLeftHand[3].position = a;
			
			m_lineToLeftHand[0].color = sf::Color(0, 255, 0, 80);
			m_lineToLeftHand[1].color = sf::Color(0, 255, 0, 80);
			m_lineToLeftHand[2].color = sf::Color(0, 255, 0, 80);
			m_lineToLeftHand[3].color = sf::Color(0, 255, 0, 80);
			
			m_circleOfPlayer = sf::CircleShape(radius);
			m_circleOfPlayer.setOutlineThickness(1.f);
			m_circleOfPlayer.setFillColor(sf::Color::Transparent);
			m_circleOfPlayer.setOutlineColor(sf::Color(0, 0, 255, 80));
			m_circleOfPlayer.setOutlineThickness(1.f);
			m_circleOfPlayer.setPosition(a);
			m_circleOfPlayer.setOrigin(radius, radius);
			
			m_velocityToLeftHand = unitVelocity(a, c);
			
			//float distance = getDistanceV(center, mousePos);
			
			//let a the pivot point (center) and b the (top point mouse)
			//(a_x + (b_y - a_y), a_y + (a_x - b_x))
			
			//sf::Vector2f leftHandPos = sf::Vector2f(mousePos.x - distance, mousePos.y - distance);
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			sf::Sprite *newSprite = new sf::Sprite(m_bulletTex);
			GameSprite newGameSprite;
			newGameSprite.sprite = newSprite;

			newGameSprite.deleted = 0;
			newGameSprite.created = m_clock.getElapsedTime().asMilliseconds();
			std::cout << "m_player.getPosition(): " << sprintV(m_player.getPosition()) << std::endl;
			
			newGameSprite.sprite->setOrigin(newGameSprite.sprite->getLocalBounds().width/2, newGameSprite.sprite->getLocalBounds().height/2);
			newGameSprite.sprite->setPosition(m_player.getPosition());
			newGameSprite.angularVelocity = m_velocityToMouse;
			newGameSprite.speed = 1.0f;
			newGameSprite.sprite->setRotation(90);
			newGameSprite.sprite->rotate(m_playerAngle);
			
			m_playerBullets.push_back(newGameSprite);
		}
	}
	
	for(std::vector<GameSprite>::iterator it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it) {
		GameSprite &bullet = *it;
		
		sf::Rect<float> bulletBox = bullet.sprite->getGlobalBounds();
		sf::Vector2f newBulletPos = bullet.sprite->getPosition() + (bullet.angularVelocity * (bullet.speed * t));
		
		bool outOfBounds = false;
		
		//if bullet is out of bounds?
		if (newBulletPos.y <= 0) {
			outOfBounds = true;
		}
		else if (newBulletPos.x <= 0) {
			outOfBounds = true;
		}
		else if (newBulletPos.x + bulletBox.width/2 >= wsize.x) {
			outOfBounds = true;
		}
		
		if (outOfBounds == false) {
			bullet.sprite->setPosition(newBulletPos);
		}
		else {
			bullet.moving = 0;
			bullet.deleted = 1;
		}
		
		//if hit an enemy?
		for(std::vector<GameSprite>::iterator itEnemy = m_enemies.begin(); itEnemy != m_enemies.end(); ++itEnemy) {
			GameSprite &enemy = *itEnemy;
			sf::Rect<float> enemyBox = enemy.sprite->getGlobalBounds();
			if (enemyBox.intersects(bulletBox)) {
				enemy.deleted = 1;
				bullet.moving = 0;
				bullet.deleted = 1;
			}
		}
	}
	
	for(std::vector<GameSprite>::iterator it = m_enemies.begin(); it != m_enemies.end(); ++it) {
		GameSprite &enemy = *it;
		enemy.sprite->move(enemy.angularVelocity * (enemy.speed * t));
	}

	for(std::vector<GameSprite>::iterator it = m_playerBullets.begin(); it != m_playerBullets.end();) {
		GameSprite &bullet = *it;
		if (bullet.deleted == 1) {
			delete bullet.sprite;
			it = m_playerBullets.erase(it);
		}
		else {
			++it;
		}
	}
	
	for(std::vector<GameSprite>::iterator it = m_enemies.begin(); it != m_enemies.end();) {
		GameSprite &enemy = *it;
		if (enemy.deleted == 1) {
			delete enemy.sprite;
			it = m_enemies.erase(it);
		}
		else {
			++it;
		}
	}
	
	std::ostringstream buff;
	buff << "event.mouseMove.x,event.mouseMove.y: " << sprintV(m_mousePointerPos) << std::endl;
	buff << "centerPos: " << sprintV(centerPos) << std::endl;
	buff << "m_playerAngle: " << m_playerAngle << std::endl;
	
	buff << "m_velocityToLeftHand: " << sprintV(m_velocityToLeftHand) << std::endl;
	buff << "m_velocityToRightHand: " << sprintV(-m_velocityToLeftHand) << std::endl;
	
	buff << "m_velocityToMouse: " << sprintV(m_velocityToMouse) << std::endl;
	m_label1.setString(buff.str());
}

void PlayState::draw()
{
	// Clear the previous drawing
	m_game.screen.clear();
	m_tiledMap.Draw(m_game.screen);
	m_game.screen.draw(m_player);

	for(std::vector<GameSprite>::iterator it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it) {
		GameSprite &bullet = *it;
		m_game.screen.draw(*bullet.sprite);
	}
	
	for(std::vector<GameSprite>::iterator it = m_enemies.begin(); it != m_enemies.end(); ++it) {
		GameSprite &enemy = *it;
		m_game.screen.draw(*enemy.sprite);
	}
	
	m_game.screen.setView(m_debugView);
		
	if (showDebug) {
		m_game.screen.draw(m_lineToMouse);
		m_game.screen.draw(m_lineToLeftHand);
		m_game.screen.draw(m_lineToLeftHand);
		
		m_game.screen.draw(m_circleOfPlayer);
		
		m_game.screen.draw(m_label1);
	}
	
	m_game.screen.display();
}

