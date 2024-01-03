#pragma once

#include "Animation.h"
#include "Common.h"


// the numbers are the ascii values of the first letter of the types
enum eAssetsTypes { TEXTURE = 84,
                    ANIMATION = 65,
                    FONT = 70,
                    SOUND = 83};

enum eTextureTypes {
  marioBRICK = 1,
  megaRUN,
  megaJUMP,
  megaSTAND,
  megaSHOT,
  megaBuster,
  marioExplosion,
  marioBUSH,
  marioSMALLBUSH = 10,
  mariocoin = 11,
  marioCLOUD = 12,
  marioBOOMBA,
  marioBUSHER = 14,
  marioPIPE,
  marioBCLOUD = 16,
  marioGOODEATH,
  marioquestionbox = 23,
  mariowater = 25,
  mariofireland = 27,
  marionormalBox = 29,
  mariostar = 31,
  mariomushroom = 33,
  marioflower = 35,
  marioboss = 37,
  marioboss2 = 39,
  mariobossshoot = 41,
  mariLONGPIPE = 43,
  mariWALL = 45,
  TLAST
};

enum eSoundTypes {
    jumpSOUND = 1,
    musicSOUND,
    COINSOUND,
    SHOOTSOUND,
    GAMEOVERSOUND,
    DIESOUND,
    PAUSESOUND,
    EATINGSOUND,
    HURTSOUND,
    WINSOUND,
    // Add other sound types here
    SLAST
};

enum eFontTypes { NUMBERS = 1,
                  PIXEL,
                  FLAST };

class Assets {
  std::map<eTextureTypes, sf::Texture> m_textureMap;
  std::map<eAnimationTypes, Animation> m_animationMap;
  std::map<eFontTypes, sf::Font> m_fontMap;
  std::map<eSoundTypes, sf::SoundBuffer> m_soundMap; // Added for sound management
  //  std::vector<std::string>                          m_animation_typess;

  void addTexture(eTextureTypes &texture_types, const std::string &path, bool smooth = true);
  void addAnimation(eAnimationTypes &animation_types,
                    eTextureTypes &texture_types, size_t frameCount,
                    size_t speed);
  void addFont(eFontTypes &fontName, const std::string &path);
  void addSound(eSoundTypes soundType, const std::string& path);
  public:
  Assets();

  void loadFromFile(const std::string &path);

  const sf::Texture &getTexture(eTextureTypes texture_types) const;
  const Animation &getAnimation(eAnimationTypes animation_types) const;
  //  const std::vector<std::string> & getanimation_typess() const;
  const sf::Font &getFont(eFontTypes fontName) const;
  const sf::SoundBuffer& getSound(eSoundTypes soundType) const;
};
