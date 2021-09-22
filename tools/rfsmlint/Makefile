byte:
	dune build rfsmlint.bc
	dune build rfsmlint.exe

test: 
	./_build/default/rfsmlint.bc ./tests/chrono.fsd


clean:
	dune clean

clobber: clean
	@rm -f *~
