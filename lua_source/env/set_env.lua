
package.path = "../lua_source/?.lua"

Game = require("libgame")
-- package.cpath = ""
if Game then
    print("-------------->")
    print(Game)
    Game.sleep(2000)
    print("<--------------")
end


require("env/reload")
