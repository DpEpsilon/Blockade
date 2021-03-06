/* -------------------------------------------
 * Blockade Modules - ioblock.cpp
 * -------------------------------------------
 * Implements a very hacky, but very useful
 * method for blocking reading and writing to
 * stdin and stdout , to prevent modules from
 * intefering with the arbiter-module stream.
 * -------------------------------------------
 * Author:  James Payor (github.com/amoebius)
 * -------------------------------------------
 */


#include "ioblock.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <ext/stdio_filebuf.h> // NB: Non-standard.


// Buffers that can wrap a POSIX file descriptor:
typedef __gnu_cxx::stdio_filebuf<char> fdbuffer;


namespace ioblock {

	int infd = dup(STDIN_FILENO), outfd = dup(STDOUT_FILENO), errfd = dup(STDERR_FILENO), devnull = open("/dev/null", O_RDWR);
	fdbuffer *out, *in, *err, *nullout, *nullin;

	void init() {

		infd = dup(STDIN_FILENO);
		outfd = dup(STDOUT_FILENO);
		errfd = dup(STDERR_FILENO);
		devnull = open("/dev/null", O_RDWR);

		out = new fdbuffer(outfd, std::ios::out);
		in = new fdbuffer(infd, std::ios::in);
		err = new fdbuffer(errfd, std::ios::out);

		dup2(devnull, STDIN_FILENO);
		dup2(devnull, STDOUT_FILENO);
		dup2(devnull, STDERR_FILENO);

		nullout = new fdbuffer(STDOUT_FILENO, std::ios::out);
		nullin = new fdbuffer(STDIN_FILENO, std::ios::in);

		unblock();
	}

	void block() {
		std::cout.rdbuf(nullout);
		std::cin.rdbuf(nullin);
		std::cerr.rdbuf(nullout);
	}

	void unblock() {
		std::cout.rdbuf(out);
		std::cin.rdbuf(in);
		std::cerr.rdbuf(err);
	}

}; // namespace ioblock
