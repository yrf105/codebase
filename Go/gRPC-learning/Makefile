gen:
	protoc --proto_path=proto proto/*.proto --go_out=pb --go_opt=paths=source_relative

clean:
	rm -rf pb/*.go

run:
	go run main.go

.PHONY: gen clean run