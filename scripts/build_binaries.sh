
build_e1codec() {
	cd $TOP_DIR/$source/E1Codec && sh ./build.sh && \
	cp -f $TOP_DIR/$source/E1Codec/libasn1.so $TOP_DIR/$source/deployment/$directory/ || return 1
}

build_service() {
    # Build each microservice
    directory=$1
    binary=$2
    source=$3
    echo "Building the Micro-services(${binary})..."

	echo $binary
    if [ $binary == "intfmgrsvc" ]; then
	    build_e1codec
        CGO_ENABLED=1
    else
        CGO_ENABLED=0
    fi
	
	cd $TOP_DIR/$source/impl/processes/$directory/main
    GOOS=linux GOARCH=amd64 go build --v -gcflags "-N -l" -o $TOP_DIR/$source/deployment/$directory/$binary main.go
}

TOP_DIR=`pwd | xargs dirname`
APP_DIR=`pwd`
PRODUCT_BUILD_CONFIG_FILE=$APP_DIR/ProductBuild.config
. $PRODUCT_BUILD_CONFIG_FILE
echo "PRODUCT_BUILD_CONFIG_FILE=$PRODUCT_BUILD_CONFIG_FILE"
echo "MICRO_SERVICES = $MICRO_SERVICES"
echo "Building the images"
for microservice in $MICRO_SERVICES; do
    directory=`echo $microservice | cut -d: -f1`
    binary=`echo $microservice | cut -d: -f2`
    source=`echo $microservice | cut -d: -f3`
	
	if [ "$directory"x = "bccsvc"x ]; then
		echo "Building bcc, dbCfgMgr, fastpath..."
		#
	else
		build_service $directory $binary $source
	fi 
done