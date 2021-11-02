function stage2()
    local x = 0
    while true do
        api.player.position.x = math.cos(x * 0.1)
        x = x + 1
        wait()
    end
end