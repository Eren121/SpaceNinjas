Time = {}
Time.seconds = function(amount)
    return amount * 1000
end
Time.milliseconds = function(amount)
    return amount
end

M_PI = math.pi
M_2PI = M_PI * 2

-- Returns a random double between -1 and 1 (signed random)
function random_s()
    return 2 * math.random() - 1
end

-- Returns a random double between min and max
function random(min, max)
    return min + (max - min) * math.random()
end

-- Return a random number between [-pi;-pi]
function random_angle()
    return random_s() * M_PI
end

-- Wait an amount of milliseconds.
-- If no argument, wait until next frame
-- More generally, all durations are in milliseconds.
function wait(duration)
    if duration == nil then
        coroutine.yield()
    else
        coroutine.yield(api:wait(duration))
    end
end

-- Send a message to the player, wait until it finishes
function message(msg, ms)
    if ms == nil then ms = 250 end

    coroutine.yield(api:message(msg, ms))
end

-- Wait until all enemies are killed, and if so, win the stage.
function stage_end()
    while api:enemyCount() > 0 do
        wait(Time.milliseconds(250))
    end

    api:win()
end

-- Get a position in the random circle just outside the player view.
function get_circle_pos(angle)
    local dx = api.bounds.size.x / 2
    local dy = api.bounds.size.y / 2
    local hypotenuse = math.sqrt(dx ^ 2 + dy ^ 2)
    local margin = 2
    local radius = hypotenuse + margin
    local x = math.cos(angle) * radius
    local y = math.sin(angle) * radius

    return vec2.new(x, y)
end