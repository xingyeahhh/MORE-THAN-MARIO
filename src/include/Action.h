#pragma once
#include <string>

enum eActionTypes {
  END = 0,
  START
};

enum eActionNames {
  TOGGLE_TEXTURE = 0,
  TOGGLE_COLLISION,
  TOGGLE_GRID,
  PAUSE,
  QUIT,
  UP,
  RIGHT,
  FIRE,
  LEFT,
};

class Action {
  eActionNames m_name;
  eActionTypes m_type;

  public:
  Action();
  Action(eActionNames name, const eActionTypes &type);

  eActionNames name() const;
  eActionTypes type() const;
  //Adding const to a member function promises not to modify any 
  // member variables of the object. This means that this function 
  // cannot change any non-static member variables of the class. 
  // If the function attempts to modify a member variable (except 
  // those declared as mutable), the compiler will report an error.
};
