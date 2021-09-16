function stage1()
    -- spawn ennemies from top
    local y = 1.2 -- y location to spawn ennemies
    for i = 1, 20 do
        local x = math.random(-100, 100) / 100
        api:spawnEnnemy(x, y, 0, -10)
        coroutine.yield(api:wait(250))
    end

    while api:ennemyCount() > 0 do
        coroutine.yield(api:wait(500))
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

    coroutine.yield(api:wait(3000))

    for i = 1, k do
        local x = 1 - 2 * i / k
        api:spawnEnnemy(x, -1, 0, vel)
        api:spawnEnnemy(x, 1, 0, -vel)
    end

    while api:ennemyCount() > 0 do
        coroutine.yield(api:wait(500))
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
        coroutine.yield(api:wait(500))
    end

    api:win()
end

function main(id)

    if(type(id) ~= "number") then
        error("id type is not int")
    end

    print("Starting level " .. id)

    -- id starts at zero in code, but one in scripts
    id = id + 1

    local stage = _ENV["stage" .. id]

    if(type(stage) == "nil") then
        api:win()
    else
        stage()
    end
end