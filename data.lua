#!/usr/bin/env wsapi.cgi
require "golpthread"

module("hello", package.seeall)

doit()
function run(wsapi_env)
  local headers = { ["Content-type"] = "text/plain" }

  local function hello_text()
    coroutine.yield(data())
  end

  return 200, headers, coroutine.wrap(hello_text)
end

return _M
