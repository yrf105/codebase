package channel

import (
	"testing"
)

func TestRange(t *testing.T) {
	tests := []struct {
		name string
	}{
		{name: "norm"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			Range()
		})
	}
}
