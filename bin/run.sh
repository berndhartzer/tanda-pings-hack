#!/bin/bash

echo "Beginning full build and run of pings solution!"
echo "Building Docker image (this may take a while)..."

IMAGE="$(docker build . -q)"

echo "Finished building image: ${IMAGE}"
echo "Running Docker container..."

CONTAINER="$(docker run -v $(pwd):/var/www -d -p 3000:80 $IMAGE)"

echo "Container running: ${CONTAINER}"
echo "Making sure container is ready..."

# Don't continue until container responds with http status code 200
while [[ "$(curl -X POST -s -o /dev/null -w ''%{http_code}'' localhost:3000/clear_data)" != "200" ]];
do
    sleep .5;
done

echo "Container is ready!"
echo "Running pings.rb tests..."

ruby pings.rb

echo "Finished running pings.rb tests!"
echo "Stopping docker container..."

docker stop $CONTAINER &>/dev/null

echo "Container stopped. All finished!"
