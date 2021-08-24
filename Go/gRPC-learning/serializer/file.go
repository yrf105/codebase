package serializer

import (
	"fmt"
	"io/ioutil"

	"github.com/golang/protobuf/proto"
)

func WriteProtobufToBinaryFile(message proto.Message, filename string) error {
	data, err := proto.Marshal(message)
	if err != nil {
		return fmt.Errorf("%w 不能被 ptoto marshal", err)
	}

	err = ioutil.WriteFile(filename, data, 0644)
	if err != nil {
		return fmt.Errorf("%w 写入文件失败", err)
	}

	return nil
}

func ReadProtobufFromBinaryFile(filename string, message proto.Message) error {
	data, err := ioutil.ReadFile(filename)
	if err != nil {
		return fmt.Errorf("%w 写入文件失败", err)
	}

	err = proto.Unmarshal(data, message)
	if err != nil {
		return fmt.Errorf("%w 不能被 ptoto unmarshal", err)
	}

	return nil
}


func WriteProtobufToJsonFile(message proto.Message, filename string) error {
	data, err := ProtobufToJson(message)
	if err != nil {
		return fmt.Errorf("%w 转化为 Json 失败", err)
	}

	err = ioutil.WriteFile(filename, []byte(data), 0644)
	if err != nil {
		return fmt.Errorf("%w 写入文件失败", err)
	}

	return nil
}