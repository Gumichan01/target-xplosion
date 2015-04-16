
#
#	Copyright (C) 2014-2015 Luxon Jean-Pierre
#	gumichan01.olympe.in
#
#	TARGETX-engine is a SDL-based game engine.
#	It can be used for open-source or commercial games thanks to the zlib/libpng license.
#
#	Luxon Jean-Pierre (Gumichan01)
#	luxon.jean.pierre@gmail.com
#

# Makefile - TARGETX Engine v0.2


# You can modify the value of DEBUG
# If you want to use debug or release mode
DEBUG=no


CC=g++
MAIN_OBJ=main.o
OBJS=LX_Chunk.o \
LX_Config.o \
LX_Graphics.o \
LX_Library.o \
LX_Mixer.o \
LX_Music.o \
LX_Physics.o \
LX_TrueTypeFont.o \
LX_Window.o\
Background.o \
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
Basic_Enemy.o

# Path to main file directory
MAIN_PATH=./src/

# Executable file
TARGETX_EXE=Target-Xplosionv0.2

# Path to directory and include directory
TARGETX_ENTITY_PATH=./src/entities/
TARGETX_ENGINE_PATH=./src/engine/
TARGETX_GAME_PATH=./src/game/
TARGETX_INCLUDE_LIB=./include/


# Libraries
STATIC_LIB_TARGETX=libTARGETX-engine0.2.a

# Select flags according to the compilation mode
ifeq ($(DEBUG),yes)

	# Debug mode
	CFLAGS=-std=c++0x -g
	OPTIMIZE=
	OPT_SIZE=

else

	# Release mode
	CFLAGS=-std=c++0x
	OPTIMIZE=-O3
	OPT_SIZE=-s

endif


# Linking flags
LUA_FLAGS=./lib/linux/liblua5.1-c++.so.0
LFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer


all : $(TARGETX_EXE)




$(TARGETX_EXE) : $(MAIN_OBJ) $(OBJS)
ifeq ($(DEBUG),yes)
	@echo "Debug mode"
else
	@echo "Release mode"
endif
	@echo $@" - Linking "
	@$(CC) -o $@ $^ $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE) $(LFLAGS) $(LUA_FLAGS)




# Fichier main
main.o : $(MAIN_PATH)main.cpp $(TARGETX_ENGINE_PATH)Lunatix_engine.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)



# Files in ./src/engine/
LX_Config.o : $(TARGETX_ENGINE_PATH)LX_Config.cpp $(TARGETX_ENGINE_PATH)LX_Config.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_Window.o : $(TARGETX_ENGINE_PATH)LX_Window.cpp $(TARGETX_ENGINE_PATH)LX_Window.hpp $(TARGETX_ENGINE_PATH)LX_Config.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_Library.o : $(TARGETX_ENGINE_PATH)LX_Library.cpp $(TARGETX_ENGINE_PATH)LX_Library.hpp $(TARGETX_ENGINE_PATH)LX_Config.hpp $(TARGETX_ENGINE_PATH)LX_Mixer.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_Graphics.o : $(TARGETX_ENGINE_PATH)LX_Graphics.cpp $(TARGETX_ENGINE_PATH)LX_Graphics.hpp $(TARGETX_ENGINE_PATH)LX_Window.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_TrueTypeFont.o : $(TARGETX_ENGINE_PATH)LX_TrueTypeFont.cpp $(TARGETX_ENGINE_PATH)LX_TrueTypeFont.hpp $(TARGETX_ENGINE_PATH)LX_Graphics.hpp $(TARGETX_ENGINE_PATH)LX_Config.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_Physics.o : $(TARGETX_ENGINE_PATH)LX_Physics.cpp $(TARGETX_ENGINE_PATH)LX_Physics.hpp $(TARGETX_ENGINE_PATH)LX_Hitbox.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_Chunk.o : $(TARGETX_ENGINE_PATH)LX_Chunk.cpp $(TARGETX_ENGINE_PATH)LX_Chunk.hpp $(TARGETX_ENGINE_PATH)LX_Sound.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_Music.o : $(TARGETX_ENGINE_PATH)LX_Music.cpp $(TARGETX_ENGINE_PATH)LX_Sound.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


LX_Mixer.o : $(TARGETX_ENGINE_PATH)LX_Mixer.cpp $(TARGETX_ENGINE_PATH)LX_Mixer.hpp $(TARGETX_ENGINE_PATH)LX_Sound.hpp $(TARGETX_ENGINE_PATH)LX_Music.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)





# Files in ./src/game/

Background.o : $(TARGETX_GAME_PATH)Background.cpp $(TARGETX_GAME_PATH)Background.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


scoring.o : $(TARGETX_GAME_PATH)scoring.cpp $(TARGETX_GAME_PATH)scoring.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


hud.o :	$(TARGETX_GAME_PATH)hud.cpp $(TARGETX_GAME_PATH)hud.hpp $(TARGETX_GAME_PATH)Observer.hpp $(TARGETX_ENTITY_PATH)Player.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Game.o : $(TARGETX_GAME_PATH)Game.cpp $(TARGETX_GAME_PATH)Game.hpp $(TARGETX_ENTITY_PATH)Basic_Enemy.hpp $(TARGETX_ENTITY_PATH)Item.hpp \
$(TARGETX_ENTITY_PATH)Player.hpp $(TARGETX_ENTITY_PATH)Enemy.hpp $(TARGETX_ENTITY_PATH)Missile.hpp $(TARGETX_GAME_PATH)Background.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)



# Files in ./src/entities/

Item.o : $(TARGETX_ENTITY_PATH)Item.cpp $(TARGETX_ENTITY_PATH)Item.hpp $(TARGETX_ENTITY_PATH)Entity.hpp $(TARGETX_GAME_PATH)random.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Entity.o : $(TARGETX_ENTITY_PATH)Entity.cpp $(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Character.o : $(TARGETX_ENTITY_PATH)Character.cpp $(TARGETX_ENTITY_PATH)Character.hpp $(TARGETX_ENTITY_PATH)Entity.hpp $(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)



Player.o : $(TARGETX_ENTITY_PATH)Player.cpp $(TARGETX_ENTITY_PATH)Player.hpp $(TARGETX_ENTITY_PATH)Character.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Enemy.o : $(TARGETX_ENTITY_PATH)Enemy.cpp $(TARGETX_ENTITY_PATH)Enemy.hpp $(TARGETX_ENTITY_PATH)Player.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Strategy.o : $(TARGETX_ENTITY_PATH)Strategy.cpp $(TARGETX_ENTITY_PATH)Strategy.hpp $(TARGETX_ENTITY_PATH)Enemy.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Missile.o : $(TARGETX_ENTITY_PATH)Missile.cpp $(TARGETX_ENTITY_PATH)Missile.hpp $(TARGETX_ENTITY_PATH)Entity.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Bomb.o : $(TARGETX_ENTITY_PATH)Bomb.cpp $(TARGETX_ENTITY_PATH)Bomb.hpp $(TARGETX_ENTITY_PATH)Missile.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)


Basic_Enemy.o : $(TARGETX_ENTITY_PATH)Basic_Enemy.cpp $(TARGETX_ENTITY_PATH)Basic_Enemy.hpp $(TARGETX_ENTITY_PATH)Rocket.hpp $(TARGETX_ENTITY_PATH)Laser.hpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(TARGETX_ENGINE_PATH) -I $(TARGETX_INCLUDE_LIB) $(CFLAGS)



clean :
	@echo "Delete object file "
	@rm -f *.o

clean-target : clean
	@echo "Delete targets"
	@rm -f $(TARGETX_EXE)












