require "golpthread"

data_dir = "data"

experiments = { 
    sizes = { 1200, 1200*2, 1200*4},
    generations = { 100, 1000, 10000 }
}


function run_experiment(size, gen) 
    local result

    doit(size, size, gen)
    while true do
        result = stats()
        if result ~= nil then break end
    end
    return result
end

function bath_processing() 
  local results = {}
  for k, size in pairs(experiments.sizes) do
      results[size] = {}
      for k, gen in pairs(experiments.generations) do
          --doit(size, size, gen)
          print ("size: " .. size .. "  gen: " .. gen)
          print ("Running ... ")
          results[size][gen] = run_experiment(size, gen)
      end
  end
  return results
end

function data(r)
    local last
    for s, c1 in pairs(r) do
        c = 0
        f = io.open(data_dir .. "/" .. s .. ".dat", "w")
        for g, c2 in pairs(c1) do
            table.sort(c2)
            for _, v in pairs(c2) do last = v end
            f:write(c .. " " ..  last*1e-9 .. "#" .. s.. ", " .. g .. "\n")
            c = c + 1
        end
        f:close()
    end
end




local results = bath_processing()
data(results)
