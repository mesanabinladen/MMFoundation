# ============================================================
# Makefile - MMFoundation
# ============================================================

CC       = gcc
CFLAGS   = -Wall -Wextra -O2 -std=c11
TARGET   = mmfoundation
SRC      = *.c   # adatta i nomi dei tuoi file

# Rileva il sistema operativo
UNAME_S := $(shell uname -s)

# ------------------------------------------------------------
# macOS / Linux → uses libcurl
# ------------------------------------------------------------
ifneq ($(OS),Windows_NT)
    ifeq ($(UNAME_S),Darwin)
        # macOS
        CFLAGS  += -I/usr/local/include -I/opt/homebrew/include
        LDFLAGS += -L/usr/local/lib -L/opt/homebrew/lib -lcurl
    else
        # Linux
        LDFLAGS += -lcurl
    endif
endif

# ------------------------------------------------------------
# Windows (MinGW) → usa WinHTTP (nessuna libcurl)
# ------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lwinhttp
    # Se usi MSVC invece di MinGW, questo Makefile non serve
endif

# ============================================================
# Regole
# ============================================================

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET).exe