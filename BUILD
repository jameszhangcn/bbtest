#!/bin/bash

usage(){
	echo ""
	echo "Usage: `basename $0` product [tag]"
	echo " The format of load is: <product>-[tag].tar.gz"
	exit 1
}

################################################
# set environment variables
################################################
set_env(){
        GOOS="linux"
	GOARCH="amd64"
	echo "Checking the GO environment"
	GO_SDK=`which go`
	[ -z "$GO_SDK" ] && echo "Error:The GO SDK is not installed!!!" && exit 1
	
	export GO111MODULE=on

	echo "Creating the directory for Go BUILD"
	cd $TOP_DIR
	[ -d "$BUILD_DIR" ] || mkdir "$BUILD_DIR"

	cd $BUILD_DIR
	pwd
	GO_BIN_DIR="bin"
	GO_PKG_DIR="pkg"
	GO_SRC_DIR="src"

	[ -d "bin" ] || mkdir bin
	[ -d "pkg" ] || mkdir pkg
	[ -d "src" ] || mkdir src

	cd $GO_SRC_DIR
	ln -fs $APP_DIR .
	ln -fs $GOFER_DIR .
        ln -fs $COMMON_DIR .
        echo "Common dir"
        echo $COMMON_DIR
	export GOPATH=$GOMODULES_DIR:$BUILD_DIR
	echo "GOPATH"
	echo $GOPATH
}


#############################################################
# create directory hierarchy for package
#############################################################
create_output_structure() {
	echo "Starting to create output structure"
	cd $TOP_DIR

	[ -d "$PACKAGE_DIR" ] && rm -rf "$PACKAGE_DIR"
	mkdir "$PACKAGE_DIR"

	# Creating directories
	[ -d $IMAGE_DIR ] || mkdir $IMAGE_DIR

	for microservice in $MICRO_SERVICES; do
		directory=`echo $microservice | cut -d: -f1`
		binary=`echo $microservice | cut -d: -f2`
		MICRO_SERVICE_DIR=$IMAGE_DIR/$directory
		[ -d $MICRO_SERVICE_DIR ] || mkdir $MICRO_SERVICE_DIR
	done

	[ -d $SCRIPT_DIR ] || mkdir $SCRIPT_DIR
	[ -d $DEPLOYMENT_DIR ] || mkdir $DEPLOYMENT_DIR

	touch $PACKAGE_DIR/software_version
	echo "version:$VERSION" > $PACKAGE_DIR/software_version
}

build_e1codec() {
	cd $VENDOR_DIR && sh ./build.sh && \
	cp -f $VENDOR_DIR/libasn1.so $BUILD_DIR/src/bbtest/impl/simucucp/ || return 1
}

#############################################################
# build packages from 3rd party vendor
#############################################################
build_vendor() {
    echo "Building vendor..."
    [ ! -z $SKIP_VENDOR ] && return 1
	
	build_e1codec
}

