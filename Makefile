.PHONY: build
configure:
	rm -r ./build
	mkdir -p ./build
	cd build && cmake ..
build: 
	cd build && make -j16
	./build/bin/observer
run_server:
	./build/bin/observer
run_client:
	./build/bin/obclient
none:
