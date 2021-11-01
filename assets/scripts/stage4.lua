function stage4()

    api:run(function()
        local timeScale = 1
        local i = 0

        -- Modulate the speed
        while true do
            timeScale = 1 + math.cos(i / 100) * 0.2
            --api.timeScale = timeScale
            i = i + 1

            coroutine.yield()
        end
    end)

    wait(1000)

    -- Spawner des ennemis qui vont vers le joueur en spawnant d'un point aléatoire autoure de lui

    -- Radius of the circle to spawn units
    -- sqrt(2) so always spawn in a hidden position centered on the screen, at closest
    local r = api.bounds.size.x / 2

    -- Length of the velocity of enemies to spawn (defines the speed of enemies)
    local velocity_length = 9

    -- Count of enemies to spawn
    local n = 100

    for i = 1, n do
        local angle = math.random() * 2 * M_PI

        local target = api:playerPos()
        local spawn_x = math.cos(angle) * r
        local spawn_y = math.sin(angle) * r
        local spawn = vec2.new(spawn_x, spawn_y)

        -- Compute direction from origin and target
        local velocity = velocity_length * (target - spawn):normalize()

        local body = api:spawnEnemy(spawn)
        body.velocity = velocity
        wait(100)
    end
end