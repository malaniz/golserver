require "orbit"
require "golpthread"

module("golserver", package.seeall, orbit.new)


golserver:dispatch_get(function (web)
    local w = web.GET['width'] or 80
    local h = web.GET['height'] or 80
    return doit(h, w);
end , "/run")

golserver:dispatch_get(function (web)
    web.headers['Content-type'] = 'application/json' 
    return data();
end , "/data")


golserver:dispatch_static("/static/.+");
golserver:dispatch_static("/js/.+");

