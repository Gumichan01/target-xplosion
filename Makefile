
#
#	Target Xplosion - A classic shoot'em up video game
#	Copyright (C) 2015  Luxon Jean-Pierre
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
#	website : gumichan01.olympe.in
#	mail : luxon.jean.pierre@gmail.com
#

# Makefile - Target Xplosion


# You can modify the value of DEBUG
# If you want to use debug or release mode
DEBUG=yes


CC=clang
DEBUG_OBJ=TX_Debug.o
MAIN_OBJ=main.o
OBJS=Background.o \
Character.o \
Item.o \
Game.o \
hud.o \
Entity.o \
Enemy.o \
EnemyData.o \
Player.o \
scoring.o \
Strategy.o \
Missile.o \
Bomb.o \
BasicEnemy.o \
Bachi.o \
Shooter.o \
Rocket.o \
Laser.o \
Level.o \
Boss.o \
SemiBoss01.o \
Boss01.o \
XMLReader.o \
Result.o \
Bullet.o \
BulletPattern.o \
Tower.o \
Rank.o \
EnemyResourceManager.o \
MissileResourceManager.o \
PlayerResourceManager.o \
ResourceManager.o

# Path to main file directory
MAIN_PATH=./src/

# Executable file
TARGETX_EXE=Target-Xplosion-v0.5-dev

# Path to directory and include directory
TARGETX_ENTITY_PATH=./src/entities/
TARGETX_GAME_PATH=./src/game/
TARGETX_LEVEL_PATH=./src/level/
TARGETX_PATTERN_PATH=./src/pattern/
TARGETX_BOSS_PATH=$(TARGETX_ENTITY_PATH)boss/
TARGETX_XML_PATH=./src/xml/
TARGETX_RC_PATH=./src/resources/
TARGETX_INCLUDE_LIB=./include/

# Debug information
TARGETX_DEBUG_PATH=./src/debug/
TARGETX_DEBUG_FILE=$(TARGETX_DEBUG_PATH)TX_Debug.cpp


# Lua compiling
LUAC=luac5.1
SCRIPT_FILE=script/LX_config.lua
COMPILED_SCRIPT=$(SCRIPT_FILE)c

# Libraries
LUNATIX_STATIC_LIB=./lib/linux/libLunatix.a
LUNATIX_SHARED_LIB=./lib/linux/libLunatix.so
TINYXML2_LIB=./lib/linux/libtinyxml2.a
LUA_LIB=./lib/linux/liblua5.1-c++.so.0

# Warning flags
WFLAGS=-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic \
-Wno-documentation -Wno-implicit-fallthrough -Wno-padded -Wno-switch-enum


# Select flags according to the compilation mode
ifeq ($(DEBUG),yes)

	# Debug mode
	CFLAGS=$(WFLAGS) -std=c++11 -g
	OPTIMIZE=
	OPT_SIZE=

else

	# Release mode
	CFLAGS=-w -std=c++11
	OPTIMIZE=-O3
	OPT_SIZE=-s

endif


# Linking flags
LUA_FLAGS=$(LUA_LIB)
LFLAGS=$(TINYXML2_LIB) $(LUNATIX_SHARED_LIB) $(LUNATIX_STATIC_LIB) \
-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer


#
# Build
#

all : $(TARGETX_EXE) $(COMPILED_SCRIPT)


$(COMPILED_SCRIPT) : $(SCRIPT_FILE)
	@echo "Compilation of the Lua script : "$<" -> "$@
	@$(LUAC) -o $@ $< && echo "Compilation done with success"

$(TARGETX_EXE) : $(MAIN_OBJ) $(OBJS)
	@echo $@" - Linking "
ifeq ($(DEBUG),yes)
	@$(CC) -c -o $(DEBUG_OBJ) $(TARGETX_DEBUG_FILE) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS) && \
	$(CC) -o $@ $^ $(DEBUG_OBJ) $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE) $(LFLAGS) $(LUA_FLAGS) && \
	echo $@" - Build finished with success"
	@echo $@" - Debug mode"
else
	@$(CC) -o $@ $^ $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE) $(LFLAGS) $(LUA_FLAGS) && \
	echo $@" - Build finished with success"
	@echo $@" - Release mode"
endif


#
# Object files
#


# The main object

main.o : $(MAIN_PATH)main.cpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)



# Files in ./src/game/

Background.o : $(TARGETX_GAME_PATH)Background.cpp $(TARGETX_GAME_PATH)Background.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


scoring.o : $(TARGETX_GAME_PATH)scoring.cpp $(TARGETX_GAME_PATH)scoring.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


hud.o :	$(TARGETX_GAME_PATH)hud.cpp $(TARGETX_GAME_PATH)hud.hpp \
$(TARGETX_ENTITY_PATH)Player.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Game.o : $(TARGETX_GAME_PATH)Game.cpp $(TARGETX_GAME_PATH)Game.hpp \
$(TARGETX_ENTITY_PATH)BasicEnemy.hpp $(TARGETX_ENTITY_PATH)Item.hpp \
$(TARGETX_ENTITY_PATH)Player.hpp $(TARGETX_ENTITY_PATH)Enemy.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp $(TARGETX_GAME_PATH)Background.hpp \
$(TARGETX_GAME_PATH)Rank.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Result.o : $(TARGETX_GAME_PATH)Result.cpp $(TARGETX_GAME_PATH)Result.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Rank.o : $(TARGETX_GAME_PATH)Rank.cpp $(TARGETX_GAME_PATH)Rank.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

# Files in ./src/entities/

