# iogen
 Auto-generator of userspace wrappers for IOKit user client external methods

## What is iogen?
iogen is a tool to automate the creation of userspace wrappers for IOKit user client external methods. Simply pass it an IOExternalMethodDispatch array, and it will generate a source and header file with the userspace wrapper functions for that IOUserClient.

## Compilation:
iogen doesn't have any dependancies so can easily be compiled on any machine, with any architecture, using any compiler. Here is an example using cmake:
```
cmake . -DCMAKE_BUILD_TYPE=Release
make install
```

## Usage
```
iogen input_file [-o output_file] [-h header_output_file]
```
**Example:**
There is a `sample_input.txt` file included in this repo which will generate the wrapper functions for IOHIDLibUserClient:
```
iogen sample_input.txt -o iohid.c
```

I hope you find this useful. Please feel free to make a pull request with any contributions if you have any :)