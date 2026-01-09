CC = gcc
TARGET = vox8

# Aggressive size reduction flags
CFLAGS = -Os -std=c11 -Wall -Wextra \
         -ffunction-sections -fdata-sections \
         -fno-asynchronous-unwind-tables \
         -fno-unwind-tables \
         -fmerge-all-constants \
         -fno-ident \
         -flto

LDFLAGS = -Wl,--gc-sections -Wl,--strip-all \
          -Wl,-z,norelro -Wl,--hash-style=gnu \
          -Wl,--build-id=none -flto

SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs)

# Source directory
SRCDIR = src
OBJDIR = obj

# Auto-discover all .c files in src/
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(SDL_LIBS)
	strip -s -R .comment -R .gnu.version $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(SRCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean