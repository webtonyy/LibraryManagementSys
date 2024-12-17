# Variáveis
CC = gcc
CFLAGS = -I./include -g -Wall -Werror
BUILD = ./build
SOURCE = ./src
MAIN = ./main
EXEC = biblioteca

# Busca automática de todos os arquivos .c no diretório SOURCE
SRC_FILES := $(wildcard $(SOURCE)/*.c)
OBJ_FILES := $(patsubst $(SOURCE)/%.c, $(BUILD)/%.o, $(SRC_FILES))
MAIN_FILE := $(MAIN)/main.c

# Regra padrão para compilar o executável
all: $(EXEC)

# Regra para criar o executável a partir dos objetos
$(EXEC): $(OBJ_FILES)
	$(CC) $(MAIN_FILE) $^ $(CFLAGS) -o $@

# Regra para compilar arquivos objeto a partir dos fontes
$(BUILD)/%.o: $(SOURCE)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

# Limpeza dos arquivos gerados
clean:
	rm -f $(BUILD)/*.o $(EXEC)

# Executar o programa
run: all
	./$(EXEC)

.PHONY: all clean run
