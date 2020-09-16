#!/bin/bash

if [  ! "X$1" == "Xuninstall" ]  &&  
   [  ! "X$1" == "Xcuup" ] ||
  [ $# -lt 1 ] ; then
      	echo "usage for install : $0  cuup " 
        echo "usage for uninstall : $0  uninstall cuup" 
        echo "$0 cuup to install cuup "
        echo "$0 uninstall cuup to uninstall cuup "
	exit

fi 

# cdmdir="/data/cdm/"
uninstallscript=$cdmdir"/pkg-manager/utils/uninstall.sh"
installscript=$cdmdir"/pkg-manager/utils/install.sh"
requirements=$cdmdir"/pkg-manager/deployment/cnf/requirements.yaml"

[ ! -d "$cdmdir" ] && echo " cmd dir $cdmdir not exists, config it in this script " && exit 
#sed -i  '/INSTALL_CAAS=true/s/INSTALL_CAAS=true/INSTALL_CAAS=false/g' $cdmdir/pkg-manager/utils/variables.sh
#sed -i  '/INSTALL_PAAS=true/s/INSTALL_PAAS=true/INSTALL_PAAS=false/g' $cdmdir/pkg-manager/utils/variables.sh
#sed -i  '/INSTALL_CNF=false/s/INSTALL_CNF=false/INSTALL_CNF=true/g' $cdmdir/pkg-manager/utils/variables.sh
#source $cdmdir/pkg-manager/utils/variables.sh

HELMCMD=$HELM_BIN 

KUBECTLCMD=$KUBECTL_BIN
[ "X`which kubectl`" != "X" ] && KUBECTLCMD="kubectl"

#[ ! "$INSTALL_PAAS" == "false" ] && echo "INSTALL_PAAS must be false" && exit 
#[ ! "$INSTALL_CAAS" == "false" ] && echo "INSTALL_CAAS must be false" && exit 
#[ ! "$INSTALL_CNF" == "true" ] && echo "INSTALL_CNF must be true" && exit 

installCNF(){
  cp $requirements  $requirements"-bak"`date +%Y%m%d%H%M%S`
  sed  -i '/CUUPBEGIN/,/CUUPEND/ d'  $requirements
  $installscript
}

installCuup(){
  $HELMCMD install --kubeconfig $KUBE_CONFIG  chartrepo/cuup --version  v0.0.1  --name  cuup
}


uninstallCNF(){
  $uninstallscript
}

uninstallCuup(){
  echo "enter delete cuup"
  $HELMCMD delete --kubeconfig $KUBE_CONFIG --purge cuup
}

checkNS(){
  k8s_master_addr=`grep "server:"  $KUBE_CONFIG | cut -d"/" -f3 | cut -d: -f1`

  while true
  do
    ns=`ssh root@${k8s_master_addr} "kubectl get ns  | grep Terminating | awk '{print $1}'"`
    [ "X$ns" == "X" ] && break
    echo "wait for unterminated namespace: $ns"
    sleep 5
  done
}


doInstall(){
  echo "install $1"  
  $HELMCMD status --kubeconfig $KUBE_CONFIG cnf-mgmt 
  if [ ! $? -eq 0 ] ; then 
    $HELMCMD status --kubeconfig $KUBE_CONFIG mtcil
    if [ ! $? -eq 0 ] ; then 
      echo "Neither cnf nor mtcil installed, install cnf or mtcil firstly"
      exit
	fi
  fi 

  if [ "X$1" == "Xcuup" ] ; then
    installCuup
  fi
}


doUninstall(){
 echo "uninstall $1"  
  if [ "X$1" == "Xcuup" ] ; then
      echo "uninstall cuup"
      uninstallCuup
  fi

  checkNS
  $HELMCMD status --kubeconfig $KUBE_CONFIG cuup 
  [  $? -eq 0 ] && exit   
  #uninstallCNF
  checkNS
  exit
}



if [ "X$1" == "Xuninstall" ] ; then
  doUninstall $2
else
  doInstall $1
fi
