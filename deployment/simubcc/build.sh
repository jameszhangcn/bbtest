#!/bin/sh
echo -e "Start to build the docker image"
svc="bccsvc"
tag=GLOBALTAG
docker stop ${svc}
docker rm ${svc}
docker rmi ${svc}:${tag}

echo -e "Docker build..."
docker build --no-cache --rm=true -t ${svc}:${tag} .
echo -e "rebuild finished"
