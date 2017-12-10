CC=musl-gcc
CFLAGS+=-m32
LDFLAGS+=`sdl2-config --cflags --libs` -m32
.POSIX:
