#pragma once

#include "Components.h"
#include <string>
#include <tuple>

enum eLevelEntities {
  PLAYER,
  BRICK,
  CLOUDBIG,
  GROUND,
  BUSHBIG,
  QUESTION = 5,
  BUSHSMALL,
  FLAGPOLE,
  PIPESMALL,
  BLOCK,
  BUSH,
  COIN = 11,
  CLOUDSMALL,
  BULLET,
  SMALLBUSH,
  PIPE,
  BUSHER = 16,
  GOOMBA,
  BCLOUD = 18,
  QUESTIONBOX = 23,
  MarioWater = 25,
  MarioFireLand = 27,
  NORMALBOX = 29,
  STAR = 31,
  MUSHROOM = 33,
  FLOWER = 35,
  LAST
};

enum eEntitiesType {
  DYNAMIC,
  TILE,
  DEC,
};

class EntityManager;

typedef std::tuple<CTransform, CLifeSpan, CInput, CBoundingBox, CAnimation, CGravity, CState, CHealth>
    ComponentsTuple;

class Entity {
  friend class EntityManager;

  bool m_active = true;
  eEntitiesType m_id;


  ComponentsTuple m_components = std::make_tuple(CTransform(),
                                                 CLifeSpan(5),
                                                 CInput(),
                                                 CBoundingBox(),
                                                 CAnimation(),
                                                 CGravity(),
                                                 CState(),
                                                 CHealth());

  // constructor is private so we can never create
  // entities outside the EntityManager which had friend access
  Entity(eEntitiesType &id, eLevelEntities &entityTag);
  Entity();

  protected:
  eLevelEntities m_tag;

  public:
  void destroy();
  eEntitiesType id() const;
  bool isActive() const;
  eLevelEntities tag() const;


  void setTag(eLevelEntities newTag) {
      m_tag = newTag;
  }

  

  template<typename T>
  bool hasComponent() {
    return getComponent<T>().has;
  }

  template<typename T, typename... TArgs>
  T &addComponents(TArgs &&...mArgs) {
    auto &component = getComponent<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.has = true;
    return component;
  }

  template<typename T>
  T &getComponent() { return std::get<T>(m_components); }

  template<typename T>
  void removeComponent() { getComponent<T>() = T(); }
};
