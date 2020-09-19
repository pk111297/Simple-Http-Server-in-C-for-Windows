# Simple-Http-Server-in-C-for-Windows
This is a simple HTTP server code for Windows in C.
This is basically basic for understanding how the server works.

## Running
Now you have to run the server by the following command:
```bash
gcc server.c -o server.exe -l ws2_32
```
Now ws2_32 is basically the library which is needed for compiling and executing the code of sockets in windows and since our HTTP server consists of sockets and so we need to include that library.

Now there will be some warnings but ignore them.

Now to run the code type
```bash
server.exe
 ```
 
 ## Images
 ![](https://github.com/pk111297/Simple-Http-Server-in-C-for-Windows/blob/master/Screenshot%20(53).png)
