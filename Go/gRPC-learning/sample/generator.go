package sample

import (
	"github.com/yrf105/codebase/Go/gRPC-learning/pb"
	"google.golang.org/protobuf/types/known/timestamppb"
)

func NewKeyboard() *pb.Keyboard {
	keyboard := pb.Keyboard{
		Layout:  randomLayout(),
		Backlit: randomBool(),
	}

	return &keyboard
}

// NewCPU returns a new sample CPU
func NewCPU() *pb.CPU {
	brand := randomCPUBrand()
	name := randomCPUName(brand)
	numberCores := randomInt(2, 8)
	numberThreads := randomInt(numberCores, 16)

	minGhz := randomFloat64(2.0, 3.8)
	maxGhz := randomFloat64(minGhz, 5.0)

	cpu := &pb.CPU{
		Name:          name,
		Brand:         brand,
		NumberCores:   uint32(numberCores),
		NumberThreads: uint32(numberThreads),
		MinGhz:        minGhz,
		MaxGhz:        maxGhz,
	}

	return cpu
}

func NewGPU() *pb.GPU {

	brand := randomGPUBrand()
	name := randomGPUName(brand)

	minGhz := randomFloat64(1.0, 1.5)
	maxGhz := randomFloat64(minGhz, 2.0)

	memGB := randomInt(2, 8)

	gpu := &pb.GPU{
		Name:   name,
		Brand:  brand,
		MinGhz: minGhz,
		MaxGhz: maxGhz,
		Memory: &pb.Memory{
			Value: int64(memGB),
			Unit:  pb.Memory_GIGABYTE,
		},
	}

	return gpu
}

func NewRAM() *pb.Memory {

	ramGB := randomInt(8, 32)

	ram := &pb.Memory{
		Value: int64(ramGB),
		Unit:  pb.Memory_GIGABYTE,
	}

	return ram
}

func NewSSD() *pb.Storage {

	ssd := &pb.Storage{
		Memory: &pb.Memory{
			Value: int64(randomInt(128, 512)),
			Unit:  pb.Memory_GIGABYTE,
		},
		Driver: pb.Storage_SSD,
	}

	return ssd
}

func NewHDD() *pb.Storage {

	hdd := &pb.Storage{
		Memory: &pb.Memory{
			Value: int64(randomInt(1, 4)),
			Unit:  pb.Memory_TERABYTE,
		},
		Driver: pb.Storage_HDD,
	}

	return hdd
}

func NewScreen() *pb.Screen {

	width := randomInt(1280, 4096)
	height := width * 9 / 16

	screen := &pb.Screen{
		SizeInch: randomFloat32(13, 17),
		Resolution: &pb.Screen_Resolution{
			Width:  int32(width),
			Height: int32(height),
		},
		Panel:      randomPanel(),
		Multitouch: randomBool(),
	}

	return screen
}

func NewLaptop() *pb.Laptop {

	brand := randomLaptopBrand()
	name := randomLaptopName(brand)

	laptop := &pb.Laptop{
		Id:          randomId(),
		Brand:       brand,
		Name:        name,
		Cpu:         NewCPU(),
		Ram:         NewRAM(),
		Gpus:        []*pb.GPU{NewGPU()},
		Storages:    []*pb.Storage{NewSSD(), NewHDD()},
		Screen:      NewScreen(),
		Keyboard:    NewKeyboard(),
		Weight:      &pb.Laptop_WeightKg{WeightKg: randomFloat64(1.0, 2.0)},
		PriceUsd:    randomFloat64(500, 800),
		ReleaseYear: uint32(randomInt(2018, 2021)),
		UpdatedAt:   timestamppb.Now(),
	}

	return laptop
}
