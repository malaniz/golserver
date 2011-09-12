require "golpthread"

function run_experiment(size, gen) 
    local result

    doit(size, size, gen)
    while true do
        result = stats()
        if result ~= nil then break end
    end
    return result
end

function results2gnuplot()
    local last
    for s, c1 in pairs(results) do
        c = 0
        f = io.open(s .. ".dat", "w")
        for g, c2 in pairs(c1) do
            table.sort(c2)
            for _, v in pairs(c2) do last = v end
            f:write(c .. " " ..  last .. "\n")
            c = c + 1
        end
        f:close()
    end
end
experiments = { 
    sizes = { 1024, 1024*2, 1024*4 },
    generations = { 100, 1000, 10000 }
}

results = {}
for k, size in pairs(experiments.sizes) do
    results[size] = {}
    for k, gen in pairs(experiments.generations) do
        --doit(size, size, gen)
        print ("size: " .. size .. "  gen: " .. gen)
        print ("Running ... ")
        results[size][gen] = run_experiment(size, gen)
    end
end




template = [[
]]

--[[
for s, c1 in pairs(results) do
    for g, c2 in pairs(c1) do
        for thread, time in pairs(c2) do
            print("size:" .. s .. 
                  " gen: " .. g .. 
                  " thread: " .. thread .. 
                  " time (ns) " .. time)
        end
    end
end
--]]

results2gnuplot()
