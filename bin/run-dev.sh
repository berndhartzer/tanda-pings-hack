#!/bin/bash

IMAGE=$1

# Run detached
# docker run -v $(pwd):/var/www -d -p 3000:80 $IMAGE

# Run in foreground
docker run -v $(pwd):/var/www -p 3000:80 $IMAGE
