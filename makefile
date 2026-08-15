# ============================================================
# Makefile - MMFoundation
# ============================================================

CC       = gcc
CFLAGS   = -Wall -Wextra -O2 -std=c11
TARGET   = mmfoundation
SRC      = *.c   

# Detect OS
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
        LDFLAGS += -lcurl -D_XOPEN_SOURCE=500
    endif
endif

# ------------------------------------------------------------
# Windows (MinGW) → usese WinHTTP (no libcurl)
# ------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lwinhttp
endif

# ============================================================
# Rules
# ============================================================

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET).exe