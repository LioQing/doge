# Data-Oriented Game Engine

namespace: doge

A game engine designed for writing data-oriented code.

Data-oriented code allows for easier debugging and tracking of variables.

It also allows for more modular design, and hence doge also support adding extensions easily.

This library uses [SFML](https://www.sfml-dev.org/), so SFML has to be installed.

This library also contains code from [ivanfratric/polypartition](https://github.com/ivanfratric/polypartition) for triangulation.



## Built-in Extensions

### physics

provide rigid body physics simulation (with [Box2D](https://box2d.org/))

### nine_slice

provide interface for 9-slice sprites using CompoundSprite

### anim

provide simple sprite sheet animation functions



## Installing

1. Add the include directory.
2. Link the libraries.
3. Also include SFML into your project (And Box2D if you are going to use doge::phy).
4. Put the assets folder into the executable output folder.

If you are in Windows, you also need to place the openal32.dll in the executable output folder.



## Dependencies

- [SFML](https://www.sfml-dev.org/)
- [Box2D](https://box2d.org/)