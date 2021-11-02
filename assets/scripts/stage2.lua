function stage2()
    local t = tween.new(10, api, {position = vec2.new(10, 0)})
    local start_time = api.time
    
    while true do
        local complete = t:set(api.time - start_time)

        if complete then
            break
        end



        wait()
    end
end