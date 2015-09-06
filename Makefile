
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


CC=gcc
MAIN_OBJ=main.o
OBJS=Background.o \
Character.o \
Item.o \
Game.o \
hud.o \
Entity.o \
Enemy.o \
Player.o \
scoring.o \
Strategy.o \
Missile.o \
Bomb.o \
BasicEnemy.o \
Bachi.o \
Rocket.o \
Laser.o \
Level.o \
Boss.o \
Boss00.o \
Boss01.o \
XMLReader.o \
Result.o \
Bullet.o \
Tower.o \
Rank.o

# Path to main file directory
MAIN_PATH=./src/

# Executable file
TARGETX_EXE=Target-Xplosion-v0.4-alpha

# Path to directory and include directory
TARGETX_ENTITY_PATH=./src/entities/
TARGETX_GAME_PATH=./src/game/
TARGETX_LEVEL_PATH=./src/level/
TARGETX_BOSS_PATH=$(TARGETX_ENTITY_PATH)boss/
TARGETX_XML_PATH=./src/xml/
TARGETX_INCLUDE_LIB=./include/

# Lua compiling
LUAC=luac5.1
SCRIPT_FILE=script/LX_config.lua
COMPILED_SCRIPT=$(SCRIPT_FILE)c

# Libraries
LUNATIX_STATIC_LIB=./lib/linux/libLunatix.a
LUNATIX_SHARED_LIB=./lib/linux/libLunatix.so
TINYXML2_LIB=./lib/linux/libtinyxml2.a
LUA_LIB=./lib/linux/liblua5.1-c++.so.0

# Select flags according to the compilation mode
ifeq ($(DEBUG),yes)

	# Debug mode
	CFLAGS=-Wall -std=c++0x -g
	OPTIMIZE=
	OPT_SIZE=

else

	# Release mode
	CFLAGS=-w -std=c++0x
	OPTIMIZE=-O3
	OPT_SIZE=-s

endif


# Linking flags
LUA_FLAGS=$(LUA_LIB)
LFLAGS=$(TINYXML2_LIB) $(LUNATIX_SHARED_LIB) $(LUNATIX_STATIC_LIB) \
-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer


all : $(TARGETX_EXE) $(COMPILED_SCRIPT)


$(COMPILED_SCRIPT) : $(SCRIPT_FILE)
	@echo "Compilation of the Lua script : "$<" -> "$@
	@$(LUAC) -o $@ $<


$(TARGETX_EXE) : $(MAIN_OBJ) $(OBJS)
ifeq ($(DEBUG),yes)
	@echo "Debug mode"
else
	@echo "Release mode"
endif
	@echo $@" - Linking "
	@$(CC) -o $@ $^ $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE) $(LFLAGS) $(LUA_FLAGS)



# The main object

main.o : $(MAIN_PATH)main.cpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)



# Files in ./src/game/

Background.o : $(TARGETX_GAME_PATH)Background.cpp $(TARGETX_GAME_PATH)Background.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


scoring.o : $(TARGETX_GAME_PATH)scoring.cpp $(TARGETX_GAME_PATH)scoring.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


hud.o :	$(TARGETX_GAME_PATH)hud.cpp $(TARGETX_GAME_PATH)hud.hpp \
$(TARGETX_GAME_PATH)Observer.hpp $(TARGETX_ENTITY_PATH)Player.hpp
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


Strategy.o : $(TARGETX_ENTITY_PATH)Strategy.cpp $(TARGETX_ENTITY_PATH)Strategy.hpp \
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

# Files in ./src/level/

Level.o : $(TARGETX_LEVEL_PATH)Level.cpp $(TARGETX_LEVEL_PATH)Level.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


# Files in ./src/entities/boss/

Boss.o : $(TARGETX_BOSS_PATH)Boss.cpp $(TARGETX_BOSS_PATH)Boss.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

Boss00.o : $(TARGETX_BOSS_PATH)Boss00.cpp $(TARGETX_BOSS_PATH)Boss00.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)

Boss01.o : $(TARGETX_BOSS_PATH)Boss01.cpp $(TARGETX_BOSS_PATH)Boss01.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


# Files in ./src/xml/

XMLReader.o : $(TARGETX_XML_PATH)XMLReader.cpp $(TARGETX_XML_PATH)XMLReader.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $<  -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


clean :
	@echo "Delete object file "
	@rm -f *.o

cleanall : clean
	@echo "Delete target"
	@rm -f $(TARGETX_EXE) $(COMPILED_SCRIPT)

