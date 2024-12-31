## About

**Lua-pgsql** is a simple sql connector to postgres database.

## How to use

Replace the string argument in `postgres.connect` located at `test.lua` with your database parameters. If you database has the following parameters:

```
Host: localhost
dbname: database
user: admin
password: passwd
```

then replace to the following:

```
postgres.connect("host=localhost dbname=database user=admin password=passwd")
```

Run `test.lua` with any valid query in you database to retrieve the result as

```
lua test.lua
```

### Version

Current version of the project is `1.0`.