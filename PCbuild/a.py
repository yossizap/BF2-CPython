import socket

sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
sock.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1 )
sock.bind( ('127.0.0.1', 12345) )
sock.listen(1)
sock.setblocking( 0 )

try:
    sock.acceptEx()
except Exception, e:
    print str(e)
    
import pdb; pdb.set_trace()