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
    local end_time = api.time + ms

    while end_time > api.time do
        coroutine.yield()
    end
end

-- Starts a process concurrently (returns immediately)
function run(f)
    lua_api:run(f)
end

-- Extension of the API in pure Lua
-- You don't have to use its functions, they are wrapped in global functions like run()
lua_api = {}

function registerApiExtensions()
    
    -- List of processes to run at each step
    local ps = {}

    -- api.run(f): Register the function f as a new coroutine each step until its end
    function lua_api:run(f)
        -- Insert f as a new coroutine into ps
        table.insert(ps, coroutine.create(f))
    end
    
    -- The real main, the global function main() is just the "bootloader"
    -- api.main(function stageF)
    -- The parameter stageF is the main coroutine of the stage associated to the ID.
    function lua_api:main(stageF)
        -- Add the main coroutine
        self:run(stageF)

        -- Run until all coroutines are dead
        while true do

            -- SIMULATION STEP

            -- Run all processes once for this step
            for key, coro in pairs(ps) do
                
                coroutine.resume(coro)
                local running = (coroutine.status(coro) ~= "dead")
                
                if not running then
                    -- Coroutine stopped, remove it from the list
                    -- Assigning to nil in Lua effectively remove the value
                    ps[key] = nil
                end
            end
            
            -- Stop the loop if there is no more processes
            local empty = (next(ps) == nil)
            
            if empty then
                break
            end
    
            -- Wait next step if there are still processes
            coroutine.yield()
        end
    end
end



function main(stage_id)

    package.path = "../assets/scripts/?.lua"

    -- Argument checking: void main(int stageID)
    if(type(stage_id) ~= "number") then
        error("id type is not int")
    end

    -- Include the module if there is one
    -- pcall() to catch the error if require fails
    local module_name = "stage" .. stage_id
    local status, stage_module = pcall(require, module_name)
    if(not status) then
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

    -- Register API hooks in Lua
    registerApiExtensions()

    print("Starting level " .. stage_id)

    -- Relay to the API main function
    lua_api:main(stage_coro)
end