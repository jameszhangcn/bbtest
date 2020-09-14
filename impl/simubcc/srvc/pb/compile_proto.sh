#!/bin/sh
echo "Start to build all protoc"
CURR_DIR=`pwd`
echo $CURR_DIR
TOP_DIR=$CURR_DIR/../..

for proto in `find . -type f -name "*.proto"`
do 
    protoFilename=`basename $proto`
    protoDir=$CURR_DIR/$dirname
    cd $protoDir
    protoc --proto_path=$GOPATH/src/ --proto_path=$TOP_DIR --proto_path=./ --go_out=plugins=grpc:. $protoFilename
done