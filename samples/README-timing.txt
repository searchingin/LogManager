All samples were run on a random CAEN server that I happened to log into.

All samples were run with a command similar to:

time ./logman sample-log.txt < sample-???-cmds.txt > sample-???-out.txt

The times below are the user time reported for each command file (second line),
after running each test 3 times and taking the median value:

all: 1.340s
b: 0.252s
c: 0.195s
d: 0.246s
e: 0.204s
k: 0.237s
q: 0.190s
t: 0.203s

