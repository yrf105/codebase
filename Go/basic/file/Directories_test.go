package myfile

import "testing"

func TestDirectories(t *testing.T) {
	tests := []struct {
		name string
	}{
		// TODO: Add test cases.
		{name: "name"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			Directories()
		})
	}
}
