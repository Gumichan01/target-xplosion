# Target Xplosion #

![tx-gif][]

2D shoot'em up game - by Gumichan01

## About ##

Target Xplosion is a 2D scrolling shooter game inspired by R-Type, Dodonpachi, and Mushihimesama.

It was developed in C++, and uses [LunatiX][].

Target Xplosion is available under the GNU GPL v3. But additionnal contents (images, font, music) are under Creative Commons.

## Game System ##

You have several types of shots:

- Normal shot
- Homing missiles
- Bomb (available in level 3)

The game also integrates a combo system. Each time you shoot an enemy, the combo value increase. So you can earn more points when you kill enemies or take a specific bonus item. But be careful! If you get hit or die, you will lose your combo.

## Control ##

> In Game

**Keyboard**:

  - Arrows: Move the ship
  - W: basic shot
  - X: Missile
  - C: Bomb
  - LEFT SHIFT: slow motion of the ship
  - ESCAPE: Quit the level/Game
  - BACKSPACE: Quit the Game

**Xbox controller**:

  - Left stick: Move the ship
  - RB: basic shot
  - A: Missile
  - X: Bomb
  - START: Quit the level
  - B (main menu): Quit the game


> In result screen

  - RETURN (keyboard) / A (Xbox controller) : continue

## Demo ##

Releases are available [here][].
You can get the current developement game thanks to the source code, but you need to compile the game.

## Compilation ##

If you want to compile the game from the source code. You will have to follow these instructions.

### Prerequisites ###

The game depends on [LunatiX][] and [TinyXML2][].
TinyXML2 is provide, but you need to generate the library files of LunatiX
in order to compile the game.
Please take a look on the README of LunatiX for more information about how to install it.

Since the repository contains big files that are indexed by [git-lfs][],
you must install *git-lfs* in your computer in order to retrieve the repository
and work on it.

To compile, you just need to execute the following command:

    $make

## Videos ##

 * [Gameplay video][gp]
 * [Game system][gs]

## Links ##

 * [Twitter page][twitter]: I'm using it as my devblog
 * [OpenGameArt page][oga]: The place where I store all of the assets of the game
 * [Youtube page][youtube]

---
[tx-gif]: https://github.com/Gumichan01/gumichan01.github.io/raw/master/portfolio/image/tx.gif
[LunatiX]: https://github.com/Gumichan01/lunatix
[TinyXML2]: https://github.com/leethomason/tinyxml2
[here]: https://github.com/Gumichan01/target-xplosion/releases
[git-lfs]: https://github.com/git-lfs/git-lfs/wiki/Installation
[gp]: https://youtu.be/GqSZv295g_k
[gs]: #
[twitter]: https://twitter.com/Gumichan01
[oga]: https://opengameart.org/users/gumichan01
[youtube]: https://www.youtube.com/user/gumichan01
