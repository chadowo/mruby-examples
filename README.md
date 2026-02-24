# MRuby Examples

Collection of sample [MRuby](https://mruby.org/) programs.

## Build

### Get MRuby

Make sure you've got MRuby installed, see [here](https://github.com/mruby/mruby#how-to-get-mruby).

> [!TIP]
> If you are on Linux chances are MRuby is already in the package repositories of your favorite distro! then you'll need to use your distro specific package manager to install it. E.g:

APT:  
```console
sudo apt install libmruby-dev
```

DNF:  
```console
sudo dnf install mruby-devel
```

Pacman:  
```console
sudo pacman -S mruby
```

### Other requirements

Some examples require SDL2. Install it:
```console
$ sudo apt install libsdl2-dev
```

or 

```
$ sudo dnf install SDL2-devel
```

### Compiling

Use the followings commands:

```console
cmake -S . -B build
cmake --build build
```

## Summary

Examples included here:

### Basic

| Example                                       | Description                                                     |
|:---------------------------------------------:| --------------------------------------------------------------- |
| [hello-world](src/hello-world/)               | Classic hello world.                                            |
| [hello-file](src/hello-file/)                 | Load and execute Ruby code from a file.                         |
| [pushing-variables](src/pushing-variables/)   | Define various types of variables from C to Ruby.               |
| [objects](src/objects/)                       | Create classes & objects from C.                                |
| [method-defined](src/method-defined/)         | Check if a certain method is defined on Ruby.                   |
| [call-methods](src/call-methods/)             | Call Ruby methods from C.                                       |
| [using-cdata](src/using-cdata/)               | Wrap C data inside a Ruby object.                               |
| [kwargs](src/kwargs/)                         | Parse keyword arguments.                                        |
| [blocks](src/blocks/)                         | Interact with Ruby's block from C.                              |
| [compiling-bytecode](src/compiling-bytecode/) | Compile Ruby code into bytecode format and save it on the disk. |

### SDL2

| Example                        | Description                                   |
|:------------------------------:| --------------------------------------------- |
| [sdl-wrapper](src/sdl-wrapper) | Basic wrapper around SDL2 to create a window. |

## Credits

Credits where due, here are some helpful articles/post/resources that helped
me to learn how to use MRuby:

- [Executing Ruby code with MRuby](https://mruby.org/docs/articles/executing-ruby-code-with-mruby.html)
- [MRuby GUI series](https://dev.to/roryo/a-new-smalltalk-style-environment-for-ruby-5f5c)

## License

All the examples found here are licensed under the [MIT License](/LICENSE).
