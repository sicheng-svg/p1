all:write read

read:read.cc contacts.pb.cc
	g++ -std=c++17 -o $@ $^ -lprotobuf

write:write.cc contacts.pb.cc
	g++ -std=c++17 -o $@ $^ -lprotobuf

.PHONY:clean
clean:
	rm -rf write read