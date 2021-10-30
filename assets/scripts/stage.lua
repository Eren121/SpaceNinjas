function stage1()
    -- spawn ennemies from top

    local y = 1.2 -- y location to spawn ennemies
    for i = 1, 20 do
        local x = math.random(-100, 100) / 100
        api:spawnEnnemy(x, y, 0, -10)
        wait(250)
    end

    while api:ennemyCount() > 0 do
        wait(500)
    end

    api:win()
end

function stage2()
    -- spawn ennemy on left and right
    local k = 20 -- count of ennemies to spawn
    local vel = 10 -- velocity of ennemy
    for i = 1, k do
        local y = 1 - 2 * i / k -- [-1; 1]
        api:spawnEnnemy(-1, y, vel, 0)
        api:spawnEnnemy(1, y, -vel, 0)
    end

    wait(3000)

    for i = 1, k do
        local x = 1 - 2 * i / k
        api:spawnEnnemy(x, -1, 0, vel)
        api:spawnEnnemy(x, 1, 0, -vel)
    end

    while api:ennemyCount() > 0 do
        wait(500)
    end

    api:win()
end

function stage3()
    local k = 100 -- count of ennemies to spawn
    local vel = 10
    local radius = math.sqrt(2)

    for i = 1, k do
        local angle = math.pi * 2 / k * i
        local x = math.cos(angle) * radius
        local y = math.sin(angle) * radius
        api:spawnEnnemy(x, y, -x * vel, -y * vel)
    end

    while api:ennemyCount() > 0 do
        wait(500)
    end

    api:win()
end


-- Wait an amount of milliseconds
function wait(ms)
    coroutine.yield(api:wait(ms))
end

-- Send a message to the player, wait until it finishes
function message(msg, ms)
    if ms == nil then ms = 250 end

    coroutine.yield(api:message(msg, ms))
end

function main(stage_id)

    package.path = "../assets/scripts/?.lua"

    -- Argument checking: void main(int stage_id)
    if(type(stage_id) ~= "number") then
        error("id type is not int")
    end

    -- Include the module if there is one
    -- pcall() to catch the error if require fails
    local module_name = "stage" .. stage_id
    local status, stage_module = pcall(require, module_name)
    if(not status) then
        -- then second return value is error_msg
        error_msg = stage_module
        print(error_msg)
        print("No module associated to the stage (searched module '" .. module_name .. "')")
    end

    -- Get the function stageXX where XX is the stage_id to run it as the main coroutine
    local stage_coro = _ENV["stage" .. stage_id]

    -- If the function does not exist, warn it and stops directly the level.
    if(type(stage_coro) == "nil") then
        print("Warning : cannot find stage with ID " .. stage_id)
        api:win()
        return
    end

    print("Starting level " .. stage_id)

    -- Relay to the stage procedure

    stage_coro()
end