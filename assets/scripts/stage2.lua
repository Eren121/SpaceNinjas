function stage2()
    -- spawn enemy on left and right

    local k = 20 -- count of enemies to spawn
    local vel = 10
    local bounds = api.bounds
    local body

    for i = 0, k do
        local pos = vec2.new()
        pos.y = bounds.origin.y + bounds.size.y * i / k

        pos.x = bounds.left
        body = api:spawnEnemy(pos)
        body.velocity = vec2.new(vel, 0)

        pos.x = bounds.right
        body = api:spawnEnemy(pos)
        body.velocity = vec2.new(-vel, 0)
    end

    wait(3000)

    for i = 0, k do
        local pos = vec2.new()
        pos.x = bounds.origin.x + bounds.size.x * i / k

        pos.y = bounds.bottom
        body = api:spawnEnemy(pos)
        body.velocity = vec2.new(0, vel)

        pos.y = bounds.top
        body = api:spawnEnemy(pos)
        body.velocity = vec2.new(0, -vel)
    end

    while api:enemyCount() > 0 do
        wait(500)
    end

    api:win()
end