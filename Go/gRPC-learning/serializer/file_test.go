package serializer_test

import (
	"testing"

	"github.com/stretchr/testify/require"
	"github.com/yrf105/codebase/Go/gRPC-learning/pb"
	"github.com/yrf105/codebase/Go/gRPC-learning/sample"
	"github.com/yrf105/codebase/Go/gRPC-learning/serializer"
	"google.golang.org/protobuf/proto"
)

func TestFileSerializer(t *testing.T) {
	t.Parallel()

	binaryFile := "../tmp/laptop.bin"
	jsonFile := "../tmp/laptop.json"

	laptop1 := sample.NewLaptop()
	err := serializer.WriteProtobufToBinaryFile(laptop1, binaryFile)
	require.NoError(t, err)

	laptop2 := &pb.Laptop{}
	err = serializer.ReadProtobufFromBinaryFile(binaryFile, laptop2)
	require.NoError(t, err)
	require.True(t, proto.Equal(laptop1, laptop2))

	err = serializer.WriteProtobufToJsonFile(laptop1, jsonFile)
	require.NoError(t, err)
}
