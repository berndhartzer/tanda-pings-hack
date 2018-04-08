#!/bin/bash

CONTAINER=$1

docker run -v $(pwd):/var/www -d -p 3000:80 $CONTAINER
