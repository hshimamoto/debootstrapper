#!/usr/bin/env ruby

require 'webrick'
require 'webrick/httpproxy'

proxy = ENV['http_proxy']

cfg = { :Port => 8080 }
if proxy then
	cfg[:ProxyURI] = URI.parse(proxy)
end

s = WEBrick::HTTPProxyServer.new(cfg)
trap('INT') {
	s.shutdown
}
s.start
