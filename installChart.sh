#!/bin/bash

if [ $# -lt 3 ]  ; then  
       	echo "usage: $0  [cuup] [image_repo_IP:port] [rhel8|centos7] [couchdbip] [cdmip] " 
        echo "such as $0 cuup 10.2.11.31:5000 rhel8 10.2.3.26"
		echo "image_repo_IP:port sample, 10.2.11.31:5000 "
		echo "[rhel8|centos7] is hostos, default is rhel8 if not input"
		echo "[couchdbip] is ip of couchdb, it's optional "		
		echo "[cdmip] is ip of a remote cdm, it's optional "
	exit
fi

srv=$1
huburl=$2
hostos=$3
couchdbip=$4
cdmip=$5

echo "srv=$srv"
echo "huburl=$huburl"
echo "hostos=$hostos"
echo "couchdbip=$couchdbip"
echo "cdmip=$cdmip"

echo "cdm ip="$cdmip
echo "image_repo_IP:port="$huburl

#configuration 
#cdmdir="/data/cdm/"
cdmworkdir=/data/nr-charts/nr-charts
#
bakdir=$cdmworkdir"-bak"`date +%Y%m%d%H%M%S`

if [ "X$cdmip" == "X" ] ; then
[ ! -d "$cdmdir" ] && echo " cmd dir $cdmdir not exists, config it in this script " && exit 
echo "run on local cdm"
[ -d $cdmworkdir ] && mv $cdmworkdir $bakdir; mkdir -p $cdmworkdir
echo "workdir on cdm="$cdmworkdir
cp -r ../deployment/charts/* tag_list ../software_version $cdmworkdir
cd $cdmworkdir;chmod a+x $cdmworkdir/*.sh;$cdmworkdir/sync-chart.sh  $srv $huburl $hostos $couchdbip
else
echo "run on remote cdm $cdmip"
ssh $cdmip " [ -d $cdmworkdir ] && mv $cdmworkdir $bakdir; mkdir -p $cdmworkdir"
echo "workdir on cdm="$cdmworkdir
scp -r ../deployment/charts/* tag_list ../software_version $cdmip:$cdmworkdir
ssh $cdmip "cd $cdmworkdir;chmod a+x $cdmworkdir/*.sh;$cdmworkdir/sync-chart.sh $srv $huburl $hostos $couchdbip "

fi 
