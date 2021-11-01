.PHONY: build
configure:
	rm -rf ./build
	mkdir -p ./build
	cd build && cmake ..
configure_debug:
	rm -rf ./build
	mkdir -p ./build
	cd build && cmake .. -DDEBUG=ON
build: 
	cd build && make -j16
	./build/bin/observer -f ./etc/observer.ini
run_server:
	./build/bin/observer -f ./etc/observer.ini
run_client:
	./build/bin/obclient
none:
