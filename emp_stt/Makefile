# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -g

# Arquivos fonte e cabeçalhos
SRCS = main.c emp_stt.c
HEADERS = emp_stt.h

# Nome do executável
TARGET = teste

# Regra padrão: compilar o programa
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Limpa arquivos gerados pela compilação
clean:
	rm -f $(TARGET)

# Regra para recompilar tudo do zero
rebuild: clean all