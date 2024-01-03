#pragma once
#include "Vec2.h"

#include <vector>

#include <SFML/Graphics.hpp>

enum eAnimationTypes {
  AniBRICK = 1,
  RUN,
  JUMP,
  STAND,
  SHOT,
  BUSTER,
  EXPLOSION,
  AniBUSH = 10,
  aniCoin,
  AniCLOUDSMALL = 12,
  aniSMALLBUSH = 14,
  aniPIPE,
  aniBUSHER,
  aniGOOMBA,
  aniBCLOUD = 18,
  aniGOODEATH,
  aniQUESTIONBOX = 23,
  aniwater = 25,
  anifireland = 27,
  aniNormalBox = 29,
  anistar = 31,
  animushroom = 33,
  aniflower = 35,
  aniboss = 37,
  aniboss2 = 39,
  anibossSHOOT = 41,
  aniLONGPIPE = 43,
  aniWALL = 45,
  ALAST
};

class Animation {
  sf::Sprite m_sprite;
  size_t m_frameCount = 1;  // total number of frames of animation
  size_t m_currentFrame = 0;// the current frame of animation being played
  size_t m_speed = 0;       // the speed to play this animation
  Vec2 m_size = {1, 1};     // size of the animation frame
  eAnimationTypes m_name;

  public:
  Animation();
  Animation(eAnimationTypes &name, const sf::Texture &t);
  Animation(eAnimationTypes &name, const sf::Texture &t, size_t frameCount,
            size_t speed);
  // In C++, it is possible to define multiple constructors for the same class, 
  // each constructor having a different parameter list.This is known as constructor 
  // overloading.It allows for providing different initialization data when creating 
  // objects of the class.
  void update();
  bool hasEnded() const;
  eAnimationTypes getName() const;
  const Vec2 &getSize() const;
  sf::Sprite &getSprite();
};
