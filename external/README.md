#How to build a libgit2 for this project from source

#Steps

##libssh2

[libssh2](https://github.com/libssh2/libssh2) is required for libgit2 as we are using an ssh access.

You will need a [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) (the instalation path can NOT contain whitespaces) or some other supported alternative to build libssh2. You also need a [CMake](https://cmake.org/) and [Make](http://gnuwin32.sourceforge.net/packages/make.htm) installed and preferably in PATH.

###How to:

1) clone repository
2) `mkdir build` inside repository
3) `cd build`
4) `cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -G"MinGW Makefiles" ..`
5) `cmake --build .`
6) DONE

##libgit2

You can get all files from [libgit2 Github](https://github.com/libgit2/libgit2)

You will need a [libssh2](##libssh2) already build.

###How to:

1) clone repository
2) `mkdir build` inside repository
3) `cd build`
4 ubuntu) `cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_CLAR=OFF -DLIBSSH2_FOUND=TRUE -DLIBSSH2_INCLUDE_DIRS=/mnt/c/Users/Matej/Desktop/libssh2-1.9.0/include/ -D=LIBSSH2_LIBRARY_DIRS=/mnt/c/Users/Matej/Desktop/libssh2-1.9.0/build/src/ ..`
4 windows) `cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_CLAR=OFF -DLIBSSH2_FOUND=TRUE -DLIBSSH2_INCLUDE_DIRS=C:/Users/Matej/Desktop/libssh2-1.9.0/include/ -D=LIBSSH2_LIBRARY_DIRS=C:/Users/Matej/Desktop/libssh2-1.9.0/build/src/  -G"MinGW Makefiles" ..`
5) `cmake --build .`
6) DONE

#Finish

Now you only need `libgit2-1.0.1/include/*` and `libgit2-1.0.1/build/Debug/git2.lib` that was build previosly.

Just save them inside `/external/libgit2/includes/` and `/external/libgit2` respectivly.