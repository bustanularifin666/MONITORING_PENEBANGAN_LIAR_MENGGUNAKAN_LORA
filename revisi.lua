--Lua script to save time/ value pair to MySQL server
luasql = require "luasql.mysql"
require "luasql.mysql"
local value=arg[1]
local value2=arg[2]
local value3=arg[3]

local value5=arg[4]
local current_time=os.date("%Y-%m-%d %H:%M:%S")
env = assert (luasql.mysql())
con = assert (env:connect('illegal_logging', 'PA', 'dragino', '10.130.1.226'))
res = assert (con:execute('INSERT INTO sensor_node1(waktu,suara,getar,latitude,longitude) VALUES("'..current_time..'",'..value..','..value2..','..value3..','..value5..')'))