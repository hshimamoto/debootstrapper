#!/usr/bin/env ruby

require 'webrick'
require 'webrick/httpproxy'

s = WEBrick::HTTPProxyServer.new( { :Port => 8080 } )
trap('INT') {
	s.shutdown
}
s.start
