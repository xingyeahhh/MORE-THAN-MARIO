#include "../include/Scene_Play.h"
#include "../include/Action.h"
#include "../include/Assets.h"
#include "../include/Components.h"
#include "../include/GameEngine.h"
#include <fstream>
#include <cassert>
#include <iostream>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath): Scene(gameEngine), m_levelPath(levelPath) {
  init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
  registerAction(sf::Keyboard::Z, eActionNames::PAUSE);
  registerAction(sf::Keyboard::Escape, eActionNames::QUIT);
  registerAction(sf::Keyboard::T, eActionNames::TOGGLE_TEXTURE);
  registerAction(sf::Keyboard::C, eActionNames::TOGGLE_COLLISION);
  registerAction(sf::Keyboard::P, eActionNames::TOGGLE_GRID);
  registerAction(sf::Keyboard::Space, eActionNames::UP);
  registerAction(sf::Keyboard::K, eActionNames::RIGHT);
  registerAction(sf::Keyboard::J, eActionNames::LEFT);
  registerAction(sf::Keyboard::D, eActionNames::FIRE);
  //  TODO: Register all other gameplay Actions

  m_scoreText.setFont(m_game->getAssets().getFont(eFontTypes::PIXEL));
  m_scoreText.setCharacterSize(22); // Choose an appropriate size
  m_scoreText.setPosition(20, 20); // Top-left corner of the screen
  m_scoreText.setString("Coin Score: 0");
  m_scoreText.setColor(sf::Color::Black); // 设置颜色 

 

  // In initialization or constructor
  m_pauseText.setFont(m_game->getAssets().getFont(eFontTypes::PIXEL));
  m_pauseText.setCharacterSize(80); // or any size that fits well
  m_pauseText.setString("PAUSED!");
  //m_pauseText.setColor(sf::Color::Orange); // Choose a color that stands out
  m_pauseText.setFillColor(sf::Color(117, 204, 239));

  // Set the position to center
  sf::FloatRect textRect = m_pauseText.getLocalBounds();
  m_pauseText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
  m_pauseText.setPosition(sf::Vector2f(m_game->window().getSize().x / 2.0f, m_game->window().getSize().y / 2.0f));

  




  const sf::SoundBuffer& buffer = m_game->getAssets().getSound(eSoundTypes::musicSOUND);
  m_backgroundMusic.setBuffer(buffer);
  m_backgroundMusic.setLoop(true);

  const sf::SoundBuffer& buffer2 = m_game->getAssets().getSound(eSoundTypes::jumpSOUND);
  m_jumpMusic.setBuffer(buffer2);
  m_jumpMusic.setLoop(false);

  const sf::SoundBuffer& buffer3 = m_game->getAssets().getSound(eSoundTypes::COINSOUND);
  m_coinMusic.setBuffer(buffer3);
  m_coinMusic.setLoop(false);

  const sf::SoundBuffer& buffer4 = m_game->getAssets().getSound(eSoundTypes::SHOOTSOUND);
  m_shootMusic.setBuffer(buffer4);
  m_shootMusic.setLoop(false);

  const sf::SoundBuffer& buffer5 = m_game->getAssets().getSound(eSoundTypes::GAMEOVERSOUND);
  m_gameoverMusic.setBuffer(buffer5);
  m_gameoverMusic.setLoop(false);

  const sf::SoundBuffer& buffer6 = m_game->getAssets().getSound(eSoundTypes::DIESOUND);
  m_dieMusic.setBuffer(buffer6);
  m_dieMusic.setLoop(false);

  const sf::SoundBuffer& buffer7 = m_game->getAssets().getSound(eSoundTypes::PAUSESOUND);
  m_pauseMusic.setBuffer(buffer7);
  m_pauseMusic.setLoop(false);

  const sf::SoundBuffer& buffer8 = m_game->getAssets().getSound(eSoundTypes::EATINGSOUND);
  m_eatMusic.setBuffer(buffer8);
  m_eatMusic.setLoop(false);

  const sf::SoundBuffer& buffer9 = m_game->getAssets().getSound(eSoundTypes::HURTSOUND);
  m_hurtMusic.setBuffer(buffer9);
  m_hurtMusic.setLoop(false);

  m_gridText.setCharacterSize(12);
  m_gridText.setFont(m_game->getAssets().getFont(eFontTypes::NUMBERS));

  loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY,
                                std::shared_ptr<Entity> entity) {
  //      TODO: This function takes in a grid (x,y) position and an Entity
  //            Return a Vec2 indicating where the CENTER position of the Entity
  //            should be You must use the Entity's Animation size to position
  //            it correctly The size of the grid width and height is stored in
  //            m_gridSize.x and m_gridSize.y The up-left corner of the
  //            Animation should align with the bottom left of the grid cell
  float x, y;
  x = gridX + entity->getComponent<CAnimation>().animation.getSize().x / 2;
  y = gridY - entity->getComponent<CAnimation>().animation.getSize().y / 2;
  return Vec2(x, y);
}

