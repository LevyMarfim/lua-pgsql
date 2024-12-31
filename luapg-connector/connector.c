#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <libpq-fe.h>

// Function to connect to PostgreSQL
static int l_pg_connect(lua_State *L) {
    const char *conninfo = luaL_checkstring(L, 1); // Get connection string from Lua

    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        lua_pushnil(L);
        lua_pushstring(L, PQerrorMessage(conn));
        PQfinish(conn);
        return 2; // Return nil and error message
    }

    // Push the connection pointer as a userdata
    *(PGconn **)lua_newuserdata(L, sizeof(PGconn *)) = conn;
    luaL_getmetatable(L, "Postgres.Connection");
    lua_setmetatable(L, -2);

    return 1; // Return connection userdata
}

// Function to close the connection
static int l_pg_close(lua_State *L) {
    PGconn **conn = luaL_checkudata(L, 1, "Postgres.Connection");
    if (*conn) {
        PQfinish(*conn);
        *conn = NULL;
    }
    return 0; // No return values
}

// Function to execute a query
static int l_pg_exec(lua_State *L) {
    PGconn **conn = luaL_checkudata(L, 1, "Postgres.Connection");
    const char *query = luaL_checkstring(L, 2);

    PGresult *res = PQexec(*conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        lua_pushnil(L);
        lua_pushstring(L, PQerrorMessage(*conn));
        PQclear(res);
        return 2; // Return nil and error message
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    lua_newtable(L); // Create a Lua table to store results

    for (int i = 0; i < rows; i++) {
        lua_newtable(L); // Create a table for each row
        for (int j = 0; j < cols; j++) {
            lua_pushstring(L, PQgetvalue(res, i, j)); // Push cell value
            lua_setfield(L, -2, PQfname(res, j)); // Set field name
        }
        lua_rawseti(L, -2, i + 1); // Insert row table into result table
    }

    PQclear(res);
    return 1; // Return the result table
}

// Register methods and metatables
static const luaL_Reg postgres_methods[] = {
    {"connect", l_pg_connect},
    {NULL, NULL}
};

static const luaL_Reg connection_methods[] = {
    {"close", l_pg_close},
    {"exec", l_pg_exec},
    {NULL, NULL}
};

// Module entry point
int luaopen_postgres(lua_State *L) {
    luaL_newmetatable(L, "Postgres.Connection");

    lua_pushstring(L, "__index");
    lua_newtable(L);
    luaL_setfuncs(L, connection_methods, 0);
    lua_settable(L, -3);

    luaL_newlib(L, postgres_methods);
    return 1; // Return the module table
}
