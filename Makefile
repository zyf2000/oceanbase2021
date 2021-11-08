.PHONY: build
configure:
	rm -rf ./build
	mkdir -p ./build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
configure_debug:
	rm -rf ./build
	mkdir -p ./build
	cd build && cmake .. -DDEBUG=ON
build:
	# Note: we can't assure that the judge machine
	# have the bison and flex libraries, hence, we shall
	# proceed the compilation progress in-place
	@echo "Compiling the Yacc and Lex files"
	@flex --outfile=./src/observer/sql/parser/lex.yy.c  --header-file=./src/observer/sql/parser/lex.yy.h ./src/observer/sql/parser/lex_sql.l 
	@bison -o ./src/observer/sql/parser/yacc_sql.tab.c --defines=./src/observer/sql/parser/yacc_sql.tab.h ./src/observer/sql/parser/yacc_sql.y

	cd build && make -j16
	./build/bin/observer -f ./etc/observer.ini
build_pure:
	cd build && make -j16
	./build/bin/observer -f ./etc/observer.ini
parse_pure:
	@echo "Compiling the Yacc and Lex files"
	@flex --outfile=./src/observer/sql/parser/lex.yy.c  --header-file=./src/observer/sql/parser/lex.yy.h ./src/observer/sql/parser/lex_sql.l               
	@bison -o ./src/observer/sql/parser/yacc_sql.tab.c --defines=./src/observer/sql/parser/yacc_sql.tab.h ./src/observer/sql/parser/yacc_sql.y
run_server:
	./build/bin/observer -f ./etc/observer.ini
run_client:
	./build/bin/obclient
none:
