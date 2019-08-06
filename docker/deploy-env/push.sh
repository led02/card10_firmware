#!/usr/bin/env bash

# Build and push the deploy env Docker container to the Gitlab container registry.

set -e

function rev() {
    ( cd $1; git describe --always --match "v[0-9].*" --dirty )
}

IMAGE=derq3k/card10-deploy-env
TAG=$(TZ=UTC date +%Y%m%d-%H%M%SZ)-$(rev .)

docker build -t $IMAGE:$TAG .
docker push $IMAGE:$TAG

echo "Pushed $IMAGE:$TAG"
