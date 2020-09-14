package mtcilsvc

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"strings"

	"bbtest/impl/simumtcil/srvc/common"

	"github.com/coreos/etcd/clientv3"
)

var loadConfigUrlStr = "http://localhost:9999/api/v1/_operations/loadConfig"
var updateUrlStr = "http://localhost:9999/updateConfig"

var (
	configChangePath string
	dayOneConfigPath string
	Namespace        string
	MicroserviceName string
	AppVersion       string
)

func Init() {
	MicroserviceName = os.Getenv("MICROSERVICE_NAME")
	Namespace = os.Getenv("K8S_NAMESPACE")
	AppVersion = os.Getenv("APPVERSION")
}

//
func WatchConfigChange(ctx context.Context, etcd *clientv3.Client, etcdWatchKey string) {
	//watcher = clientv3.NewWatcher(etcd)
	fmt.Println("CIM start WatchConfigChange")
	defer etcd.Close()
	watchChan := etcd.Watch(ctx, etcdWatchKey, clientv3.WithPrefix())
	if watchChan == nil {
		fmt.Println("watch channel is nill")
		return
	}

	for watchResp := range watchChan {
		select {
		case <-ctx.Done():
			{
				fmt.Println("Watch config Done")
				return
			}
		default:
			{
				go func(resp clientv3.WatchResponse) {
					for _, event := range resp.Events {
						handleConfigUpdate(event)
					}
				}(watchResp)
			}
		}
	}
}

func watchDayOneConfig(ctx context.Context, etcd *clientv3.Client, etcdWatchKey string) {

	fmt.Println("CIM start watchDayOneConfig")
	defer etcd.Close()
	watchChan := etcd.Watch(ctx, etcdWatchKey, clientv3.WithPrefix())
	if watchChan == nil {
		fmt.Println("watch channel is nill")
		return
	}

	for watchResp := range watchChan {
		select {
		case <-ctx.Done():
			{
				fmt.Println("Watch config Done")
				return
			}
		default:
			{
				go func(resp clientv3.WatchResponse) {
					for _, event := range resp.Events {
						handleLoadDayOneConfig(event)
					}
				}(watchResp)
			}
		}
	}
}

func handleAppConfigUpdate(event *clientv3.Event, keys []string, eventType string) {
	var data = map[string]string{
		"change-set-key": string(event.Kv.Key),
		"data-key":       keys[5],
		"config-patch":   string(event.Kv.Value),
		"revision":       keys[6],
	}
	jsonValue, err := json.Marshal(data)
	if err != nil {
		fmt.Println("Error while marshalling the data", err)
		return
	}
	resp, err := http.Post(updateUrlStr, "application/json", bytes.NewReader(jsonValue))
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(resp)
}

func handleAppLoadConfig(event *clientv3.Event, keys []string, eventType string) {
	var data = map[string]string{
		"change-set-key": string(event.Kv.Key),
		"data-key":       keys[5],
		"config-patch":   string(event.Kv.Value),
		"revision":       keys[6],
	}
	jsonValue, err := json.Marshal(data)
	if err != nil {
		fmt.Println("Error while marshalling the data", err)
		return
	}
	resp, err := http.Post(updateUrlStr, "application/json", bytes.NewReader(jsonValue))
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(resp)
}

func handleConfigUpdate(event *clientv3.Event) {
	fmt.Println("Event received: ", event.Type, "executed on", string(event.Kv.Key[:]), "with value", string(event.Kv.Value[:]))
	eventType := fmt.Sprintf("%s", event.Type)
	keys := strings.Split(string(event.Kv.Key), "/")
	if MicroserviceName == keys[2] && AppVersion == keys[3] && eventType == "PUT" {
		handleAppConfigUpdate(event, keys, eventType)
	}
}

func handleLoadDayOneConfig(event *clientv3.Event) {
	fmt.Println("Event received: ", event.Type, "executed on", string(event.Kv.Key[:]), "with value", string(event.Kv.Value[:]))
	eventType := fmt.Sprintf("%s", event.Type)
	keys := strings.Split(string(event.Kv.Key), "/")
	if MicroserviceName == keys[2] && AppVersion == keys[3] && eventType == "PUT" {
		handleAppLoadConfig(event, keys, eventType)
	}
}

func WatchETCD() {
	ctx := context.Background()
	dayOneConfigPath = "config/" + Namespace + "/" + MicroserviceName
	configChangePath = "change-set/" + Namespace + "/" + MicroserviceName
	go watchDayOneConfig(ctx, common.EtcdClient, dayOneConfigPath)
	go WatchConfigChange(ctx, common.EtcdClient, etcdWatchKey)
}