#############################################################
# build services for bbtest
#############################################################
build_service() {
	# Build each microservice
	directory=$1
	binary=$2
	source=$3
	echo "Building the Micro-services($binary)..."

	if [ "$binary" == "simucucp" ]; then
		CGO_ENABLED=1
	else
		CGO_ENABLED=0
	fi
	if [[ "$binary" != "myetcd" ]] && [[ "$binary" != "mynats" ]]; then
	    if [ "$binary" != "testcases" ]; then
	        cd $(echo $BUILD_DIR/src/$source/impl/$directory/main | tr -d '\r')
	        rm -rf $BUILD_DIR/src/$source/deployment/$directory/$binary
	        GOOS=linux GOARCH=amd64 go build --v -gcflags "-N -l" -o $BUILD_DIR/src/$source/deployment/$directory/$binary main.go
	        [ $? -ne 0  ] && exit 1
		if [ "$binary" == "simucucp" ];then
		  #copy libans1 to deployment
		  cd  $BUILD_DIR/src/bbtest/E1Codec
		  ./build.sh
		  cp -f libasn1.so $BUILD_DIR/src/$source/deployment/$directory/
		fi
	    fi 
	    if [ "$binary" == "testcases" ]; then
	        echo "Building the sos ..."
	        cd $(echo $BUILD_DIR/src/$source/impl/$directory | tr -d '\r')
	        rm -rf $BUILD_DIR/src/$source/deployment/$directory/sos
	        mkdir -p $BUILD_DIR/src/$source/deployment/$directory/sos
			for dirgo in $(ls .)
			do
					echo "Building the sos ($dir)..."
				if [ -d $dirgo ]; then
					files=$(ls $dirgo)
					for gofile in $files
					do
					filename=`echo "$gofile" | cut -f 1 -d '.'`
							echo "Building the sos file ($filename).($gofile).."
						echo $filenam
							GOOS=linux GOARCH=amd64 go build -buildmode=plugin -gcflags "-N -l" -o $BUILD_DIR/src/$source/deployment/$directory/sos/$filename.so ./$dirgo/$gofile
							[ $? -ne 0  ] && exit 1
						done
					fi
			done
			cp -rf $BUILD_DIR/src/$source/deployment/$directory/sos/* $BUILD_DIR/src/$source/deployment/simuctl/sos/
	    fi
        else
	        cd $(echo $BUILD_DIR/src/$source/impl/$directory/main | tr -d '\r')
	        rm -rf $BUILD_DIR/src/$source/deployment/$directory/$binary
	        mkdir -p $BUILD_DIR/src/$source/deployment/$directory
                cp ./$binary $BUILD_DIR/src/$source/deployment/$directory
	        [ $? -ne 0  ] && exit 1
	fi
}

#############################################################
# build slowpath
#############################################################
build_bbtest() {
    build_vendor
	
    echo "Building bbtest ..."
	for microservice in $MICRO_SERVICES; do
		directory=`echo $microservice | cut -d: -f1`
		binary=`echo $microservice | cut -d: -f2`
		source=`echo $microservice | cut -d: -f3`

		build_service $directory $binary $source
	done
}

#############################################################
# build datapath
#############################################################
build_up_dp() {
    echo "Building dataplane ..."
	cd $TOP_DIR/up_dp && sh ./BUILD $UPDP_PLTF UPDP $TAG 
}

#############################################################
# check produced bin files (func SCM environment required)
#############################################################
check_bin_files() {
	echo "Starting to check binary files"
	typeset -i error=0
	for microservice in $MICRO_SERVICES; do
		directory=`echo $microservice | cut -d: -f1`
		binary=`echo $microservice | cut -d: -f2`
		source=`echo $microservice | cut -d: -f3`
		binfile=$BUILD_DIR/src/$source/deployment/$directory/$binary
		if [ ! -f $binfile ]; then
			echo "ERROR: failed to build file: $binfile"
			error=1
		else
			echo "OK: $binfile"
		fi
	done
	[ $error -eq 1 ] && exit 1
}

#############################################################
# copy necessary files into package and do packaging
#############################################################
copy_files() {
	echo "Starting to copy files"
	echo $TAG
    globaltag=` echo $TAG | sed 's/[-_]/./g'`
    
	for microservice in $MICRO_SERVICES; do
		directory=`echo $microservice | cut -d: -f1`
		binary=`echo $microservice | cut -d: -f2`
		source=`echo $microservice | cut -d: -f3`
		echo "Copying files for $directory ..."
		cp -r $BUILD_DIR/src/$source/deployment/$directory/* $TOP_DIR/$IMAGE_DIR/$directory
        sed -i 's/GLOBALTAG/'$globaltag'/' $TOP_DIR/$IMAGE_DIR/$directory/build.sh
		echo $globaltag
		echo $TOP_DIR/$IMAGE_DIR/$directory
	done

	# Copy the chart
	cp -rf $APP_DIR/charts $TOP_DIR/$DEPLOYMENT_DIR
    sed -i 's/GLOBALTAG/'$globaltag'/' $TOP_DIR/$DEPLOYMENT_DIR/charts/bbtest/values.yaml    

    [ -d $TOP_DIR/$DEPLOYMENT_DIR/charts/bbtest/config ] || mkdir -p $TOP_DIR/$DEPLOYMENT_DIR/charts/bbtest/config
    cp -rf $APP_DIR/config/* $TOP_DIR/$DEPLOYMENT_DIR/charts/bbtest/config
    cp -rf $APP_DIR/config/* $TOP_DIR/$DEPLOYMENT_DIR/charts/bbtest/charts/simuctl/config

	# Copy the scripts
	cp -rf $APP_DIR/ProductBuild.config $TOP_DIR/$SCRIPT_DIR
	cp -rf $APP_DIR/SrvToSvc.config $TOP_DIR/$SCRIPT_DIR
	cp -rf $APP_DIR/scripts/install_images.sh $TOP_DIR/$SCRIPT_DIR
	cp -rf $APP_DIR/scripts/installChart.sh $TOP_DIR/$SCRIPT_DIR
	cp -rf $APP_DIR/scripts/install.sh $TOP_DIR/$SCRIPT_DIR
	cp -rf $APP_DIR/scripts/launch.sh $TOP_DIR/$SCRIPT_DIR
	cp -rf $APP_DIR/scripts/inventory.ini $TOP_DIR/$SCRIPT_DIR
}

packaging() {
    echo "BUILD_RHEL: packaging..."

	cd $TOP_DIR
	[ -f "$PACKAGE_DIR.tar.gz" ] && \
	mv "$PACKAGE_DIR.tar.gz" "$PACKAGE_DIR.tar.gz.b4"
	tar czvf $PACKAGE_DIR.tar.gz $PACKAGE_DIR &>/dev/null
	echo "$PACKAGE_DIR.tar.gz successfully generated."
}

BUILD_BBTEST() {
	echo "BUILD_RHEL:making output structure..."
	[ -z "$BUILD_ENV" ] && create_output_structure

	echo "BUILD_RHEL:building bb-test ..."
	[ -z "$BUILD_ENV" ] && build_bbtest

	# Package
	[ -z "$BUILD_ENV" ] && check_bin_files
	echo
	echo "BUILD_RHEL:making copy_files..."
	echo
	[ -z "$BUILD_ENV" ] && copy_files
	
	packaging
	
	echo
	echo "BUILD_RHEL: build completes successful"
}

#############################################################
# main body of BUILD script
#############################################################

PRODUCT=`echo $1 | tr a-z A-Z`
TAG=`echo $2 | tr a-z A-Z`

[ "$PRODUCT" = "BBTEST" ] || usage

TOP_DIR=`pwd | xargs dirname`
APP_NAME="bbtest"
APP_DIR="$TOP_DIR/$APP_NAME"
COMMON_DIR="$TOP_DIR/common"
GOMODULES_DIR="$TOP_DIR/gomodules"
VENDOR_DIR="$TOP_DIR/bbtest/E1Codec"
BUILD_DIR="$TOP_DIR/build"
PLATFORM="UBUNTU18"

echo "TOP_DIR=$TOP_DIR"
echo "APP_DIR=$APP_DIR"
echo "BUILD_DIR=$BUILD_DIR"

[ -z "$TAG" ] && VERSION=`cd ../..; pwd | xargs basename` || VERSION=$TAG
echo "VERSION=$VERSION"
echo "TAG=$TAG"

PACKAGE_DIR=$PRODUCT-$PLATFORM_LOWER
[ ! -z "$TAG" ] && PACKAGE_DIR="$PACKAGE_DIR-$TAG"

set_env

PRODUCT_BUILD_CONFIG_FILE=$APP_DIR/ProductBuild.config
. $PRODUCT_BUILD_CONFIG_FILE
echo "PRODUCT_BUILD_CONFIG_FILE=$PRODUCT_BUILD_CONFIG_FILE"
echo "MICRO_SERVICES = $MICRO_SERVICES"
echo "GCOMMON_MICRO_SERVICES = $GCOMMON_MICRO_SERVICES"

# Directory for docker images
IMAGE_DIR=$PACKAGE_DIR/images

# Directory for scripts
SCRIPT_DIR=$PACKAGE_DIR/scripts

# Directory for chart
DEPLOYMENT_DIR=$PACKAGE_DIR/deployment

case $PRODUCT in
BBTEST)     BUILD_BBTEST;;
esac

exit