// I'm in a sense rewriting the loadLevel method, in there i'll only read the
// specification of the player and the goomba and perhaps others special entites
// special means with special attributes
void Scene_Play::loadMap(const std::string &filePath) {
  std::ifstream file(filePath);
  int value;
  eLevelEntities level_entities;
  eAnimationTypes animation_types;
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 36; j++) {
      file >> value;
      if (value == -1) continue;
      assert(file.good());// checking for any error in the file reading
      level_entities = static_cast<eLevelEntities>(value);
      switch (level_entities) {
        case PLAYER:
          //m_playerConfig.X = j;
          //m_playerConfig.Y = i;
          //use level.txt to reset the position of player everytime
          break;
        case GOOMBA:
          m_enemyConfig.X = j;
          m_enemyConfig.Y = i;
          spawnEnemy();
          break;
        default:
          // handling the undefined entity
          if (value >= eLevelEntities::LAST) {
            std::cerr << "unknown entity" << std::endl;
            continue;
          }
          auto entity = m_entityManager.addEntity((level_entities % 2 == 0) ? DEC : TILE, level_entities);
          animation_types = static_cast<eAnimationTypes>(level_entities);
          entity->addComponents<CAnimation>(m_game->getAssets().getAnimation(animation_types), true);
          // using j, i instead of x, y, simulating a matrix
          entity->addComponents<CTransform>(gridToMidPixel((float) j * 64, (float) i * 64, entity));
          entity->getComponent<CTransform>().prevPos =
              entity->getComponent<CTransform>().pos;
          if (entity->id() == TILE)
            entity->addComponents<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize());
          break;
      }
    }
  }
}

void Scene_Play::loadLevel(const std::string &filePath) {
  //  reset the entity manager every time we load a level
  m_entityManager = EntityManager();
  // TODO : read in the level file and add the appropriate entities
  //        use the PlayerConfig struct m_playerConfig to store player
  //        proprieties this struct is defined at the top of Scene_Play.h

  // NOTE : all the code below is a sample code which shows you how to
  //        set up and use entities with the new syntax, it should be removed
  std::ifstream file(filePath);
  std::string str;
  int Intstr;
  eLevelEntities level_entities;
  eAnimationTypes animation_types;
  loadMap("../../../bin/MapConfig/Map.txt");
  while (file.good() && str != " ") {
    file >> str;
    Intstr = std::stoi(str);
    //change the str into int into the function
    level_entities = static_cast<eLevelEntities>(Intstr);
    if (level_entities < eLevelEntities::LAST && level_entities >= 0) {
      switch (level_entities) {
        case PLAYER:
          file >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >>
              m_playerConfig.CY >> m_playerConfig.JUMP >> m_playerConfig.SPEED >>
              m_playerConfig.MAXHEIGHT >>
              m_playerConfig.GRAVITY;// remain to implement an attribute which is
                                     // the bullet animation
          break;                     // that has to be read from file
        case GOOMBA:
          file >> m_enemyConfig.X >> m_enemyConfig.Y >> m_enemyConfig.SPEED 
               >> m_enemyConfig.GRAVITY
              ;
          spawnEnemy(); // Spawn a Goomba at the specified position
          break;
        default:
          int x, y;
          file >> x >> y;
           auto entity = m_entityManager.addEntity((level_entities % 2 == 0) ? DEC : TILE, level_entities);
           animation_types = static_cast<eAnimationTypes>(level_entities);
           entity->addComponents<CAnimation>(m_game->getAssets().getAnimation(animation_types), true);
           entity->addComponents<CTransform>(gridToMidPixel((float) x * 64, (float) y * 64, entity));
           entity->getComponent<CTransform>().prevPos =entity->getComponent<CTransform>().pos;
           if (entity->id() == TILE)
             entity->addComponents<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize());
           // has to change have to adapt a correct bounding
           // box for all the entities'
          break;
      }
    } else {
      std::cerr << "unknown entity" << std::endl;
      getline(file, str);// go to the next line for the saving the safety of
                         // the other attributes
    }
  }
  
  
  //this order is for using level's information on the position of player
  spawnPlayer();

  /*
    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT : always add the CAnimation component first so that gridToMidPixel can compute correctly
    brick->addComponents<CAnimation>(m_game->getAssets().getAnimation("Brick"),true); 
    brick->addComponents<CTransform>(Vec2(96, 480));
    // NOTE : you finally code should position the entity with the grid x,y position read from the file
    //  brick->addComponents<CTransform>(gridToMidPixel(gridX, gridY, brick))

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
      std::cout << "This could be a good way of identifying if a tile is a brick!" << std::endl;

  */
  //  auto brick = m_entityManager.addEntity(eLevelEntities::BRICK);
  //  brick->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::AniBRICK),
  //  true); brick->addComponents<CTransform>(Vec2(224, 480));
  //  // add a bounding box, this will now show up if we press the 'C' key
  //  brick->addComponents<CBoundingBox>(m_gridSize);
  //	brick->getComponent<CTransform>().scale.y =
  // brick->getComponent<CBoundingBox>().size.y /
  // brick->getComponent<CAnimation>().animation.getSize().y;
  //	brick->getComponent<CTransform>().scale.x =
  // brick->getComponent<CBoundingBox>().size.x /
  // brick->getComponent<CAnimation>().animation.getSize().x;
  //
  //  auto question = m_entityManager.addEntity("tile");
  //  question->addComponents<CAnimation>(m_game->getAssets().getAnimation("Question"),
  //  true); question->addComponents<CTransform>(Vec2(352, 480));

  // NOTE : THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
  //        Components are now returned as reference rather than pointers
  //        if you do not specify a reference variable type, it will COPY the
  //        component here is an example
  //
  //        This will COPY the transform into the variable 'transform1' - it's
  //        INCORRECT any changes you make to transform1 will not be changed
  //        inside the entity auto transform1 = entity->get<CTransform>();
  //
  //        This will REFERENCE the transform with the variable 'transform2' -
  //        it's CORRECT Now any changes you make to transform2 will be changed
  //        inside the entity auto & transform2 = entity->get<CTransform>()
}

