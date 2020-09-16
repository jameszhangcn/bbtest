#!/bin/bash

if [ $# -gt 2 -o $1 == "-h" ] ; then
  echo "$0 [cdm path] [kube config file]"
  echo  "[Optional] cdm path e.g., /opt/cdm"
  echo  "[Optional] Kube config file: e.g., /tmp/admin.conf"
  exit
fi 

[ ! -z $1 ] && cdmdir=`echo $1 | xargs`|| cdmdir="/opt/cdm"
export cdmdir

function parse_vars(){
  VARFILE=$1
  while IFS=: read -ra arr; do
    ( [[ ${arr[0]} =~ ^# ]] || [[ ${arr[0]} =~ ^- ]] || [[ ${arr[0]} =~ ^[[:space:]] ]] ) &&  continue
    key=`echo ${arr[0]} | xargs`
    val=`echo ${arr[1]} | xargs`
    [[ ! -z "$key" ]] && [[ ! -z "$val" ]] && echo "$key=$val" &&export "$key=$val"
  done < $VARFILE
}

parse_vars ${cdmdir}/playbooks/inventories/cdm-docker/group_vars/all
parse_vars ${cdmdir}/playbooks/inventories/cdm-docker/group_vars/chart_repository
CHART_REPO_PORT=$port

function parse_inventory_ini() {
  INIFILE=$1
  SECTION=$2
  KEY=$3

  #echo $INIFILE
  #echo $SECTION
  RETURN=`grep -A1 "\[$SECTION\]" $INIFILE | grep -v "\[$SECTION\]"`
  export "$KEY=$RETURN"
}

parse_inventory_ini ${cdmdir}/playbooks/inventories/cdm-docker/inventory.ini chart_repository CHART_REPO_ADDR

#Image Repo
imgRepoUrl=$CHART_REPO_ADDR:30500
export IMAGE_REPO_ADDR=$imgRepoUrl
echo "IMAGE_REPO_ADDR=$IMAGE_REPO_ADDR"

#Chart Repo
export CHART_REPO_NAME=chartrepo
export CHART_REPO_ADDR=$CHART_REPO_ADDR:$CHART_REPO_PORT
echo "CHART_REPO_ADDR=$CHART_REPO_ADDR"

#Install Config (true/false)
export INSTALL_CAAS=false
export INSTALL_PAAS=false
export INSTALL_CNF=true

###############################################
#Shorthand Variable Declarations (Do not edit)#
###############################################
[ ! -z $2 ] && ADMIN_CONF=`echo $2 | xargs`|| ADMIN_CONF="/opt/cdm/pkg-manager/conf/admin.conf"
export BASE_PATH=$cdmdir/pkg-manager/
export KUBE_CONFIG=$ADMIN_CONF
export HELM_DEPLOY=$BASE_PATH/deployment/
export HELM_HOME=/root/.helm
echo "KUBE_CONFIG=$KUBE_CONFIG"

prod="cuup"

./install_images.sh $imgRepoUrl || exit 1
./installChart.sh $prod $imgRepoUrl centos7
./install.sh uninstall $prod
./install.sh $prod
