# Target Xplosion #

![tx-gif][]

Target Xplosion is a shoot'em up video game.
It was developed in C++, and uses [LunatiX][].

Target Xplosion is available under the GNU GPL v3. But additionnal contents (images, font, music) are under Creative Commons.


## Demo ##

Releases are available [here][].
You can get the current developement game thanks to the source code.
But you need to compile the game, and the developement version is unstable.

### Prerequisites ###

Since the repository contains big files that are indexed by [git-lfs][],
you must install *git-lfs* in your computer in order to retrieve the repository
and work on it.

## Control ##

> In Game

**Keyboard**:

  - Arrows: Move the ship
  - W: basic shot
  - X: Missile
  - C: Bomb
  - LEFT SHIFT: slow motion of the ship
  - ESCAPE : Quit the level/Game

**Xbox controller**:
  
  - Left stick: Move the ship
  - RB: basic shot
  - A: Missile
  - X: Bomb
  - START: Quit the level
  - B (main menu): Quit the gama


> In result screen

  - RETURN (keyboard) / A (Xbox controller) : continue


## Compilation ##

The game depends on [LunatiX][] and [TinyXML2][].
TinyXML2 library is provided, but you need to generate the library file of LunatiX
in order to compile the game.
Please take a look on the README of LunatiX for more information about how to install it.

To compile, you just need to execute the following command:

    $make


## Videos ##

 * [First Gameplay footage][g1]
 * [Second Gameplay footage][g2]

---
[tx-gif]: https://github.com/Gumichan01/gumichan01.github.io/raw/master/portfolio/image/tx.gif
[LunatiX]: https://github.com/Gumichan01/lunatix
[TinyXML2]: https://github.com/leethomason/tinyxml2
[here]: https://github.com/Gumichan01/target-xplosion/releases
[git-lfs]: https://github.com/git-lfs/git-lfs/wiki/Installation
[g1]: https://youtu.be/g9j2TCowB8A
[g2]: https://youtu.be/2yA6yp9MBs8