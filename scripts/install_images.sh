#!/bin/bash
#####################################################################
#                                                                   #
# Copyright (C) 2019                                                #
# Mavenir Systems, Inc.                                             #
# Richardson, TX, USA                                               #
#                                                                   #
# ALL RIGHTS RESERVED                                               #
#                                                                   #
# Permission is hereby granted to licenses of Mavenir Systems,      #
# Inc. products to use or abstract this computer program for the    #
# sole purpose of implementing a product based on                   #
# Mavenir Systems, Inc. products.  No other rights to reproduce,    #
# use, or disseminate this computer program, whether in part or in  #
# whole, are granted.                                               #
#                                                                   #
# Mavenir Systems, Inc. makes no representation or warranties       #
# with respect to the performance of this computer program, and     #
# specifically disclaims any responsibility for any damages,        #
# special or consequential, connected with the use of this program. #
#                                                                   #
#####################################################################
# Version: 1.0                                                      #
# 2020.05.01 Initial                                                #
#####################################################################

usage ()
{
    echo ""
    echo "Usage: `basename $0` <image_repo_IP:port> "
    echo ""
    echo "- image_repo_IP:port sample, 10.2.11.31:5000"
    echo "  The created image will be push to the image repo!"
    echo ""
    exit 1
}

[ $# -ne 1 ]&&usage

which docker
[ $? -ne 0 ]&&echo "docker runtime not found!"&&exit 1

#baseimage_loaded=`docker images -q mone-rhel7.6:v2`
#[ -z "$baseimage_loaded" ]&&echo "Base image(mone-rhel7.6:v2) has not been loaded!"&&exit 2

TOP_DIR=`pwd | xargs dirname`
echo "TOP_DIR=$TOP_DIR"
SCRIPT_DIR="$TOP_DIR/scripts"
IMAGE_DIR="$TOP_DIR/images"

SOFTWARE_VERSION_FILE=$TOP_DIR/software_version
VERSION=`grep "version:" $SOFTWARE_VERSION_FILE | cut -d: -f2 | sed 's/[-_]/./g'`

PRODUCT_BUILD_CONFIG_FILE=$SCRIPT_DIR/ProductBuild.config
. $PRODUCT_BUILD_CONFIG_FILE

echo "PRODUCT_BUILD_CONFIG_FILE=$PRODUCT_BUILD_CONFIG_FILE"
echo "version:"$VERSION

listfile=$TOP_DIR/scripts/tag_list
[ -f "$listfile" ]&&echo "Move $listfile to backup."&&mv -f $listfile ${listfile}.backup

for microservice in $MICRO_SERVICES; do
    directory=`echo $microservice | cut -d: -f1`
    binary=`echo $microservice | cut -d: -f2`
    echo "Building the images($directory) ..."
    cd $IMAGE_DIR/$directory
	#[ "$binary" == "bccsvc" ] && binary="bccApp"
    microsvc_version=`ls -l --time-style='+%Y%m%d_%H%M%S' $binary | awk '{print $6}'`
	#[ "$binary" == "bccApp" ] && binary="bccsvc"
#    tag="$binary:${VERSION}.$microsvc_version"
    tag="$binary:${VERSION}"
#    docker stop $binary
#    docker rm $binary
    docker rmi ${tag}
    docker rmi $1/$tag

    echo -e "Image $tag build..."
    docker build --no-cache --rm=true -t ${tag} .
    echo -e "Image $tag build done."
    echo -e "Create image $1/$tag."
    docker tag $tag $1/$tag
    echo -e "Push image $1/$tag."
    docker push $1/$tag
    echo "$binary ${VERSION}.$microsvc_version" >> $listfile
    echo "------------------------------------------------"
done
