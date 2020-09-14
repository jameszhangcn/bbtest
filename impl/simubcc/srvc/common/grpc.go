package common

import (
	"context"
	"fmt"
	"log"
	"net"
	"os"
	"time"

	"google.golang.org/grpc"
	"google.golang.org/grpc/reflection"

	pb "bbtest/impl/simubcc/srvc/pb"
)

const (
	address     = "localhost:9009"
	defaultName = "world"
)

var intfclient pb.GreeterClient

func SendGrpc() {
	name := defaultName
	if len(os.Args) > 1 {
		name = os.Args[1]
	}

	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()
	r, err := intfclient.SayHello(ctx, &pb.HelloRequest{Name: name})
	if err != nil {
		log.Fatalf("could not gret: %v", err)
	}
	log.Printf("Greeting: %s", r.Message)
}

type server struct{}

func (s *server) SayHello(ctx context.Context, in *pb.HelloRequest) (*pb.HelloReply, error) {
	return &pb.HelloReply{Message: "Hello " + in.Name}, nil
}

func InitGrpcServer() {

	lis, err := net.Listen("tcp", "localhost:9009")
	if err != nil {
		log.Fatalf("failed to lister: %v", err)
	}

	s := grpc.NewServer()

	pb.RegisterGreeterServer(s, &server{})

	reflection.Register(s)
	if err := s.Serve(lis); err != nil {
		log.Fatalf("failed to serve: %v", err)
	}
}

func CreateGrpcClient() {
	//set up a connection to server.
	conn, err := grpc.Dial(address, grpc.WithInsecure(), grpc.WithTimeout(time.Second*5), grpc.WithBlock())
	if err != nil {
		log.Fatalf("did not connect: %v", err)
	}
	fmt.Println("conn: ", conn)
	defer conn.Close()
	intfclient = pb.NewGreeterClient(conn)

}

func StartGrpc() {
	go InitGrpcServer()
	CreateGrpcClient()
}
