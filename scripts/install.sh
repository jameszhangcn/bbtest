#!/bin/bash

if [  ! "X$1" == "Xuninstall" ]  &&  
   [  ! "X$1" == "Xbbtest" ] ||
  [ $# -lt 1 ] ; then
      	echo "usage for install : $0  bbtest " 
        echo "usage for uninstall : $0  uninstall bbtest" 
        echo "$0 bbtest to install bbtest "
        echo "$0 uninstall bbtest to uninstall bbtest "
	exit

fi 

# cdmdir="/data/cdm/"
uninstallscript=$cdmdir"/pkg-manager/utils/uninstall.sh"
installscript=$cdmdir"/pkg-manager/utils/install.sh"
requirements=$cdmdir"/pkg-manager/deployment/cnf/requirements.yaml"

[ ! -d "$cdmdir" ] && echo " cmd dir $cdmdir not exists, config it in this script " && exit 

HELMCMD=$HELM_BIN 
[ "X`which helm`" != "X" ] && HELMCMD="helm"

KUBECTLCMD=$KUBECTL_BIN
[ "X`which kubectl`" != "X" ] && KUBECTLCMD="kubectl"

#[ ! "$INSTALL_PAAS" == "false" ] && echo "INSTALL_PAAS must be false" && exit 
#[ ! "$INSTALL_CAAS" == "false" ] && echo "INSTALL_CAAS must be false" && exit 
#[ ! "$INSTALL_CNF" == "true" ] && echo "INSTALL_CNF must be true" && exit 

installCNF(){
  cp $requirements  $requirements"-bak"`date +%Y%m%d%H%M%S`
  sed  -i '/BBTESTBEGIN/,/BBTESTEND/ d'  $requirements
  $installscript
}

installBbtest(){
  $HELMCMD install --kubeconfig $KUBE_CONFIG  local-repo/bbtest --version  v0.0.1  --name  cuup
}


uninstallCNF(){
  $uninstallscript
}

uninstallBbtest(){
  echo "enter delete bbtest"
  $HELMCMD delete --kubeconfig $KUBE_CONFIG --purge bbtest
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
  #don't do mtcil check
  #$HELMCMD status --kubeconfig $KUBE_CONFIG cnf-mgmt 
  #if [ ! $? -eq 0 ] ; then 
  #  $HELMCMD status --kubeconfig $KUBE_CONFIG mtcil
  #  if [ ! $? -eq 0 ] ; then 
  #    echo "Neither cnf nor mtcil installed, install cnf or mtcil firstly"
  #    exit
#	fi
#  fi 

  if [ "X$1" == "Xbbtest" ] ; then
    installBbtest
  fi
}


doUninstall(){
 echo "uninstall $1"  
  if [ "X$1" == "Xbbtest" ] ; then
      echo "uninstall bbtest"
      uninstallBbtest
  fi

  checkNS
  $HELMCMD status --kubeconfig $KUBE_CONFIG bbtest
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
