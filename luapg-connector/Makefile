LUA_CFLAGS = $(shell pkg-config --cflags lua5.4)
LUA_LDFLAGS = $(shell pkg-config --libs lua5.4)
# PG_CFLAGS = $(shell pg_config --includedir)
PG_CFLAGS = -I/usr/include/postgresql
PG_LDFLAGS = $(shell pg_config --libdir)

all:
	gcc -O2 -shared -o postgres.so -fPIC connector.c $(LUA_CFLAGS) $(PG_CFLAGS) -lpq $(LUA_LDFLAGS)
