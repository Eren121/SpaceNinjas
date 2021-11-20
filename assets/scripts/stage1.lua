-- Enemies spawn from the top
function phase_rain(arg)
    local duration = arg.duration or Time.seconds(1)
    local velocity = arg.velocity or 10
    local period = arg.period or Time.milliseconds(250)

    local start_time = api.time
    local end_time = start_time + duration

    while api.time < end_time do

        -- Small margin to not ugly spawn fom nowhere
        local margin = 2

        -- Compute position of body
        local pos = vec2.new()
        pos.x = api.bounds.left + api.bounds.size.x * math.random()
        pos.y = api.bounds.top + margin

        -- Spawn body
        -- 10% modulation of the velocity possible

        local body = api:spawnEnemy(pos)
        body.velocity = vec2.new(0, -velocity * (1 + random_s() * 0.1))

        -- Wait before spawn new body (or stop if time is elapsed)
        wait(period)
    end
end


-- Enemies spawn from a random location to the current player location
function phase_follow(arg)
    local duration = arg.duration or Time.seconds(1)
    local velocity = arg.velocity or 10
    local period = arg.period or Time.milliseconds(200)

    local start_time = api.time
    local end_time = start_time + duration

    -- Ennemies can't spawn from the bottom edge, delimited by angles 5pi/4 and 7pi/4
    -- It is to make it more easy, making the bottom acts like a "safe zone" for beginner levels

    local angle_bound = math.atan(api.bounds.top / api.bounds.right)
    local angle_min = -angle_bound
    local angle_max = M_PI + angle_bound

    while api.time < end_time do

        local pos = get_circle_pos(random(angle_min, angle_max))
        local target = api:playerPos()

        local body = api:spawnEnemy(pos)
        body.velocity = vec2.normalize(target - pos) * velocity * (1 + random_s() * 0.1)

        wait(period)
    end
end

-- Spawn enemies at the same time in a circle (do not wait)
function phase_circle(arg)
    local count = arg.count or 10
    local velocity = arg.velocity or 5

    for i = 1, count do
        local angle = i / count * M_2PI
        local pos = get_circle_pos(angle)
        local body = api:spawnEnemy(pos)

        -- Always target center (for targeting the player, use phase_follow())
        local target = vec2.new(0, 0)

        body.velocity = vec2.normalize(target - pos) * velocity
    end
end

function stage1()

    phase_rain{duration=Time.seconds(30), velocity=7, period=400}
    phase_follow{duration=Time.seconds(20), period=Time.milliseconds(750), velocity=6}
    wait(Time.seconds(3))
    phase_circle{count=40, velocity=3}
    wait(Time.seconds(15))

    -- Re-start the pattern but with 1st phase with higer velocity (but shorter duration to be not too hard)
    
    phase_rain{duration=Time.seconds(15), velocity=20}
    phase_follow{duration=Time.seconds(20), period=Time.milliseconds(750), velocity=7}
    wait(Time.seconds(3))
    phase_circle{count=30}
    wait(Time.seconds(15))

    -- Many but low-speed

    phase_rain{duration=Time.seconds(20), velocity=4, period=40}

    stage_end()
end