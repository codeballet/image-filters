# Image Filters

The Image Filters program applies filters to bitmap (.bmp) images.
There are currently five different filters that can be applied:

- Grayscale
- Sepia
- Blur - applying a boxblur algorithm
- Reflect - creates a horizontal mirror image
- Edge Detection - applying a Sobel Operator to the image

The program is written in C, as an assignment for the CS50 course at Harvard.

## Running the program

The executable file, which is included in the repo, is called `filter`. To filter a bmp image, enter the following command in the directory where you have that `filter` file:

```
./filter -option infile outfile
```

The available options are:

- `g` for the Grayscale filter
- `s` for the Sepia filter
- `b` for the Blur filter
- `r` for the Reflect filter
- `e` for the Edge Detection filter

Using one of the provided bmp in the `images` folder of the cloned project, an example of a command applying the Edge Detection filter is:

```
./filter -e ./images/yard.bmp outfile.bmp
```

## Compiling the program

The repo contains an already compiled, executable file called `filter`.

However, to compile the program yourself, you need to have `make` and the C compiler `clang` installed.
After cloning the repo, compile the program by running the command:
`make filter`

The compiler will create a new, executable file called `filter`.

## License

MIT
