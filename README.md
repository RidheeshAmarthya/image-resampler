# Image Resampling and Format Conversion

This program implements image resampling and format conversion techniques. It takes an input image in RGB format and generates an output image in one of three standard output formats, using various resampling methods.

## Usage

The program takes five command-line arguments:

```bash
image.exe <input_image_path> <width> <height> <resampling_method> <output_format>
```

- `<input_image_path>`: Path to the input RGB image file
- `<width>`: Width of the input image in pixels (4000 or 400)
- `<height>`: Height of the input image in pixels (3000 or 300)
- `<resampling_method>`: Integer value (1 or 2) specifying the resampling method
- `<output_format>`: String specifying the output format (O1, O2, or O3)

## Input Image Formats

- High-resolution: 4000x3000 pixels
- Low-resolution: 400x300 pixels

## Output Image Formats

- O1: 1920x1080 pixels
- O2: 1280x720 pixels
- O3: 640x480 pixels

## Resampling Methods

### Downsampling (input larger than output)
1. Specific sampling: Choose one specific pixel
2. Average (or Gaussian) smoothing: Choose the average of a set of samples

### Upsampling (input smaller than output)
1. Nearest neighbor interpolation
2. Bilinear/Cubic interpolation

## Implementation Details

1. The program parses command-line arguments to set image properties and resampling options.
2. It reads the input image and determines whether upsampling or downsampling is required based on the input and output dimensions.
3. The appropriate resampling method is applied to generate the output image.
4. The resulting image is displayed using the Windows API.

## Example Usage

```bash
image.exe C:/myDir/myImage.rgb 4000 3000 1 O2
```

This command will read a 4000x3000 image, downsample it to 1280x720 using specific sampling.

```bash
image.exe C:/myDir/myImage.rgb 400 300 2 O1
```

This command will read a 400x300 image, upsample it to 1920x1080 using bilinear/cubic interpolation.

## Notes

- The program uses the Windows API for image display.
- Error handling is implemented for invalid input parameters.
- The code includes commented sections for potential future enhancements or debugging.