void Scene_Play::spawnPlayer() {
  // here is a sample player entity which you can use to construct other
  // entities

  m_player = m_entityManager.addEntity(eEntitiesType::DYNAMIC, eLevelEntities::PLAYER);
  m_player->addComponents<CGravity>().gravity = m_playerConfig.GRAVITY;
  m_player->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::STAND), true);
  m_player->addComponents<CTransform>(gridToMidPixel(64 * m_playerConfig.X, 64 * m_playerConfig.Y, m_player));
  m_player->addComponents<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
  m_player->getComponent<CTransform>().scale.y =64 / m_player->getComponent<CAnimation>().animation.getSize().y;
  m_player->getComponent<CTransform>().scale.x =64 / m_player->getComponent<CAnimation>().animation.getSize().x;
  // TODO: be sure to add the remaining components to the player
  m_player->addComponents<CHealth>(100); // Assuming CHealth is a component for health management
}

void Scene_Play::spawnBullet(const std::shared_ptr<Entity> &entity) {
  Vec2 entityPos = entity->getComponent<CTransform>().pos;
  (entity->getComponent<CTransform>().scale.x < 0) ? entityPos.x -= 52: entityPos.x += 20;
  entityPos.y += 10;
  // TODO: this should spawn a bullet at the given entity, going in the
  // direction the entity is facing
  auto bullet = m_entityManager.addEntity(DYNAMIC, eLevelEntities::BULLET);
  bullet->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::BUSTER), true);
  bullet->addComponents<CTransform>(gridToMidPixel((entityPos.x), entityPos.y, bullet));
  bullet->addComponents<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize() / 2);
  bullet->getComponent<CTransform>().velocity.x = entity->getComponent<CTransform>().scale.x * m_playerConfig.SPEED * 5;
  bullet->addComponents<CLifeSpan>(1.5);
}

void Scene_Play::spawnEnemy() {
  // TODO(due: Friday): implement all the spawn enemy stuff
  // outh use the goomba animation and be added as Tile entity
  // see the current implementation of how the goomba is built
  // form the loadLevel method
  auto enemy = m_entityManager.addEntity(DYNAMIC, GOOMBA);
  
  enemy->addComponents<CAnimation>(m_game->getAssets().getAnimation(aniGOOMBA), true);
  enemy->addComponents<CTransform>(gridToMidPixel(m_enemyConfig.X * 64, m_enemyConfig.Y * 64, enemy));
  enemy->getComponent<CTransform>().prevPos = enemy->getComponent<CTransform>().pos;
  enemy->addComponents<CBoundingBox>(enemy->getComponent<CAnimation>().animation.getSize());
  enemy->getComponent<CTransform>().velocity.x = m_enemyConfig.SPEED;

  // Set the gravity for the Goomba
  
  enemy->addComponents<CGravity>().gravity = m_enemyConfig.GRAVITY;
  
}

void Scene_Play::update() {
    if (m_paused) {
        // If the game is paused, stop or pause the music
        if (m_backgroundMusic.getStatus() == sf::Music::Playing) {
            m_backgroundMusic.pause(); // or use stop(), depending on desired behavior
        }

        sRender();
        return;
    }

    // If the game is not paused and the music is not already playing, start the music
    if (m_backgroundMusic.getStatus() != sf::Music::Playing) {
        m_backgroundMusic.play();
    }

    // The rest of your game update logic
    m_entityManager.update();
    if (!m_player->isActive()) {
        //m_isGameOver = true;
        m_backgroundMusic.stop();
        return;
    }
    sMovement();
    sCollision();
    sLifespan();
    sAnimation();
    sRender();
}

