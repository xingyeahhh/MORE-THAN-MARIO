# MORE-THAN-MARIO
## Introduction
MORE-THAN-MARIO is not just a game; it's an adventure into a world where every element is meticulously crafted using advanced Entity-Component-System (ECS) architecture, built with C++ and SFML. This game offers an immersive platforming experience, combining classic mechanics with modern game development techniques.
![image](https://github.com/xingyeahhh/MORE-THAN-MARIO-GAME/assets/123461462/47c2b655-a328-43c0-8cb1-64921d3ae87d)

## Core Features
ECS Architecture: Our game distinguishes itself through its use of ECS, a powerful architectural pattern that separates logic (Systems) from data (Components), applied to game entities. This separation results in more organized, efficient, and maintainable code.

Dynamic Gameplay: Players navigate through levels, encountering various challenges and enemies. The Scene_Play class orchestrates game levels, integrating player and enemy configurations, and managing diverse game states.
Rich Audiovisual Experience: SFML powers our game's graphics and sound, providing a rich and immersive experience. From background music to sound effects for actions like jumping and collecting items, every aspect is carefully designed for an engaging gameplay experience.

Physics and Collision Detection: The Physics class offers a custom approach to handling game physics and collision detection, adding realism and depth to player interactions with the game world.
Modular Entity Management: The EntityManager class manages entities dynamically, making it easy to add, update, and remove entities during gameplay. This flexibility is key to creating a varied and evolving game environment.
Customizable Player and Enemy Behaviors: The game features a robust configuration system for player and enemy entities, allowing for varied and complex behaviors as defined in PlayerConfig and EnemyConfig.

### Game Menu Screen
![image](https://github.com/xingyeahhh/MORE-THAN-MARIO-GAME/assets/123461462/0d888528-2f71-411a-a1fd-fea3711c43a1)

### Game Menu Screen

# MORE-THAN-MARIO - Game Architecture Overview
## Component Classes
### Class: Components
- Attributes:
  - `has: bool`
### Class: CTransform (Inherits from Components)
- Attributes:
  - `pos: Vec2`
  - `prevPos: Vec2`
  - `scale: Vec2`
  - `velocity: Vec2`
  - `angle: float`
  - `aimPos: float`
- Constructors:
  - `CTransform()`
  - `CTransform(const Vec2 &p)`
  - `CTransform(const Vec2 &p, const Vec2 &sp, const Vec2 &sc, float a)`
### Class: CBoundingBox (Inherits from Components)
- Attributes:
  - `size: Vec2`
- Constructors:
  - `CBoundingBox()`
  - `CBoundingBox(const Vec2 &s)`
### Class: CLifeSpan (Inherits from Components)
- Attributes:
  - `clock: sf::Clock`
  - `lifespan: float`
- Constructors:
  - `CLifeSpan(int l)`
### Class: CInput (Inherits from Components)
- Attributes:
  - `up: bool`
  - `down: bool`
  - `left: bool`
  - `right: bool`
  - `shoot: bool`
  - `canShoot: bool`
  - `canJump: bool`

### Class: CAnimation (Inherits from Components)
- Attributes:
  - `animation: Animation`
  - `repeat: bool`
- Constructors:
  - `CAnimation()`
  - `CAnimation(const Animation &animation, bool r)`

### Class: CGravity (Inherits from Components)
- Attributes:
  - `gravity: float`
  - `knockback: int`
- Constructors:
  - `CGravity()`
  - `CGravity(float g, int kb)`

### Class: CState (Inherits from Components)
- Attributes:
  - `state: eComponentStates`
  - `frames: size_t`
  - `notGrounded: bool`
- Constructors:
  - `CState()`
  - `CState(eComponentStates &s)`

### Class: CHealth (Inherits from Components)
- Attributes:
  - `value: int`
- Constructors:
  - `CHealth()`
  - `CHealth(int val)`

## Action Class

### Enum: eComponentStates
- Values:
  - `JUMPING`
  - `FALLING`
  - `RUNNING`
  - `STANDING`
  - `SHOUTING`

### Enum: eActionTypes
- Values:
  - `END`
  - `START`

### Enum: eActionNames
- Values:
  - `TOGGLE_TEXTURE`
  - `TOGGLE_COLLISION`
  - `TOGGLE_GRID`
  - `PAUSE`
  - `QUIT`
  - `UP`
  - `RIGHT`
  - `FIRE`
  - `LEFT`

### Class: Action
- Attributes:
  - `m_name: eActionNames`
  - `m_type: eActionTypes`
- Methods:
  - `Action()`
  - `Action(eActionNames name, const eActionTypes &type)`
  - `name() const: eActionNames`
  - `type() const: eActionTypes`

## Class: GameEngine
- Attributes:
  - `assets: Assets`
  - `window: sf::RenderWindow`
  - `currentScene: Scene*`
  - `scenes: map<string, Scene*>`
  - `running: bool`
- Methods:
  - `init(): void`
  - `currentScene(): Scene*`
  - `run(): void`
  - `update(): void`
  - `changeScene(scene: Scene*): void`
  - `quit(): void`
  - `getAssets(): Assets &`
  - `window(): sf::RenderWindow &`
  - `simulate(frames: int): void`
  - `registerAction(inputKey: int, actionName: eActionNames): void`

## Class: Scene (Abstract Base)
- Attributes:
  - `game: GameEngine*`
  - `entityManager: EntityManager`
  - `actionMap: ActionMap`
  - `paused: bool`
  - `hasEnded: bool`
- Methods:
  - `onEnd() = 0`
  - `setPaused(paused: bool): void`
  - `update() = 0`
  - `sDoAction(action: Action) = 0`
  - `sRender() = 0`
  - `simulate(frames: size_t): void`
  - `registerAction(inputKey: int, actionName: eActionNames): void`
  - `drawLine(p1: Vec2, p2: Vec2): void`

## Class: Scene_Play (Inherits from Scene)
- Attributes:
  - `levelPath: string`
  - `player: Entity*`
  - `playerConfig: PlayerConfig`
- Methods:
  - `init(): void`
  - `update(): void`
  - `loadLevel(filePath: string): void`
  - `spawnPlayer(): void`
  - `sMovement(): void`
  - `sLifespan(): void`
  - `sCollision(): void`
  - `sDoAction(action: Action): void`
  - `sAnimation(): void`
  - `sRender(): void`

## Class: Scene_Menu (Inherits from Scene)
- Attributes:
  - `menuStrings: vector<string>`
  - `menuText: sf::Text`
  - `menuIndex: int`
- Methods:
  - `init(): void`
  - `update(): void`
  - `sRender(): void`
  - `sDoAction(action: Action): void`

## Class: EntityManager
- Attributes:
  - `entities: vector<Entity*>`
  - `entityMap: map<string, Entity*>`
  - `toAdd: vector<Entity*>`
- Methods:
  - `init(): void`
  - `update(): void`
  - `addEntity(args...): Entity*`
  - `getEntities(): vector<Entity*> &`
  - `getEntities(type: string): vector<Entity*> &`

## Class: Entity
- Attributes:
  - `active: bool`
  - `id: eEntitiesType`
- Methods:
  - `destroy(): void`
  - `addComponent<C>(args...): void`
  - `hasComponent<C>(): bool`
  - `getComponent<C>(): C &`
  - `removeComponent<C>(): void`

## Class: Physics
- Methods:
  - `isCollision(entityA: Entity, entityB: Entity): bool`
  - `intersect(lineA: Line, lineB: Line): bool`
  - `inside(point: Vec2, line: Line): bool`

## Class: Vec2
- Attributes:
  - `x: double`
  - `y: double`
- Methods:
  - `operator==(): bool`
  - `operator!=(): bool`
  - `operator+(): Vec2`
  - `operator-(): Vec2`
  - `operator/(): Vec2`
  - `operator*(): Vec2`
  - `normalize(): void`
  - `length(): double`

## Class: Assets
- Attributes:
  - `textures: map<string, sf::Texture>`
  - `animations: map<string, Animation>`
  - `sounds: map<string, sf::Sound>`
  - `fonts: map<string, sf::Font>`
- Methods:
  - `addTexture(name: string, path: string): void`
  - `addAnimation(name: string, animation: Animation): void`
  - `addSound(name: string, sound: sf::Sound): void`
  - `addFont(name: string, path: string): void`
  - `getTexture(name: string): sf::Texture &`
  - `getAnimation(name: string): Animation &`
  - `getSound(name: string): sf::Sound &`
  - `getFont(name: string): sf::Font &`

## Class: Animation
- Attributes:
  - `sprite: sf::Sprite`
  - `frameCount: int`
  - `currentFrame: int`
  - `speed: int`
  - `size: Vec2`
  - `name: string`
- Methods:
  - `update(): void`
  - `hasEnded(): bool`
  - `getName(): string`
  - `getSize(): Vec2 &`
  - `getSprite(): sf::Sprite &`




## ECS Implementation Insights
In MORE-THAN-MARIO, ECS is more than a pattern; it's a philosophy. We leverage it to create a game that is:

Flexible: Entities are modular, and components can be mixed and matched to create diverse functionalities.

Efficient: Systems like sMovement, sCollision, and sRender operate on entities with relevant components, optimizing game performance.

Evolving: New game features can be added seamlessly by introducing new components and systems without disrupting existing code.

## Contribute and Connect
Join our development journey! Contributions to enhance gameplay, improve code efficiency, or extend features are always welcome. We value community feedback and collaboration.

## Acknowledgements
I extend our gratitude to the SFML community and C++ enthusiasts who inspired and supported the development of this game. Special thanks to the creators and contributors of the SFML library for providing the tools that brought our game to life.

Special thanks to COMP4300 for providing the inspiration and foundational concepts that have greatly contributed to the development of this project.
https://www.youtube.com/watch?v=Jo5DlIrNiaE&list=PL_xRyXins84_Jf-aCh7chj47HR4oZLPwK&index=11

