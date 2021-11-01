.PHONY: build
configure:
	rm -r ./build
	mkdir -p ./build
	cd build && cmake ..
build: 
	cd build && make -j16
	./build/bin/observer -f ./etc/observer.ini
run_server:
	./build/bin/observer -f ./etc/observer.ini
run_client:
	./build/bin/obclient
none:
