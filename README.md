# Target Xplosion #

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

In Game

  - Arrows: Move the ship
  - W: basic shot
  - X: Missile
  - C: Bomb
  - LEFT SHIFT: slow motion of the ship
  - ESCAPE : Quit the level/Game

In result screen

  - RETURN : continue


## Compilation ##

The game depends on LunatiX and TinyXML2.
TinyXML2 library is provided, but you need to generate the library file of LunatiX
in order to compile the game.
Please take a look on the README of LunatiX for more information about how to install it.

To compile, you just need to execute the following command:

    $make


## Videos ##

 * [Screen cancel] (https://youtu.be/93-SQ1IHczk)
 * [Circle pattern in Game] (https://youtu.be/UJDJcMnYdTs)
 * [BulletPattern: Circle] (https://www.youtube.com/watch?v=YbJIrxNCHXE)
 * [Shot on target] (https://www.youtube.com/watch?v=bn9uyc8VS3I)
 * [Boss explosion] (https://www.youtube.com/watch?v=V8S_vlE1q8M)
 * [New Shots] (https://www.youtube.com/watch?v=0pTTyYSnGSA)
 * [Bullet hell test] (https://www.youtube.com/watch?v=BtfDJbHe7gs)

---
[LunatiX]: https://github.com/Gumichan01/lunatix
[here]: https://github.com/Gumichan01/target-xplosion/releases
[git-lfs]: https://github.com/git-lfs/git-lfs/wiki/Installation