void Scene_Play::sMovement() {
  // TODO: implement player movement / jumping based on its CInput component
  // TODO: implement gravity's effect on the player
  // TODO: implement the maximum player speed in both X and Y directions
  // NOTE: setting an  entity's scale.x to -1/1 will make it face to the
  // left/right
  Vec2 vel = {m_playerConfig.SPEED,m_player->getComponent<CTransform>().velocity.y};
  // sample example of player movement, you should iterate in the entities
  if (!(m_player->getComponent<CInput>().up ||
        m_player->getComponent<CInput>().left ||
        m_player->getComponent<CInput>().right ||
        m_player->getComponent<CInput>().shoot)) {
    m_player->addComponents<CAnimation>(
        m_game->getAssets().getAnimation(eAnimationTypes::STAND), true);
    m_player->getComponent<CState>().state = STANDING;
  }

  while (true) {
    if (m_player->getComponent<CInput>().up) {
      m_player->getComponent<CTransform>().velocity.y = -(m_playerConfig.JUMP);
      if (m_player->getComponent<CTransform>().pos.y <=
          m_player->getComponent<CTransform>().aimPos) {
        m_player->getComponent<CInput>().up = false;
        m_player->getComponent<CTransform>().velocity.y = 0;
        m_player->getComponent<CState>().notGrounded = true;
      }
      break;
    }
    if (m_player->getComponent<CInput>().left) {
      m_player->getComponent<CTransform>().velocity.x = -(vel.x);
      break;
    } else
      m_player->getComponent<CTransform>().velocity.x = 0;

    if (m_player->getComponent<CInput>().right)
      m_player->getComponent<CTransform>().velocity.x = vel.x;
    else
      m_player->getComponent<CTransform>().velocity.x = 0;
    break;

  }

  // m_player->getComponent<CTransform>().prevPos =
  //     m_player->getComponent<CTransform>().pos;

  for (auto &e: m_entityManager.getEntities()) {

    if (e->hasComponent<CTransform>() && e->tag() != GOOMBA) {
      e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
      if (e->hasComponent<CGravity>() && e->getComponent<CState>().notGrounded ) {
        e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
        e->getComponent<CTransform>().pos.y += e->getComponent<CTransform>().velocity.y;
      }
      e->getComponent<CTransform>().pos.x += e->getComponent<CTransform>().velocity.x;
    }

    if (e->hasComponent<CTransform>() && e->tag() == GOOMBA) {
        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
        if (e->hasComponent<CGravity>() && e->getComponent<CState>().notGrounded) {
            e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
            e->getComponent<CTransform>().pos.y += e->getComponent<CTransform>().velocity.y;
        }
        else {
            e->getComponent<CTransform>().pos.x += e->getComponent<CTransform>().velocity.x;
        }
    }

  }

  // add spawnEnemy()
  if (m_player->getComponent<CTransform>().pos.y > height())
    spawnPlayer();
}

void Scene_Play::sLifespan() {
  // TODO: check lifespan of entities that have them, and destroy them if they
  // go over
  for (auto e: m_entityManager.getEntities()) {
    if (e->hasComponent<CLifeSpan>())
      if (e->getComponent<CLifeSpan>().lifespan <
          e->getComponent<CLifeSpan>().clock.getElapsedTime().asSeconds())
        e->destroy();
  }
}

void Scene_Play::spawnCoin(const Vec2& position) {
    // Create and configure a new coin entity
    auto coinEntity = m_entityManager.addEntity(TILE, COIN);
    coinEntity->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::aniCoin), true);
    coinEntity->addComponents<CTransform>(position);
    coinEntity->addComponents<CBoundingBox>(coinEntity->getComponent<CAnimation>().animation.getSize());
}

void Scene_Play::spawnStar(const Vec2& position) {
    
    auto coinEntity = m_entityManager.addEntity(TILE, STAR);
    coinEntity->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::anistar), true);
    coinEntity->addComponents<CTransform>(position);
    coinEntity->addComponents<CBoundingBox>(coinEntity->getComponent<CAnimation>().animation.getSize());
}

void Scene_Play::spawnMushroom(const Vec2& position) {
    
    auto coinEntity = m_entityManager.addEntity(TILE, MUSHROOM);
    coinEntity->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::animushroom), true);
    coinEntity->addComponents<CTransform>(position);
    coinEntity->addComponents<CBoundingBox>(coinEntity->getComponent<CAnimation>().animation.getSize());
}

void Scene_Play::spawnFlower(const Vec2& position) {
    
    auto coinEntity = m_entityManager.addEntity(TILE, FLOWER);
    coinEntity->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::aniflower), true);
    coinEntity->addComponents<CTransform>(position);
    coinEntity->addComponents<CBoundingBox>(coinEntity->getComponent<CAnimation>().animation.getSize());
}