Item.o : $(TARGETX_ENTITY_PATH)Item.cpp $(TARGETX_ENTITY_PATH)Item.hpp \
$(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Entity.o : $(TARGETX_ENTITY_PATH)Entity.cpp $(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Character.o : $(TARGETX_ENTITY_PATH)Character.cpp $(TARGETX_ENTITY_PATH)Character.hpp \
$(TARGETX_ENTITY_PATH)Entity.hpp $(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Player.o : $(TARGETX_ENTITY_PATH)Player.cpp $(TARGETX_ENTITY_PATH)Player.hpp \
$(TARGETX_ENTITY_PATH)Character.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Enemy.o : $(TARGETX_ENTITY_PATH)Enemy.cpp $(TARGETX_ENTITY_PATH)Enemy.hpp \
$(TARGETX_ENTITY_PATH)Player.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Strategy.o : $(TARGETX_PATTERN_PATH)Strategy.cpp $(TARGETX_PATTERN_PATH)Strategy.hpp \
$(TARGETX_ENTITY_PATH)Enemy.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Missile.o : $(TARGETX_ENTITY_PATH)Missile.cpp $(TARGETX_ENTITY_PATH)Missile.hpp \
$(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Bomb.o : $(TARGETX_ENTITY_PATH)Bomb.cpp $(TARGETX_ENTITY_PATH)Bomb.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


BasicEnemy.o : $(TARGETX_ENTITY_PATH)BasicEnemy.cpp \
$(TARGETX_ENTITY_PATH)BasicEnemy.hpp $(TARGETX_ENTITY_PATH)Rocket.hpp \
$(TARGETX_ENTITY_PATH)Laser.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Rocket.o : $(TARGETX_ENTITY_PATH)Rocket.cpp $(TARGETX_ENTITY_PATH)Rocket.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Laser.o : $(TARGETX_ENTITY_PATH)Laser.cpp $(TARGETX_ENTITY_PATH)Laser.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Bullet.o : $(TARGETX_ENTITY_PATH)Bullet.cpp $(TARGETX_ENTITY_PATH)Bullet.hpp \
$(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Tower.o : $(TARGETX_ENTITY_PATH)Tower.cpp $(TARGETX_ENTITY_PATH)Tower.hpp \
$(TARGETX_ENTITY_PATH)Enemy.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

Bachi.o : $(TARGETX_ENTITY_PATH)Bachi.cpp $(TARGETX_ENTITY_PATH)Bachi.hpp \
$(TARGETX_ENTITY_PATH)Enemy.hpp $(TARGETX_ENTITY_PATH)Bullet.hpp \
$(TARGETX_GAME_PATH)Game.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

Shooter.o : $(TARGETX_ENTITY_PATH)Shooter.cpp $(TARGETX_ENTITY_PATH)Shooter.hpp \
$(TARGETX_ENTITY_PATH)Player.hpp $(TARGETX_ENTITY_PATH)Bullet.hpp \
$(TARGETX_GAME_PATH)Game.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


# Files in ./src/pattern/

BulletPattern.o : $(TARGETX_PATTERN_PATH)BulletPattern.cpp \
$(TARGETX_PATTERN_PATH)BulletPattern.hpp $(TARGETX_ENTITY_PATH)Player.hpp \
$(TARGETX_PATTERN_PATH)Angle.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)



# Files in ./src/level/

Level.o : $(TARGETX_LEVEL_PATH)Level.cpp $(TARGETX_LEVEL_PATH)Level.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

EnemyData.o : $(TARGETX_LEVEL_PATH)EnemyData.cpp \
$(TARGETX_LEVEL_PATH)EnemyData.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


# Files in ./src/entities/boss/

Boss.o : $(TARGETX_BOSS_PATH)Boss.cpp $(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

SemiBoss01.o : $(TARGETX_BOSS_PATH)SemiBoss01.cpp $(TARGETX_BOSS_PATH)SemiBoss01.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

Boss01.o : $(TARGETX_BOSS_PATH)Boss01.cpp $(TARGETX_BOSS_PATH)Boss01.hpp \
	$(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


# Files in ./src/xml/

XMLReader.o : $(TARGETX_XML_PATH)XMLReader.cpp $(TARGETX_XML_PATH)XMLReader.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


# Files in ./src/resources/

EnemyResourceManager.o : $(TARGETX_RC_PATH)EnemyResourceManager.cpp \
$(TARGETX_RC_PATH)EnemyResourceManager.hpp $(TARGETX_XML_PATH)XMLReader.hpp
	echo $@" - Compiling "$<
	$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

MissileResourceManager.o : $(TARGETX_RC_PATH)MissileResourceManager.cpp \
$(TARGETX_RC_PATH)MissileResourceManager.hpp $(TARGETX_XML_PATH)XMLReader.hpp
	echo $@" - Compiling "$<
	$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

PlayerResourceManager.o : $(TARGETX_RC_PATH)PlayerResourceManager.cpp \
$(TARGETX_RC_PATH)PlayerResourceManager.hpp $(TARGETX_XML_PATH)XMLReader.hpp
	echo $@" - Compiling "$<
	$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


ResourceManager.o : $(TARGETX_RC_PATH)ResourceManager.cpp \
$(TARGETX_RC_PATH)ResourceManager.hpp \
$(TARGETX_RC_PATH)MissileResourceManager.hpp \
$(TARGETX_RC_PATH)EnemyResourceManager.hpp \
$(TARGETX_XML_PATH)XMLReader.hpp
	echo $@" - Compiling "$<
	$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


#
# Clean
#

clean :
	@echo "Delete object file "
	@rm -f *.o

cleanall : clean
	@echo "Delete target"
	@rm -f $(TARGETX_EXE) $(COMPILED_SCRIPT)

