# MAKEFILE GENERICO PARA USOS MULTIPLES
# 2019 - germansc
#

# INFORMACIÓN DEL PROTECTO #
PROJECT_NAME = "2022 - Advent of Code"
COMPANY = "GSC Industries"
AUTHOR = "germansc"
YEAR = "2022"

# TOOLCHAIN  #
CXX = gcc
SIZE = size

# NOMBRE DEL EJECUTABLE #
BIN_NAME = $(shell basename $$(dirname $(CURDIR)))

# EXTENSIONES - LENGUAJE #
SRC_EXT = c

# OPCIONES Y LIBRERÍAS UTILIZADAS #
CFLAGS = -Wall -Wextra
LIBS =

release: CFLAGS +=-O2
debug: CFLAGS +=-O0 -g3

part1: CFLAGS += -DPART1

# PATHS #
SRC_PATH = ../src
OBJ_PATH = ./objects

# Listo todos los archivos fuente del directorio de SRC_PATH y los subdirectorios.
SRC_LIST = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)')
SRC_DIRS = $(shell find $(SRC_PATH) -type d)

# Genero el nombre de los archivos objeto a generar a partir de los sources.
OBJECTS = $(SRC_LIST:$(SRC_PATH)/%.$(SRC_EXT)=$(OBJ_PATH)/%.o)

# Genero dependencias para recompilar en modificaciones de headers.
DEPENDS = $(patsubst %.o, %.d, $(OBJECTS))

# INCLUDES - Agrego todos los subdirectorios de SRC_PATH #
INCLUDES = $(SRC_DIRS:%=-I %)

# DEFAULT TARGET #
.PHONY: default_target
default_target: all

# TARGETS AUXILIARES #
.PHONY: help
help:
	@echo
	@echo $(PROJECT_NAME)
	@echo "----------------------------------------------"
	@echo "$(COMPANY)"
	@echo "$(AUTHOR) · $(YEAR)"
	@echo ""
	@echo ""
	@echo "TARGETS DE COMPILACIÓN:"
	@echo "    debug      : Versión sin optimizaciones y con información de símbolos."
	@echo "    release    : Versión con optimizaciones -O2 y sin información de símbolos."
	@echo ""
	@echo "TARGETS AUXILIARES:"
	@echo "    clean      : Elimina todos los archivos objeto generados"
	@echo "    help       : Esta ayuda."
	@echo ""

.PHONY: dirs
dirs:
	@echo "Creating directories..."
	@mkdir -p $(dir $(OBJECTS))

.PHONY: clean
clean:
	@echo "Deleting executables and directories..."
	@echo ""
	@$(RM) -r $(OBJ_PATH) $(BIN_NAME)

# TARGETS PRINCIPALES #
.PHONY: all
all: release

.PHONY: release
release: dirs $(BIN_NAME)

.PHONY: debug
debug: dirs $(BIN_NAME)

.PHONY: part1
part1: dirs $(BIN_NAME)

# EJECUTABLE PRINCIPAL #
$(BIN_NAME): $(OBJECTS)
	@echo ""
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) $(LIBS) -o $@
	@echo ""
	@echo "Build complete -> $(BIN_NAME)"
	@echo "------------------------------------------------"
	$(SIZE) $(BIN_NAME)
	@echo ""

# --------------------------------------------------------------------------- #
# Reglas generales para compilacion de objetos.
-include $(DEPENDS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo ""
	@echo "Compiling: $< -> $@"
	$(CXX) $(CFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
