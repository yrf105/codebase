package sample

import (
	"math/rand"
	"time"

	"github.com/google/uuid"
	"github.com/yrf105/codebase/Go/gRPC-learning/pb"
)

func init() {
	rand.Seed(time.Now().UnixNano())
}

func randomLayout() pb.Keyboard_Layout {
	switch rand.Intn(4) {
	case 1:
		return pb.Keyboard_QWERTY
	case 2:
		return pb.Keyboard_QWERTZ
	case 3:
		return pb.Keyboard_AZERTY
	default:
		return pb.Keyboard_UNKNOWN
	}
}

func randomBool() bool {
	return rand.Intn(2) == 1
}

func randomInt(min, max int) int {
	return min + rand.Intn(max-min+1)
}

func randomFloat64(min, max float64) float64 {
	return min + rand.Float64()*(max-min)
}

func randomGPUName(brand string) string {
	if brand == "AMD" {
		return randomStringFromSet(
			"RX 590",
			"RX 580",
			"RX 5700-XT",
			"RX Vega-56",
		)
	}

	return randomStringFromSet(
		"RTX 2060",
		"RTX 2070",
		"GTX 1660-Ti",
		"GTX 1070",
	)
}

func randomCPUName(brand string) string {
	if brand == "AMD" {
		return randomStringFromSet(
			"Ryzen 7 PRO 2700U",
			"Ryzen 5 PRO 3500U",
			"Ryzen 3 PRO 3200GE",
		)
	}

	return randomStringFromSet(
		"Xeon E-2286M",
		"Core i9-9980HK",
		"Core i7-9750H",
		"Core i5-9400F",
		"Core i3-1005G1",
	)
}

func randomGPUBrand() string {
	return randomStringFromSet("AMD", "NVIDIA")
}

func randomCPUBrand() string {
	return randomStringFromSet("AMD", "Intel")
}

func randomStringFromSet(strs ...string) string {
	n := len(strs)
	if n == 0 {
		return ""
	}
	return strs[rand.Intn(n)]
}

func randomFloat32(min, max float32) float32 {
	return min + rand.Float32()*(max-min)
}

func randomPanel() pb.Screen_Panel {
	if rand.Intn(2) == 1 {
		return pb.Screen_IPS
	}

	return pb.Screen_OLED
}

func randomId() string {
	return uuid.New().String()
}

func randomLaptopBrand() string {
	return randomStringFromSet(
		"Apple", "Dell", "Lenovo",
	)
}

func randomLaptopName(brand string) string {
	switch brand {
	case "Apple":
		return randomStringFromSet("Macbook Air", "Macbook Pro")
	case "Dell":
		return randomStringFromSet("Latitude", "Vostro", "XPS", "Alienware")
	default:
		return randomStringFromSet("Thinkpad X1", "Thinkpad P1", "Thinkpad P53")
	}
}