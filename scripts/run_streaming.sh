#!/bin/zsh

taskset -c 0 ../bin/UserAccountServer &

python3 ../generator/GeneratorServer.py 0 &
# python3 ../generator/GeneratorServer.py 1 &
# python3 ../generator/GeneratorServer.py 2 &
# python3 ../generator/GeneratorServer.py 3 &


sudo renice -n -20 -u yg397 




