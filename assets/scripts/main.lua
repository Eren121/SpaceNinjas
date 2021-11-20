function main(stage_id)

    package.path = "../assets/scripts/?.lua"

    require("common_functions")

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
        print("No module associated to the stage (searched module \"" .. module_name .. "\")")
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