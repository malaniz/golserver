require "orbit"
require "golpthread"

module("golserver", package.seeall, orbit.new)


golserver:dispatch_get(function (web)
    local w = web.GET['width'] or 80
    local h = web.GET['height'] or 80
    return doit(h, w, 1000)
end , "/run")

golserver:dispatch_get(function (web)
    web.headers['Content-type'] = 'application/json' 
    return data();
end , "/data")

golserver:dispatch_get(function (web)
    return web:redirect("/static/index.html")
end, "/")


golserver:dispatch_static("/static/.+")

