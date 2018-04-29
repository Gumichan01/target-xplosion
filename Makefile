
#
#	Target Xplosion - A classic shoot'em up video game
#	Copyright © 2017 Luxon Jean-Pierre
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#	Luxon Jean-Pierre (Gumichan01)
#	website: https://gumichan01.github.io/
#	mail: luxon.jean.pierre@gmail.com
#

# Makefile - Target Xplosion

.PHONY: clean mrproper all

# You can modify the value of DEBUG
# If you want to use debug or release mode
DEBUG=yes

CC=g++
MAIN_OBJ=main.o
OBJS=TargetXplosion.o Background.o Character.o Item.o Engine.o EntityHandler.o \
AudioHandler.o Hud.o Entity.o Enemy.o Player.o Scoring.o Strategy.o Missile.o \
TreeMissile.o Bomb.o BasicEnemy.o Bachi.o Kamikaze.o TargetShooter.o NetShooter.o \
Heaviside.o Rocket.o Laser.o Level.o LoadingScreen.o Boss.o Airship.o SemiBoss01.o \
SemiBoss02.o SemiBoss03.o Boss01.o Boss02.o Boss03.o Boss04.o TX_Asset.o Result.o \
Bullet.o BulletPattern.o Tower.o PlayerVisitor.o EnemyResourceManager.o \
MissileResourceManager.o PlayerResourceManager.o SoundResourceManager.o \
ExplosionResourceManager.o MenuResourceManager.o ResourceManager.o \
Framerate.o EnemyLoader.o PlayerInput.o Menu.o GUI.o OptionHandler.o \
GamepadControl.o WinID.o tinyxml2.o

# Path to main file directory
MAIN_PATH=./src/
MAIN_SRC=$(MAIN_PATH)main.cpp

# Executable file
TARGETX_EXE=targetxplosion-v0.5.2

# Path to directory and include directory
TARGETX_TX_PATH=./src/tx/
TARGETX_ENTITY_PATH=./src/entities/
TARGETX_GAME_PATH=./src/game/
TARGETX_ENGINE_PATH=$(TARGETX_GAME_PATH)engine/
TARGETX_LEVEL_PATH=./src/level/
TARGETX_PATTERN_PATH=./src/pattern/
TARGETX_BOSS_PATH=$(TARGETX_ENTITY_PATH)boss/
TARGETX_XML_PATH=./src/asset/
TARGETX_RC_PATH=./src/resources/
TARGETX_UI_PATH=./src/ui/
TARGETX_OPT_PATH=./src/option/
TARGETX_UTIL_PATH=./src/utils/
SDL2_I_PATH=`pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf`
LUNATIX_I_PATH=./include/

# Libraries
LUNATIX_STATIC_LIB=./lib/linux/libLunatix.a
LUNATIX_SHARED_LIB=./lib/linux/libLunatix.so
SDL_LFLAGS=`pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf`

# Warning flags
WMISC_FLAGS=-Wlogical-op -Wuseless-cast -Wdouble-promotion -Wformat=2 \
-Wmissing-declarations -Woverloaded-virtual -fno-common
WFLAGS=-Wall -Wextra -pedantic -Weffc++


# Select flags according to the compilation mode
ifeq ($(DEBUG),yes)

	# Debug mode
	CFLAGS=$(WFLAGS) -std=c++11 -g -fsanitize=address
	OPTIMIZE=-O0
	OPT_SIZE=

else

	# Release mode
	CFLAGS=-w -std=c++11
	OPTIMIZE=-O3
	OPT_SIZE=-s

endif


# Linking flags
LFLAGS=$(LUNATIX_SHARED_LIB) $(LUNATIX_STATIC_LIB) $(SDL_LFLAGS)


#
# Build
#

all : $(TARGETX_EXE)


$(TARGETX_EXE) : $(MAIN_OBJ) $(OBJS)
	@echo $@" - Linking ..."
ifeq ($(DEBUG),yes)
	@echo $@" - Debug mode"
else
	@echo $@" - Release mode"
endif
	@$(CC) -o $@ $^ $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE) $(LFLAGS) && \
	echo $@" - Build finished with success"


#
# Object files
#

# The main object

main.o : $(MAIN_SRC)
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

TargetXplosion.o : $(TARGETX_TX_PATH)TargetXplosion.cpp $(TARGETX_TX_PATH)TargetXplosion.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

# Files in ./src/game/

