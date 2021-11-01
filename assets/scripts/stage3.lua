function stage3()

    -- spawn enemies in circle

    local k = 100 -- count of enemies to spawn
    local vel = 10
    local radius = api.bounds.size.x / 2

    for i = 1, k do
        local angle = M_PI * 2 / k * i
        local x = math.cos(angle) * radius
        local y = math.sin(angle) * radius
        local pos = vec2.new(x, y)

        local body = api:spawnEnemy(pos)
        body.velocity = -pos:normalize() * vel
    end

    while api:enemyCount() > 0 do
        wait(500)
    end

    api:win()
end