void Scene_Play::sCollision() {
    Vec2 overlapRect;
    for (auto& dynamicEntity : m_entityManager.getEntities(DYNAMIC)) {
        dynamicEntity->getComponent<CState>().notGrounded = true;
        
        for (auto& multiEntity : m_entityManager.getEntities()) {
            if (dynamicEntity->getComponent<CTransform>().prevPos == multiEntity->getComponent<CTransform>().prevPos)
                continue; // Skip same dynamic entity

            if (multiEntity->id() == DEC)
                continue; // Skip decoration entities

            if (dynamicEntity->tag() == GOOMBA && multiEntity->tag() == GOOMBA) {
                // Skip collision logic for Goomba-to-Goomba collision
                continue;
            }
           

            
            

            Vec2 initialOverlap = m_physics.GetOverlap(dynamicEntity, multiEntity);
            if (initialOverlap.x > 0 && initialOverlap.y > 0 && dynamicEntity->tag() == BULLET) {
                dynamicEntity->destroy();
                continue;
            }

            overlapRect = m_physics.GetOverlap(multiEntity, dynamicEntity);
            if (dynamicEntity->tag() != GOOMBA && dynamicEntity->getComponent<CTransform>().pos.x <= 32)
                dynamicEntity->getComponent<CTransform>().pos.x += 1; // Boundary analysis

            if (overlapRect > 0) {

                if (dynamicEntity->tag() == GOOMBA) {
                    auto multiEntityTag = multiEntity->tag();
                    if (multiEntityTag == BRICK || multiEntityTag == QUESTIONBOX || multiEntityTag == NORMALBOX) {
                        if (dynamicEntity->getComponent<CState>().notGrounded == true) {
                            dynamicEntity->getComponent<CState>().notGrounded = false;
                            dynamicEntity->getComponent<CTransform>().pos.y -= (dynamicEntity->getComponent<CTransform>().pos.y - dynamicEntity->getComponent<CTransform>().prevPos.y) / std::abs(dynamicEntity->getComponent<CTransform>().pos.y - dynamicEntity->getComponent<CTransform>().prevPos.y) * overlapRect.y;
                        }
                    }
                }

                if (dynamicEntity->tag() == PLAYER && multiEntity->tag() == MarioWater) {
                    m_hurtMusic.play();
                    dynamicEntity->getComponent<CHealth>().value -= 0.2;
                    if (dynamicEntity->getComponent<CHealth>().value <= 0) dynamicEntity->destroy();
                    continue;
                }

                if (dynamicEntity->tag() == PLAYER && multiEntity->tag() == MarioFireLand) {
                    m_hurtMusic.play();
                    dynamicEntity->getComponent<CHealth>().value -= 0.7;
                    if (dynamicEntity->getComponent<CHealth>().value <= 0) dynamicEntity->destroy();
                    continue;
                }

                if (dynamicEntity->tag() == GOOMBA && multiEntity->tag() == MarioWater) {
                    dynamicEntity->destroy();
                    continue;
                }
                if (dynamicEntity->tag() == GOOMBA && multiEntity->tag() == MarioFireLand) {
                    dynamicEntity->destroy();
                    continue;
                }

                if (dynamicEntity->tag() == PLAYER && multiEntity->tag() == COIN) {
                    std::cout << "touch coin";
                    m_coinMusic.play();
                    multiEntity->destroy();
                    m_score++; // Increase the score
                    m_scoreText.setString("Score: " + std::to_string(m_score)); // Update the score text
                    continue;
                }

                if (dynamicEntity->tag() == PLAYER && multiEntity->tag() == STAR) {
                    std::cout << "touch star";
                    m_eatMusic.play();
                    multiEntity->destroy();
                    m_score = m_score+25; // Increase the score
                    m_scoreText.setString("Score: " + std::to_string(m_score)); // Update the score text
                    continue;
                }

                if (dynamicEntity->tag() == PLAYER && multiEntity->tag() == MUSHROOM) {
                    std::cout << "touch mushroom";
                    m_eatMusic.play();
                    multiEntity->destroy();
                    dynamicEntity->getComponent<CHealth>().value += 15;
                    
                    continue;
                }

                if (dynamicEntity->tag() == PLAYER && multiEntity->tag() == FLOWER) {
                    std::cout << "touch flower";
                    m_eatMusic.play();
                    multiEntity->destroy();
                    dynamicEntity->getComponent<CHealth>().value += 30;
                    continue;
                }

                if (m_physics.GetPreviousOverlap(multiEntity, dynamicEntity).x > 0) {
                    // Collision above
                    Vec2 coinPos;
                    int randomNumber = std::rand() % 100;
                    //initialize outside for case can run normally

                    

                    if (dynamicEntity->getComponent<CTransform>().pos.y < dynamicEntity->getComponent<CTransform>().prevPos.y) { 

                        if (multiEntity->tag() == GOOMBA && dynamicEntity->tag() == PLAYER) {
                            m_hurtMusic.play();
                            multiEntity->destroy();
                            dynamicEntity->getComponent<CHealth>().value -= 10;
                            if (dynamicEntity->getComponent<CHealth>().value <= 0) dynamicEntity->destroy();
                            std::cout << "Goomba kill man";
                            break;
                        }

                        switch (multiEntity->tag()) {
                            
                      

                              
                        case BRICK:
                            std::cout << "hit brick";
                            multiEntity->removeComponent<CBoundingBox>(); // remove thoroughly
                            //multiEntity->getComponent<CBoundingBox>().size = { 0, 0 };
                            multiEntity->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::EXPLOSION), false);
                            
                            dynamicEntity->getComponent<CInput>().up = false;
                            dynamicEntity->getComponent<CTransform>().velocity.y = dynamicEntity->getComponent<CGravity>().gravity;
                            break;

                        case QUESTIONBOX:
                            std::cout << "hit questionbox";
                          
                            dynamicEntity->getComponent<CTransform>().velocity.y = 9;//forbid skipping through the brick
                            dynamicEntity->getComponent<CInput>().up = false;
                            multiEntity->getComponent<CAnimation>().animation = m_game->getAssets().getAnimation(eAnimationTypes::aniNormalBox);
                            //only change his animation not put in a new one
                            // Change the tag or type of the entity to NORMALBOX
                            multiEntity->setTag(NORMALBOX);

                            coinPos = multiEntity->getComponent<CTransform>().pos;
                            coinPos.y -= m_gridSize.y; // Adjust position to spawn coin above the box
                            
                            if (randomNumber < 50) {
                                // 50% probability for coin
                                spawnCoin(coinPos);
                            }
                            else if (randomNumber < 70) {
                                // 20% probability for flower
                                spawnFlower(coinPos);
                            }
                            else if (randomNumber < 90) {
                                // 20% probability for mushroom
                                spawnMushroom(coinPos);
                            }
                            else {
                                // 10% probability for star
                                spawnStar(coinPos);
                            }

                            //spawnCoin(coinPos); // Call the function to spawn the coin
                            //spawnFlower(coinPos);
                            //spawnStar(coinPos);
                            //spawnMushroom(coinPos);
                            break;

                        case NORMALBOX:
                            std::cout << "hit normalbox";
           
                            dynamicEntity->getComponent<CTransform>().velocity.y = 9;//forbid skipping through the brick
                            dynamicEntity->getComponent<CInput>().up = false;
                            break;

                        default:
                            break;
                        }
                    }
                    else {
                        // Collision below
       
                        dynamicEntity->getComponent<CInput>().canJump = true;
                        dynamicEntity->getComponent<CState>().notGrounded = false;
                        

                        if (dynamicEntity->tag() == GOOMBA ) {

                            //std::cout << "arrive ground!!" << dynamicEntity->getComponent<CTransform>().velocity.x;
                            dynamicEntity->getComponent<CTransform>().velocity.y = 0;
                           
                        }
                        
                        dynamicEntity->getComponent<CTransform>().velocity.y = 0;
                        if (dynamicEntity->tag() == PLAYER && multiEntity->tag() == GOOMBA) {
                            multiEntity->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::aniGOODEATH), false);
                            multiEntity->getComponent<CBoundingBox>().size = { 0, 0 };
                            multiEntity->getComponent<CTransform>().has = false;  
                            std::cout << "man kill goomba";
                            //continue;
                        }
                        

                        if (dynamicEntity->tag() == PLAYER)
                            dynamicEntity->getComponent<CTransform>().pos.y -= overlapRect.y;
                 
                    }
                }
                else {
                    //dynamicEntity->getComponent<CState>().notGrounded = false;
                    // Horizontal collision
                    if (multiEntity->id() == TILE) {
                        dynamicEntity->getComponent<CTransform>().pos.x -= (dynamicEntity->getComponent<CTransform>().pos.x - dynamicEntity->getComponent<CTransform>().prevPos.x) / std::abs(dynamicEntity->getComponent<CTransform>().pos.x - dynamicEntity->getComponent<CTransform>().prevPos.x) * overlapRect.x;
                        if (dynamicEntity->tag() == BULLET)
                            dynamicEntity->destroy();
                        if (dynamicEntity->tag() == GOOMBA)
                            dynamicEntity->getComponent<CState>().notGrounded = false;
            
                            dynamicEntity->getComponent<CTransform>().velocity.x *= -1;
                            //std::cout << "horizontal attack!!"<< dynamicEntity->getComponent<CTransform>().velocity.x;
                    }
                    else {
                        // Handling for other entities
                        if ((dynamicEntity->tag() == GOOMBA && multiEntity->tag() == PLAYER) ||
                            (dynamicEntity->tag() == PLAYER && multiEntity->tag() == GOOMBA)) {
                            // Logic for PLAYER death when colliding with GOOMBA
                            if (dynamicEntity->tag() == PLAYER) {
                                m_hurtMusic.play();
                                multiEntity->destroy();
                                dynamicEntity->getComponent<CHealth>().value -= 10;
                                
                                if (dynamicEntity->getComponent<CHealth>().value <= 0) dynamicEntity->destroy();
                            }
                            else if (multiEntity->tag() == PLAYER) {
                                m_hurtMusic.play();
                                dynamicEntity->destroy();
                                multiEntity->getComponent<CHealth>().value -= 10;
                                
                                if (multiEntity->getComponent<CHealth>().value <= 0) multiEntity->destroy();
                            }
                        }
                        else if (dynamicEntity->tag() == GOOMBA && multiEntity->tag() == GOOMBA) {
                            // Skip collision logic for Goomba-to-Goomba collision
                            continue;
                        }
                        else {
                            // Default destruction logic for other entities
                            multiEntity->destroy();
                            dynamicEntity->destroy();
                        }
                    }
                }
            }
        }
    }
}