Background.o : $(TARGETX_GAME_PATH)Background.cpp $(TARGETX_GAME_PATH)Background.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Scoring.o : $(TARGETX_GAME_PATH)Scoring.cpp $(TARGETX_GAME_PATH)Scoring.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Hud.o :	$(TARGETX_ENGINE_PATH)Hud.cpp $(TARGETX_ENGINE_PATH)Hud.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Engine.o : $(TARGETX_ENGINE_PATH)Engine.cpp $(TARGETX_ENGINE_PATH)Engine.hpp \
$(TARGETX_ENTITY_PATH)BasicEnemy.hpp $(TARGETX_ENTITY_PATH)Item.hpp \
$(TARGETX_ENTITY_PATH)Player.hpp $(TARGETX_ENTITY_PATH)Enemy.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp $(TARGETX_GAME_PATH)Background.hpp \
$(TARGETX_ENGINE_PATH)PlayerInput.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

EntityHandler.o : $(TARGETX_ENGINE_PATH)EntityHandler.cpp $(TARGETX_ENGINE_PATH)EntityHandler.hpp \
$(TARGETX_ENTITY_PATH)Enemy.hpp $(TARGETX_ENTITY_PATH)Item.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

AudioHandler.o: $(TARGETX_ENGINE_PATH)AudioHandler.cpp $(TARGETX_ENGINE_PATH)AudioHandler.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

PlayerInput.o : $(TARGETX_ENGINE_PATH)PlayerInput.cpp $(TARGETX_ENGINE_PATH)PlayerInput.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Result.o : $(TARGETX_GAME_PATH)Result.cpp $(TARGETX_GAME_PATH)Result.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Framerate.o : $(TARGETX_ENGINE_PATH)Framerate.cpp $(TARGETX_ENGINE_PATH)Framerate.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

# Files in ./src/entities/

