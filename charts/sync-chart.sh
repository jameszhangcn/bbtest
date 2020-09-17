#!/bin/sh

if [ $# -lt 3 ]  ; then 
       	echo "usage: $0  [bbtest] [image_repo_IP:port] [rhel8|centos7] " 
       # echo "such as $0 10.69.12.211 cuup 10.2.11.31:5000 rhel8"
		echo "image_repo_IP:port sample, 10.2.11.31:5000"
		echo "[rhel8|centos7] is hostos, default is rhel8 if not input"
	exit
fi
version="0.0.1"
nfsvc=$1
huburl=$2
hostos=$3
couchbaseip=$4


[ ! -d "$cdmdir" ] && echo " env cmd dir $cdmdir not exists, config it " && exit 

HELMCMD=$HELM_BIN
allsvc=`grep "name"   $nfsvc/requirements.yaml | grep -v ^# | cut -d ":" -f2`
echo "allsvc=$allsvc"
nf="nr"
[ "X$hostos" == "X" ] && hostos="ubuntu"

######## from common-chart.sh

charts=""
rm -f  $nfsvc"-"$version".tgz"
rm -f ./$nfsvc/charts/*.tgz

for svc in `echo $allsvc`
do
    echo $svc
    mv ./$nfsvc/charts/$svc ./
    imgname=$svc
    imgtag=`grep ^$imgname tag_list |cut -d" " -f2`
    sed -i '/image:.*'$imgname'/,/tag/s/\(tag: \)\(.*\)/\1'$imgtag'/' $svc/values.yaml
    sed -i 's/appVersion:.*/appVersion: "'$imgtag'"/' $svc/Chart.yaml
    rm -f "$svc*.tgz"
    $HELMCMD package ./$svc
    cp -f $svc*.tgz ./$nfsvc/charts/
    charts=$charts"  $svc*.tgz"
done

software_version=`head software_version  |cut -d":" -f2`
sed -i 's/appVersion:.*/appVersion: "'$software_version'"/' ./$nfsvc/Chart.yaml

nfsvcversion=`sed -n -e '/version/ s/version:\(.*\)/\1/p' ./$nfsvc/Chart.yaml | xargs`

echo "ls -al ./$nfsvc/charts/"
ls -al ./$nfsvc/charts/
sed -i 's/GLOBALHUBURL/'$huburl'/' $nfsvc/values.yaml
# sed -i 's/COUCHBASEIP/'$couchbaseip'/' $nfsvc/values.yaml
# sed -i "s/PODSLABEL/'\"true\"'/g" $nfsvc/values.yaml
# sed -i '/serverName/s/"serverName":".*",/"serverName":"'$couchbaseip'",/g' $nfsvc/config/mgmt/storeware.json

$HELMCMD package ./$nfsvc
# rm -f  ./$nf/charts/$nfsvc"-"*.tgz
ls -al $nfsvc-$version.tgz


#cp  $nfsvc"-"$nfsvcversion".tgz" ./$nf/charts
#ls -al ./$nf/charts

#sed -i '/'$nfsvc'/,/repo/ s/^#//g' ./$nf/requirements.yaml

#$HELMCMD package ./$nf

charts=$charts" $nfsvc-$nfsvcversion.tgz "

[ -d "/repo/charts/" ] || ((cd / && mkdir repo && cd repo && ln -s /data/chartrepo/charts charts) && \
(cd /root && mkdir chartrepo && cd /root/chartrepo && ln -s /data/chartrepo/charts charts))

ls  -al $charts
cp -f $charts /repo/charts/
curdir=`pwd`
cd /repo/charts/
echo "in /repo/charts/"
ls -al $charts
cd $curdir

for i in `ls $charts`; do 
  echo pushing chart: $i; 
  url=`echo $i | sed -e 's/.tgz//' -e 's/-/\//'`
  curl -X DELETE $CHART_REPO_ADDR/api/charts/$url  ;
  curl --data-binary "@$i" $CHART_REPO_ADDR/api/charts ;
  echo ""
done
echo "================================"
echo "check the output to confirm the charts are as expected:"
echo "================================"
$HELMCMD install --dry-run --debug $nfsvc  --kubeconfig $KUBE_CONFIG
$HELMCMD repo update  --kubeconfig $KUBE_CONFIG


