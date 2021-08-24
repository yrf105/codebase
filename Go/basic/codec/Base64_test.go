package mycodec

import "testing"

func TestBase64(t *testing.T) {
	tests := []struct {
		name string
	}{
		// TODO: Add test cases.
		{name: "norm"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			Base64()
		})
	}
}
