local postgres = require("postgres")

-- Connect to PostgreSQL
local conn, err = postgres.connect("host=host dbname=db user=user password=password")
if not conn then
    error("Connection failed: " .. err)
end

-- Execute a query
local result, err = conn:exec("SELECT * FROM table;")
if not result then
    error("Query failed: " .. err)
end

-- Print results
for i, row in ipairs(result) do
    for col, val in pairs(row) do
        print(col .. ": " .. val)
    end
end

-- Close the connection
conn:close()
