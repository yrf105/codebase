package service

import (
	"context"
	"log"

	"github.com/google/uuid"
	"github.com/yrf105/codebase/Go/gRPC-learning/pb"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
)

type LaptopServer struct {
}

func NewLaptopServer() *LaptopServer {
	return &LaptopServer{}
}

func (server *LaptopServer) CreateLaptop(
	ctx context.Context,
	req *pb.CreateLaptopRequest,
) (*pb.CreateLaptopResponse, error) {
	laptop := req.GetLaptop()
	log.Printf("收到一条 create-laptop 请求 id 为 %v", laptop.Id)

	if len(laptop.Id) > 0 {
		// 检查是否是有效的 UUID
		_, err := uuid.Parse(laptop.Id)
		if err != nil {
			return nil, status.Errorf(codes.InvalidArgument, "laptop 的 Id %v 无效", laptop.Id)
		}
	} else {
		
	}
}
