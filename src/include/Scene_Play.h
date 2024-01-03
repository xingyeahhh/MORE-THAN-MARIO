#pragma once
#include "Common.h"
#include "Physics.h"
#include "Scene.h"


class Scene_Play : public Scene {
  struct PlayerConfig {
    float X, Y, CX, CY, SPEED, MAXHEIGHT, JUMP, GRAVITY;
  };

  struct EnemyConfig {
    float X, Y, SPEED = 2, GRAVITY;
  };

  int m_score = 0;
  
  sf::Text m_scoreText;
  sf::Text m_pauseText;
  sf::Text gameOverText;
  sf::Text shareText;
  sf::Text pressText;
  sf::Sound m_backgroundMusic;
  sf::Sound m_jumpMusic;
  sf::Sound m_coinMusic;
  sf::Sound m_shootMusic;
  sf::Sound m_gameoverMusic;
  sf::Sound m_dieMusic;
  sf::Sound m_pauseMusic;
  sf::Sound m_eatMusic;
  sf::Sound m_hurtMusic;
  protected:
  std::shared_ptr<Entity> m_player;
  std::string m_levelPath;
  PlayerConfig m_playerConfig;
  EnemyConfig m_enemyConfig;
  bool m_drawTextures = true;
  bool m_drawCollision = false;
  bool m_drawGrid = false;
  const Vec2 m_gridSize = {64, 64};
  sf::Text m_gridText;
  Physics m_physics;

  void init(const std::string &levelPath);
  void loadLevel(const std::string &filePath);
  void loadMap(const std::string &filePath);
  void spawnPlayer();
  void update() override;
  void onEnd() override;

  void spawnBullet(const std::shared_ptr<Entity> &entity);
  void spawnEnemy();

  Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

  void sMovement();
  void sLifespan();
  void spawnCoin(const Vec2& position);
  void spawnStar(const Vec2& position);
  void spawnMushroom(const Vec2& position);
  void spawnFlower(const Vec2& position);
  void sCollision();
  void sDoAction(const Action &action) override;
  void sAnimation();
  void sRender() override;

  public:
  Scene_Play(GameEngine *gameEngine, const std::string &levelPath);
};
