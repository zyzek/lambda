all: lambda.cpp lexer.cpp lexer.h
	clang++ -o lambda lambda.cpp lexer.cpp

clean:
	rm lambda
