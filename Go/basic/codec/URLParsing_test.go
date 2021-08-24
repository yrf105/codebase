package mycodec

import "testing"

func TestURLParsing(t *testing.T) {
	tests := []struct {
		name string
	}{
		// TODO: Add test cases.
		{name: "norm"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			URLParsing()
		})
	}
}
