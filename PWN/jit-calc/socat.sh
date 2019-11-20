#!/bin/bash
sudo socat TCP-LISTEN:8000,reuseaddr,fork EXEC:"docker run --rm -ti 0da3052ebdd0",pty
