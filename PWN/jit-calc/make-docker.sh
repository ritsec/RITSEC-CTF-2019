#!/bin/bash


sudo /home/ubuntu/.local/bin/dockerize -t jit-calc --add-file `realpath flag` /flag --add-file /bin/cat /bin/cat --add-file /bin/sh /bin/sh --add-file /bin/bash /bin/bash --add-file `realpath jit-calc` /jit-calc --entrypoint /jit-calc `realpath jit-calc`