Heaviside.o : $(TARGETX_ENTITY_PATH)Heaviside.cpp $(TARGETX_ENTITY_PATH)Heaviside.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Item.o : $(TARGETX_ENTITY_PATH)Item.cpp $(TARGETX_ENTITY_PATH)Item.hpp \
$(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Entity.o : $(TARGETX_ENTITY_PATH)Entity.cpp $(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Character.o : $(TARGETX_ENTITY_PATH)Character.cpp \
$(TARGETX_ENTITY_PATH)Character.hpp $(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Player.o : $(TARGETX_ENTITY_PATH)Player.cpp $(TARGETX_ENTITY_PATH)Player.hpp \
$(TARGETX_ENTITY_PATH)Character.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Enemy.o : $(TARGETX_ENTITY_PATH)Enemy.cpp $(TARGETX_ENTITY_PATH)Enemy.hpp \
	$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Strategy.o : $(TARGETX_PATTERN_PATH)Strategy.cpp $(TARGETX_PATTERN_PATH)Strategy.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Missile.o : $(TARGETX_ENTITY_PATH)Missile.cpp $(TARGETX_ENTITY_PATH)Missile.hpp \
$(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

TreeMissile.o : $(TARGETX_ENTITY_PATH)TreeMissile.cpp $(TARGETX_ENTITY_PATH)TreeMissile.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Bomb.o : $(TARGETX_ENTITY_PATH)Bomb.cpp $(TARGETX_ENTITY_PATH)Bomb.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

BasicEnemy.o : $(TARGETX_ENTITY_PATH)BasicEnemy.cpp $(TARGETX_ENTITY_PATH)BasicEnemy.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Rocket.o : $(TARGETX_ENTITY_PATH)Rocket.cpp $(TARGETX_ENTITY_PATH)Rocket.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Laser.o : $(TARGETX_ENTITY_PATH)Laser.cpp $(TARGETX_ENTITY_PATH)Laser.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Bullet.o : $(TARGETX_ENTITY_PATH)Bullet.cpp $(TARGETX_ENTITY_PATH)Bullet.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Tower.o : $(TARGETX_ENTITY_PATH)Tower.cpp $(TARGETX_ENTITY_PATH)Tower.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Bachi.o : $(TARGETX_ENTITY_PATH)Bachi.cpp $(TARGETX_ENTITY_PATH)Bachi.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

TargetShooter.o : $(TARGETX_ENTITY_PATH)TargetShooter.cpp $(TARGETX_ENTITY_PATH)TargetShooter.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

PlayerVisitor.o : $(TARGETX_ENTITY_PATH)PlayerVisitor.cpp $(TARGETX_ENTITY_PATH)PlayerVisitor.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

NetShooter.o : $(TARGETX_ENTITY_PATH)NetShooter.cpp $(TARGETX_ENTITY_PATH)NetShooter.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Airship.o : $(TARGETX_ENTITY_PATH)Airship.cpp $(TARGETX_ENTITY_PATH)Airship.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Kamikaze.o : $(TARGETX_ENTITY_PATH)Kamikaze.cpp $(TARGETX_ENTITY_PATH)Kamikaze.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)


# Files in ./src/pattern/

BulletPattern.o : $(TARGETX_PATTERN_PATH)BulletPattern.cpp \
$(TARGETX_PATTERN_PATH)BulletPattern.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)


# Files in ./src/level/

Level.o : $(TARGETX_LEVEL_PATH)Level.cpp $(TARGETX_LEVEL_PATH)Level.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

LoadingScreen.o : $(TARGETX_LEVEL_PATH)LoadingScreen.cpp $(TARGETX_LEVEL_PATH)LoadingScreen.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)


# Files in ./src/entities/boss/

Boss.o : $(TARGETX_BOSS_PATH)Boss.cpp $(TARGETX_BOSS_PATH)Boss.tpp $(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

SemiBoss01.o : $(TARGETX_BOSS_PATH)SemiBoss01.cpp $(TARGETX_BOSS_PATH)SemiBoss01.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

SemiBoss02.o : $(TARGETX_BOSS_PATH)SemiBoss02.cpp $(TARGETX_BOSS_PATH)SemiBoss02.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

SemiBoss03.o : $(TARGETX_BOSS_PATH)SemiBoss03.cpp $(TARGETX_BOSS_PATH)SemiBoss03.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Boss01.o : $(TARGETX_BOSS_PATH)Boss01.cpp $(TARGETX_BOSS_PATH)Boss01.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Boss02.o : $(TARGETX_BOSS_PATH)Boss02.cpp $(TARGETX_BOSS_PATH)Boss02.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp $(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Boss03.o : $(TARGETX_BOSS_PATH)Boss03.cpp $(TARGETX_BOSS_PATH)Boss03.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

Boss04.o : $(TARGETX_BOSS_PATH)Boss04.cpp $(TARGETX_BOSS_PATH)Boss04.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

# Files in ./src/asset/

TX_Asset.o : $(TARGETX_XML_PATH)TX_Asset.cpp $(TARGETX_XML_PATH)TX_Asset.tpp \
	$(TARGETX_XML_PATH)TX_Asset.hpp $(TARGETX_UTIL_PATH)tinyxml2.h
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)


# Files in ./src/resources/

EnemyResourceManager.o : $(TARGETX_RC_PATH)EnemyResourceManager.cpp \
$(TARGETX_RC_PATH)EnemyResourceManager.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

MissileResourceManager.o : $(TARGETX_RC_PATH)MissileResourceManager.cpp \
$(TARGETX_RC_PATH)MissileResourceManager.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

PlayerResourceManager.o : $(TARGETX_RC_PATH)PlayerResourceManager.cpp \
$(TARGETX_RC_PATH)PlayerResourceManager.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

SoundResourceManager.o : $(TARGETX_RC_PATH)SoundResourceManager.cpp \
$(TARGETX_RC_PATH)SoundResourceManager.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

ExplosionResourceManager.o : $(TARGETX_RC_PATH)ExplosionResourceManager.cpp \
$(TARGETX_RC_PATH)ExplosionResourceManager.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

MenuResourceManager.o : $(TARGETX_RC_PATH)MenuResourceManager.cpp \
$(TARGETX_RC_PATH)MenuResourceManager.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

ResourceManager.o : $(TARGETX_RC_PATH)ResourceManager.cpp $(TARGETX_RC_PATH)ResourceManager.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

EnemyLoader.o : $(TARGETX_RC_PATH)EnemyLoader.cpp $(TARGETX_RC_PATH)EnemyLoader.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

WinID.o : $(TARGETX_RC_PATH)WinID.cpp $(TARGETX_RC_PATH)WinID.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)


# Files in ./src/ui/

Menu.o : $(TARGETX_UI_PATH)Menu.cpp $(TARGETX_UI_PATH)Menu.hpp $(TARGETX_UI_PATH)GUI.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

GUI.o : $(TARGETX_UI_PATH)GUI.cpp $(TARGETX_UI_PATH)GUI.hpp $(TARGETX_UI_PATH)GUI.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)


# Files in ./src/option/

OptionHandler.o : $(TARGETX_OPT_PATH)OptionHandler.cpp $(TARGETX_OPT_PATH)OptionHandler.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

GamepadControl.o : $(TARGETX_OPT_PATH)GamepadControl.cpp $(TARGETX_OPT_PATH)GamepadControl.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)


# Files in ./src/option/

tinyxml2.o : $(TARGETX_UTIL_PATH)tinyxml2.cpp $(TARGETX_UTIL_PATH)tinyxml2.h
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(SDL2_I_PATH) -I $(LUNATIX_I_PATH) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE)

#
# Clean
#

clean :
	@echo "Delete object file "
	@rm -f *.o

mrproper : clean
	@echo "Delete target"
	@rm -f $(TARGETX_EXE)
