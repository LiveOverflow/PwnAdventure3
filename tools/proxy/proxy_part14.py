import socket
import os
from threading import Thread
import parser_part14 as parser
import random
import struct

# this code gets worse and worse!!!

class Proxy2Server(Thread):

    def __init__(self, host, port):
        super(Proxy2Server, self).__init__()
        self.game = None # game client socket not known yet
        self.port = port
        self.host = host
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.connect((host, port))

    # run in thread
    def run(self):
        while True:
            data = self.server.recv(4096)
            if data:
                #print "[{}] <- {}".format(self.port, data[:100].encode('hex'))
                try:         
                    parser.parse(data, self.port, 'server')
                    if len(parser.CLIENT_QUEUE)>0:
                        pkt = parser.CLIENT_QUEUE.pop()
                        #print "got queue client: {}".format(pkt.encode('hex'))
                        self.game.sendall(pkt)
                except Exception as e:
                    print 'server[{}]'.format(self.port), e
                # forward to client
                self.game.sendall(data)

class Game2Proxy(Thread):

    def __init__(self, host, port):
        super(Game2Proxy, self).__init__()
        self.server = None # real server socket not known yet
        self.port = port
        self.host = host
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((host, port))
        sock.listen(1)
        # waiting for a connection
        self.game, addr = sock.accept()

    def run(self):
        while True:
            data = self.game.recv(4096)
            if data:
                #print "[{}] -> {}".format(self.port, data[:100].encode('hex'))
                try:
                    parser.parse(data, self.port, 'client')
                    if len(parser.SERVER_QUEUE)>0:
                        pkt = parser.SERVER_QUEUE.pop()
                        #print "got queue server: {}".format(pkt.encode('hex'))
                        self.server.sendall(pkt)
                except Exception as e:
                    print 'client[{}]'.format(self.port), e
                # forward to server
                self.server.sendall(data)

class Proxy(Thread):

    def __init__(self, from_host, to_host, port):
        super(Proxy, self).__init__()
        self.from_host = from_host
        self.to_host = to_host
        self.port = port
        self.running = False

    def run(self):
        while True:
            print "[proxy({})] setting up".format(self.port)
            self.g2p = Game2Proxy(self.from_host, self.port) # waiting for a client
            self.p2s = Proxy2Server(self.to_host, self.port)
            print "[proxy({})] connection established".format(self.port)
            self.g2p.server = self.p2s.server
            self.p2s.game = self.g2p.game
            self.running = True

            self.g2p.start()
            self.p2s.start()


master_server = Proxy('0.0.0.0', '192.168.178.54', 3333)
master_server.start()

game_servers = []
for port in range(3000, 3006):
    _game_server = Proxy('0.0.0.0', '192.168.178.54', port)
    _game_server.start()
    game_servers.append(_game_server)


while True:
    try:
        
        cmd = raw_input('$ ')
        if cmd[:4] == 'quit':
            os._exit(0)
        elif cmd[0:2] == 'S ':
            # send to server
            for server in game_servers:
                if server.running:
                    parser.SERVER_QUEUE.append(cmd[2:].decode('hex'))
        elif cmd[0:2] == 'C ':
            # send to client
            for server in game_servers:
                if server.running:
                    parser.CLIENT_QUEUE.append(cmd[2:].decode('hex'))
        elif cmd[0:5] == 'brute':
            # go through all possible 16bit values
            for i in range(0,0xFFFF+1):
                brute = bin(i)[2:].rjust(16, '0')
                pattern = "0_1_1_0_1_0_1_1_1_1_1_1_1_1_1_1_".replace('_','')
                # merge the 16bit brute-force value and the fixed pattern
                test = "".join(["{}{}".format(a,b) for a,b in zip(pattern, brute)])
                # convert to integer and place in packet
                circuit_input = int(test, 2)
                pkt = "30310a0046696e616c5374616765".decode('hex')+struct.pack("<I", circuit_input)
                # put the packet in the queue to be sent out
                parser.SERVER_QUEUE.append(pkt)
        elif cmd[0:5] == 'bits ':
            circuit_input = int(cmd[5:], 2)
            print cmd[5:]
            print bin(circuit_input)
            pkt = "30310a0046696e616c5374616765".decode('hex')+struct.pack("<I", circuit_input)
            #    S 30310a0046696e616c537461676500000064
            parser.SERVER_QUEUE.append(pkt)
        else:
            reload(parser)
    except Exception as e:
        print e



