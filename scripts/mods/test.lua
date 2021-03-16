print ("Hello from a mod")

local turn_count = 0
local tick_count = 0

function next_tick()
    tick_count = tick_count + 1
end

function next_turn()
    turn_count = turn_count + 1
    print("It's turn " .. turn_count .. ", there have been " .. tick_count .. " ticks")
end

function stringify_location(l)
    if l:is_valid() == true then
        return "(" .. l.area .. "/".. l.x .. "," .. l.y .. "," .. l.z .. ")"
    end
    return "(Invalid Location)"
end

function on_add_entity_event(event)
    local loc = stringify_location(event.location)
    print("Added entity: " .. event.entity .. " at ".. loc)
end