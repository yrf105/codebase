package service

import (
	"fmt"
	"sync"

	"github.com/yrf105/codebase/Go/gRPC-learning/pb"
)

var AlreadyExistError = fmt.Errorf("该笔记本已存在")

type LaptopStore interface {
	Save(laptop *pb.Laptop) error
}

type InMemoryLaptopStore struct {
	mu   sync.Mutex
	data map[string]*pb.Laptop
}

func NewInMemoryLaptopStore() *InMemoryLaptopStore {
	return &InMemoryLaptopStore{
		data: make(map[string]*pb.Laptop),
	}
}

func (store *InMemoryLaptopStore) Save(laptop *pb.Laptop) error {
	store.mu.Lock()
	defer store.mu.Unlock()

	if store.data[laptop.Id] != nil {
		return AlreadyExistError
	}

	// 深拷贝
}