void Scene_Play::sDoAction(const Action &action) {
  if (action.type() == eActionTypes::START) {
    for (auto &e: m_entityManager.getEntities(DYNAMIC)) {
      if (e->getComponent<CInput>().canJump) {
        e->getComponent<CInput>().down = false;
        e->getComponent<CInput>().left = false;
        e->getComponent<CInput>().right = false;
        e->getComponent<CInput>().shoot = false;
      }
    }
    switch (action.name()) {
      case eActionNames::TOGGLE_TEXTURE: {
        m_drawTextures = !m_drawTextures;
        break;
      }
      case eActionNames::TOGGLE_COLLISION: {
        m_drawCollision = !m_drawCollision;
        break;
      }
      case eActionNames::TOGGLE_GRID: {
        m_drawGrid = !m_drawGrid;
        break;
      }
      case eActionNames::PAUSE: {
          m_pauseMusic.play();
        setPaused(!m_paused);
        break;
      }

      case eActionNames::QUIT: {
          // If the game is paused, stop or pause the music
          if (m_backgroundMusic.getStatus() == sf::Music::Playing) {
              m_backgroundMusic.pause(); // or use stop(), depending on desired behavior
          }
        onEnd();
        break;
      }
      case eActionNames::UP: {
        if (m_player->getComponent<CInput>().canJump) {
          m_jumpMusic.play();
          m_player->getComponent<CInput>().up = true;
          m_player->getComponent<CState>().state = JUMPING;
          m_player->addComponents<CAnimation>(
              m_game->getAssets().getAnimation(eAnimationTypes::JUMP), true);
          m_player->getComponent<CTransform>().aimPos =
              m_player->getComponent<CTransform>().pos.y -
              m_playerConfig.MAXHEIGHT;
          m_player->getComponent<CInput>().canJump = false;
          m_player->getComponent<CState>().notGrounded = true;
        }
        break;
      }
      case eActionNames::RIGHT:
        m_player->getComponent<CInput>().right = true;
        m_player->addComponents<CAnimation>(
            m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
        m_player->getComponent<CState>().state = RUNNING;
        (m_player->getComponent<CTransform>().scale.x < 0)
            ? m_player->getComponent<CTransform>().scale.x *= -1
            : 1;
        break;
      case eActionNames::LEFT:
        m_player->getComponent<CInput>().left = true;
        m_player->addComponents<CAnimation>(
            m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
        (m_player->getComponent<CTransform>().scale.x > 0)
            ? m_player->getComponent<CTransform>().scale.x *= -1
            : 1;
        m_player->getComponent<CState>().state = RUNNING;
        break;
      case FIRE:
          m_shootMusic.play();
        m_player->getComponent<CInput>().shoot = true;
        m_player->addComponents<CAnimation>(
            m_game->getAssets().getAnimation(eAnimationTypes::SHOT), true);
        spawnBullet(m_player);
        m_player->getComponent<CState>().state = SHOUTING;
        break;
    }
  } else if (action.type() == eActionTypes::END) {
    // has to be fixed the movement bug
    switch (action.name()) {
      case eActionNames::RIGHT:
        m_player->getComponent<CInput>().right = false;
        break;
      case eActionNames::LEFT:
        m_player->getComponent<CInput>().left = false;
        break;
      case eActionNames::UP:
        if (m_player->getComponent<CInput>().up) {
          m_player->getComponent<CInput>().up = false;
          m_player->getComponent<CTransform>().velocity.y = 0;
          m_player->getComponent<CState>().notGrounded = true;
        }
        break;
      case eActionNames::FIRE:
        m_player->getComponent<CInput>().shoot = false;
        m_player->getComponent<CState>().state = STANDING;
        break;
    }
  }
}

void Scene_Play::sAnimation() {
  // TODO: complete the Animation class code first

  // TODO: set the animation of the player based on its CState component
  // TODO: for each entity with an animation, call
  // entity->getComponent<CAnimation>().animation.update();
  //       if the animation is not repeated, and it has ended, destroy the
  //       entity
  for (auto &e: m_entityManager.getEntities()) {
    if (e->hasComponent<CAnimation>()) {
      e->getComponent<CAnimation>().animation.update();
      if (e->getComponent<CAnimation>().animation.hasEnded() &&
          !e->getComponent<CAnimation>().repeat)
        e->destroy();
    }
  }
}

void Scene_Play::onEnd() {
  // TODO: when the scene ends, change back to the MENU scene
  //       use m_game->changeState(correct params)
  m_game->changeScene(MENU, m_game->getScene(MENU));
}

void Scene_Play::sRender() {
  // color the background darker, so you know that the game is paused
  if (!m_paused) {
    m_game->window().clear(sf::Color(87, 174, 209));

  } else {
    
    m_game->window().clear(sf::Color(50, 50, 150));
   
  }

  

  if (m_paused) {
      //m_game->window().clear(sf::Color(87, 174, 209));
      // Save the current view
      sf::View currentView = m_game->window().getView();

      // Set the view to the default view to draw the score
      m_game->window().setView(m_game->window().getDefaultView());

      // Render the score text
      m_game->window().draw(m_pauseText);

      // Restore the original view
      m_game->window().setView(currentView);
  }

  // Save the current view
  sf::View currentView = m_game->window().getView();

  // Set the view to the default view to draw the score
  m_game->window().setView(m_game->window().getDefaultView());

  // Render the score text
  m_game->window().draw(m_scoreText);

  // Restore the original view
  m_game->window().setView(currentView);

  //instruction
  //在更改视图之前，保存当前视图。
  //在渲染得分文本之前，将窗口视图设置为默认视图。
  //绘制得分文本。
  //绘制得分文本后，恢复原始视图。


  if (m_player->isActive()) {
      auto& health = m_player->getComponent<CHealth>();
      float healthBarWidth = 60.0f; // Width of the health bar at full health
      float currentHealthWidth = healthBarWidth * (health.value / 100.0f);

      sf::RectangleShape healthBarBackground(sf::Vector2f(healthBarWidth, 10));
      healthBarBackground.setFillColor(sf::Color::Black);
      healthBarBackground.setPosition(m_player->getComponent<CTransform>().pos.x - 30, m_player->getComponent<CTransform>().pos.y - 55);

      sf::RectangleShape healthBar(sf::Vector2f(currentHealthWidth, 10));
      healthBar.setFillColor(sf::Color::Red);
      healthBar.setPosition(m_player->getComponent<CTransform>().pos.x - 30, m_player->getComponent<CTransform>().pos.y - 55);

      m_game->window().draw(healthBarBackground);
      m_game->window().draw(healthBar);
  }

  


  // set the viewport of the window to be centered on the player if its far
  // enough right
  auto &pPos = m_player->getComponent<CTransform>().pos;
  float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
  sf::View view = m_game->window().getView();
  view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
  m_game->window().setView(view);


  // draw all Entity textures / animations
  if (m_drawTextures) {
    for (auto &e: m_entityManager.getEntities()) {
      auto &transform = e->getComponent<CTransform>();
      if (e->hasComponent<CAnimation>()) {
        auto &animation = e->getComponent<CAnimation>().animation;
        animation.getSprite().setRotation(transform.angle);
        animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
        animation.getSprite().setScale(transform.scale.x, transform.scale.y);
        m_game->window().draw(animation.getSprite());
      }
    }
  }



  // draw all Entity collision bounding boxes with a rectangle-shape
  if (m_drawCollision) {
    for (auto &e: m_entityManager.getEntities()) {
      if (e->hasComponent<CBoundingBox>()) {
        auto &box = e->getComponent<CBoundingBox>();
        auto &transform = e->getComponent<CTransform>();
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
        rect.setOrigin(sf::Vector2f(box.size.x / 2, box.size.y / 2));
        rect.setPosition(transform.pos.x, transform.pos.y);
        rect.setFillColor(sf::Color(0, 0, 0, 0));
        rect.setOutlineColor(sf::Color(255, 255, 255, 255));
        rect.setOutlineThickness(1);
        m_game->window().draw(rect);
      }
    }
  }

  // draw gri
  if (m_drawGrid) {
    for (float x = m_gridSize.x; x <= width(); x += m_gridSize.x)
      drawLine(Vec2(x, 0), Vec2(x, height()));

    for (float y = 0; y < height(); y += m_gridSize.y) {
      drawLine(Vec2(0, height() - y), Vec2(width(), height() - y));

      for (int i = 0; i < width(); i += m_gridSize.x) {
        for (int j = 0; j < height(); j += m_gridSize.y) {
          m_gridText.setPosition(i, j);
          m_gridText.setCharacterSize(12);
          m_gridText.setString("(" + std::to_string((int) i / 64) + "," +
                               std::to_string((int) j / 64) + ")");
          ;
          m_game->window().draw(m_gridText);
        }
      }
    }
  }
  if (!m_player->isActive()) {
      
      m_game->window().clear(sf::Color(190, 40, 1)); // 结算页面背景颜色
      // 设置并渲染 "GAME OVER"
      gameOverText.setFont(m_game->getAssets().getFont(eFontTypes::PIXEL));
      gameOverText.setString("GAME OVER");
      gameOverText.setCharacterSize(70); // 大号字体
      gameOverText.setFillColor(sf::Color(255, 211, 177)); // 字体颜色

      sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
      gameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width / 2.0f, gameOverBounds.top + gameOverBounds.height / 2.0f);
      gameOverText.setPosition(m_game->window().getSize().x / 2.0f, m_game->window().getSize().y / 2.0f - 150);

      // 设置并渲染分数
      m_scoreText.setCharacterSize(32); // 大号字体
      m_scoreText.setFillColor(sf::Color(255, 211, 177)); // 字体颜色
      sf::FloatRect scoreBounds = m_scoreText.getLocalBounds();
      m_scoreText.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f, scoreBounds.top + scoreBounds.height / 2.0f);
      m_scoreText.setPosition(m_game->window().getSize().x / 2.0f, m_game->window().getSize().y / 2.0f);
      

      // 设置并渲染分享提示
      shareText.setFont(m_game->getAssets().getFont(eFontTypes::PIXEL));
      shareText.setString("Please take a screenshot of the game's settlement screen to share on your Moments");
      shareText.setCharacterSize(16); // 小号字体
      shareText.setFillColor(sf::Color::Black); // 字体颜色

      sf::FloatRect shareBounds = shareText.getLocalBounds();
      shareText.setOrigin(shareBounds.left + shareBounds.width / 2.0f, shareBounds.top + shareBounds.height / 2.0f);
      shareText.setPosition(m_game->window().getSize().x / 2.0f, m_game->window().getSize().y / 2.0f + 80);
      

      // 设置并渲染分享提示
      pressText.setFont(m_game->getAssets().getFont(eFontTypes::PIXEL));
      pressText.setString("Press the Q key to return to the menu page");
      pressText.setCharacterSize(16); // 小号字体
      pressText.setFillColor(sf::Color::Black); // 字体颜色
      
      
      sf::FloatRect pressBounds = pressText.getLocalBounds();
      pressText.setOrigin(pressBounds.left + pressBounds.width / 2.0f, pressBounds.top + pressBounds.height / 2.0f);
      pressText.setPosition(m_game->window().getSize().x / 2.0f, m_game->window().getSize().y / 2.0f + 100);


      sf::View currentView2 = m_game->window().getView();

      // Set the view to the default view to draw the score
      m_game->window().setView(m_game->window().getDefaultView());

      // Render the score text
      m_game->window().draw(gameOverText);
      m_game->window().draw(m_scoreText);
      m_game->window().draw(shareText);
      m_game->window().draw(pressText);
      // Restore the original view
      m_game->window().setView(currentView2);

  }
 
  m_game->window().display();
}
