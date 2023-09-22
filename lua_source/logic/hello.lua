
local Hello = {}
function Hello:say_helllo()
    print("hello lua", debug.traceback())
end

function Hello:OnReload()
    self:say_helllo()
end

return Hello
